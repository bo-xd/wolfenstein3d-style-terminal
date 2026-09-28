#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "combat.h"
#include "ecs.h"
#include "enemy.h"
#include "game_config.h"
#include "game_types.h"
#include "map.h"
#include "pickup.h"
#include "player.h"
#include "raycast.h"
#include "weapon.h"

static const double renderMinWallDistance = 0.001;
static const double renderWallShadeDistanceScale = 1.5;
static const double renderFloorDetailStartRatio = 0.75;
static const double renderCeilingDetailStartRatio = 0.20;

typedef enum GameState {
  GamePlaying,
  GameVictory
} GameState;

static int colorsEnabled = 0;

static const char *const victoryBanner[] = {
  "V   V IIIII  CCC  TTTTT  OOO  RRRR  Y   Y",
  "V   V   I   C       T   O   O R   R  Y Y ",
  "V   V   I   C       T   O   O RRRR    Y  ",
  " V V    I   C       T   O   O R R     Y  ",
  "  V   IIIII  CCC    T    OOO  R  RR   Y  "
};

static void InitColors(void) {
  if (!has_colors() || start_color() == ERR) {
    return;
  }

  short wallColors[RenderWallShadeCount];
  short floorColor = COLOR_BLUE;
  short handColor = COLOR_YELLOW;
  for (int shade = 0; shade < RenderWallShadeCount; shade++) {
    wallColors[shade] = COLOR_WHITE;
  }

  if (can_change_color() && COLORS >= PaletteWallStart + RenderWallShadeCount) {
    for (int shade = 0; shade < RenderWallShadeCount; shade++) {
      short color = PaletteWallStart + shade;
      short gray = (short)(1000 - (shade * 750 / (RenderWallShadeCount - 1)));

      if (init_color(color, gray, gray, gray) == OK) {
        wallColors[shade] = color;
      }
    }
  }

  if (can_change_color() && COLORS > PaletteWeaponHand) {
    if (init_color(PaletteFloor, 180, 220, 350) == OK) {
      floorColor = PaletteFloor;
    }
    if (init_color(PaletteWeaponHand, 650, 430, 260) == OK) {
      handColor = PaletteWeaponHand;
    }
  }

  for (int shade = 0; shade < RenderWallShadeCount; shade++) {
    init_pair((short)(shade + 1), wallColors[shade], COLOR_BLACK);
  }
  init_pair(PairFloor, floorColor, COLOR_BLACK);
  init_pair(PairHud, COLOR_WHITE, COLOR_BLACK);
  init_pair(PairEnemy, COLOR_RED, COLOR_BLACK);
  init_pair(PairWeaponTop, COLOR_WHITE, COLOR_BLACK);
  init_pair(PairWeaponFlash, COLOR_YELLOW, COLOR_BLACK);
  init_pair(PairWeaponHand, handColor, COLOR_BLACK);
  init_pair(PairCeiling, wallColors[RenderWallShadeCount - 2], COLOR_BLACK);
  init_pair(PairWeaponSide, wallColors[4], COLOR_BLACK);
  init_pair(PairWeaponDark, wallColors[9], COLOR_BLACK);
  if (COLOR_PAIRS > PairPickupHealth) {
    init_pair(PairPickupAmmo, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PairPickupHealth, COLOR_GREEN, COLOR_BLACK);
  }
  colorsEnabled = 1;
}

static void RenderVictorySystem(int frame) {
  int screenHeight;
  int screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);
  erase();

  int lineCount = (int)(sizeof(victoryBanner) / sizeof(victoryBanner[0]));
  int bannerWidth = 0;
  for (int line = 0; line < lineCount; line++) {
    int width = (int)strlen(victoryBanner[line]);
    if (width > bannerWidth) {
      bannerWidth = width;
    }
  }

  int fullBanner = screenWidth >= bannerWidth;
  int bannerHeight = fullBanner ? lineCount : 1;
  int targetY = (screenHeight - bannerHeight) / 2;
  if (targetY < 0) {
    targetY = 0;
  }
  int bannerY = frame / RenderVictoryFramesPerRow - bannerHeight;
  if (bannerY > targetY) {
    bannerY = targetY;
  }

  chtype style = A_BOLD;
  if (colorsEnabled) {
    style |= COLOR_PAIR(PairWeaponFlash);
  }
  attron(style);
  for (int line = 0; line < bannerHeight; line++) {
    const char *text = fullBanner ? victoryBanner[line] : "*** VICTORY ***";
    int row = bannerY + line;
    int column = (screenWidth - (int)strlen(text)) / 2;
    if (column < 0) {
      column = 0;
    }
    if (row >= 0 && row < screenHeight) {
      mvaddnstr(row, column, text, screenWidth - column);
    }
  }
  attroff(style);

  if (bannerY == targetY) {
    const char *message = "ALL ENEMIES DEFEATED - PRESS Q OR ESC TO QUIT";
    int row = targetY + bannerHeight + 2;
    int column = (screenWidth - (int)strlen(message)) / 2;
    if (column < 0) {
      column = 0;
    }
    if (row < screenHeight) {
      mvaddnstr(row, column, message, screenWidth - column);
    }
  }
  refresh();
}

static char CrosshairPixel(PlayerState player) {
  if (player.hitMarkerTicks > 0) {
    return 'X';
  }
  if (player.shotTicks > 0) {
    return '*';
  }
  return '+';
}

static void RenderHud(
  const World *world,
  Entity playerEntity,
  Vec2 playerPosition,
  int screenWidth
) {
  PlayerState player = world->player[playerEntity];
  char hud[128];
  if (screenWidth < 60) {
    snprintf(hud, sizeof(hud), "HP %d A %d E %d",
             world->health[playerEntity], player.ammo, CountEnemies(world));
  } else {
    snprintf(hud, sizeof(hud),
             "HEALTH %03d  AMMO %02d  SCORE %04d  ENEMIES %d  POS %.1f,%.1f",
             world->health[playerEntity], player.ammo, player.score,
             CountEnemies(world), playerPosition.x, playerPosition.y);
  }

  chtype hudStyle = colorsEnabled ? COLOR_PAIR(PairHud) : A_NORMAL;
  attron(hudStyle);
  move(0, 0);
  clrtoeol();
  mvaddnstr(0, 0, hud, screenWidth - 1);
  attroff(hudStyle);
}

static void RenderSystem(
  const World *world,
  Entity playerEntity,
  const GameMap *map
) {
  if (!EcsHas(
        world,
        playerEntity,
        ComponentPosition | ComponentDirection | ComponentPlayer
      )) {
    return;
  }

  Vec2 playerPosition = world->position[playerEntity];
  Vec2 playerDirection = world->direction[playerEntity];
  PlayerState player = world->player[playerEntity];
  int screenHeight;
  int screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);

  if (screenWidth < RenderMinScreenWidth || screenHeight < RenderMinScreenHeight) {
    erase();
    refresh();
    return;
  }

  erase();

  Vec2 cameraPlane = {
    -playerDirection.y * defaultPlayerSettings.cameraFov,
    playerDirection.x * defaultPlayerSettings.cameraFov
  };
  double zBuffer[screenWidth];

  for (int x = 0; x < screenWidth; x++) {
    double cameraX = 2.0 * x / screenWidth - 1.0;
    Vec2 rayDirection = {
      playerDirection.x + cameraPlane.x * cameraX,
      playerDirection.y + cameraPlane.y * cameraX
    };

    RayHit rayHit = CastRay(map, playerPosition, rayDirection);
    double wallDistance = rayHit.distance;
    if (wallDistance < renderMinWallDistance) {
      wallDistance = renderMinWallDistance;
    }
    zBuffer[x] = wallDistance;

    int wallHeight = (int)(screenHeight / wallDistance);
    int wallTop = screenHeight / 2 - wallHeight / 2;
    int wallBottom = screenHeight / 2 + wallHeight / 2;

    if (wallTop < 0) {
      wallTop = 0;
    }
    if (wallBottom >= screenHeight) {
      wallBottom = screenHeight - 1;
    }

    const char wallShades[RenderWallShadeCount + 1] = "@%#8O0o*+=-;:,.`";
    int shadeIndex = (int)(wallDistance * renderWallShadeDistanceScale) +
                     rayHit.side * RenderSideShadePenalty;
    if (shadeIndex >= RenderWallShadeCount) {
      shadeIndex = RenderWallShadeCount - 1;
    }

    for (int y = 0; y < screenHeight; y++) {
      char pixel = ' ';

      if (y >= wallTop && y <= wallBottom) {
        pixel = wallShades[shadeIndex];
      } else if (y > wallBottom) {
        if (y > screenHeight * renderFloorDetailStartRatio) {
          pixel = '.';
        } else {
          pixel = '-';
        }
      } else if (y > screenHeight * renderCeilingDetailStartRatio &&
                 (x + y * RenderCeilingPatternYScale) %
                   RenderCeilingPatternSpacing == 0) {
        pixel = '.';
      }
      
      chtype style = 0;
      if (colorsEnabled && y >= wallTop && y <= wallBottom) {
        style = COLOR_PAIR(shadeIndex + 1);
      } else if (colorsEnabled && y > wallBottom) {
        style = COLOR_PAIR(PairFloor);
      } else if (colorsEnabled && pixel == '.') {
        style = COLOR_PAIR(PairCeiling);
      }

      mvaddch(y, x, pixel | style);
    }
  }

  RenderEnemySystem(
    world,
    playerPosition,
    playerDirection,
    cameraPlane,
    screenHeight,
    screenWidth,
    zBuffer,
    colorsEnabled
  );
  RenderPickupSystem(
    world,
    playerPosition,
    playerDirection,
    cameraPlane,
    screenHeight,
    screenWidth,
    zBuffer,
    colorsEnabled
  );

  chtype markerStyle = colorsEnabled ? COLOR_PAIR(PairWeaponFlash) : A_BOLD;
  char marker = CrosshairPixel(player);
  mvaddch(screenHeight / 2, screenWidth / 2,
          marker | markerStyle | A_BOLD);

  DrawWeapon(screenHeight, screenWidth, player.shotTicks, colorsEnabled);
  RenderHud(world, playerEntity, playerPosition, screenWidth);
  refresh();
}

static int ShouldQuit(int key) {
  return key == 'q' || key == 'Q' || key == 27;
}

static void HandleInput(
  int key,
  World *world,
  Entity playerEntity,
  const GameMap *map
) {
  switch (key) {
    case 'w':
    case 'W':
    case KEY_UP:
      MovementSystem(
        world,
        playerEntity,
        map,
        defaultPlayerSettings.moveSpeed
      );
      break;
    case 's':
    case 'S':
    case KEY_DOWN:
      MovementSystem(
        world,
        playerEntity,
        map,
        -defaultPlayerSettings.moveSpeed
      );
      break;
    case 'a':
    case 'A':
    case KEY_LEFT:
      TurnSystem(world, playerEntity, -defaultPlayerSettings.turnSpeed);
      break;
    case 'd':
    case 'D':
    case KEY_RIGHT:
      TurnSystem(world, playerEntity, defaultPlayerSettings.turnSpeed);
      break;
    case ' ':
    case 'f':
    case 'F':
      CombatSystem(world, playerEntity, map);
      break;
  }
}

int main(void) {
  GameMap map;
  World world;
  Entity playerEntity;

  if (!ReadMap("src/assets/map.txt", &map, &world, &playerEntity)) {
    fprintf(stderr, "Could not read map.txt or find P\n");
    return 1;
  }

  initscr();
  InitColors();

  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(RenderInputTimeoutMilliseconds);

  int victoryFrame = 0;
  GameState gameState = GamePlaying;
  while (1) {
    int key = getch();

    if (ShouldQuit(key)) {
      break;
    }
    if (gameState == GameVictory) {
      RenderVictorySystem(victoryFrame++);
      continue;
    }

    HandleInput(key, &world, playerEntity, &map);

    PickupSystem(&world, playerEntity);
    EnemyMovementSystem(&world, playerEntity, &map);
    if (CountEnemies(&world) == 0) {
      gameState = GameVictory;
      RenderVictorySystem(victoryFrame++);
    } else {
      RenderSystem(&world, playerEntity, &map);
      CombatTimerSystem(&world);
    }
  }

  endwin();
  return 0;
}
