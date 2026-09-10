#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "combat.h"
#include "ecs.h"
#include "enemy.h"
#include "game_config.h"
#include "game_types.h"
#include "weapon.h"
#include "raycast.h"
#include "map.h"
#include "player.h"

static const double RENDER_MIN_WALL_DISTANCE = 0.001;
static const double RENDER_WALL_SHADE_DISTANCE_SCALE = 1.5;
static const double RENDER_FLOOR_DETAIL_START_RATIO = 0.75;
static const double RENDER_CEILING_DETAIL_START_RATIO = 0.20;

typedef enum GameState {
  GAME_PLAYING,
  GAME_VICTORY
} GameState;

int colorsEnabled = 0;

static const char *const VICTORY_BANNER[] = {
  "V   V IIIII  CCC  TTTTT  OOO  RRRR  Y   Y",
  "V   V   I   C       T   O   O R   R  Y Y ",
  "V   V   I   C       T   O   O RRRR    Y  ",
  " V V    I   C       T   O   O R R     Y  ",
  "  V   IIIII  CCC    T    OOO  R  RR   Y  "
};

void InitColors(void) {
  if (!has_colors() || start_color() == ERR) return;

  short wallColors[RENDER_WALL_SHADE_COUNT];
  short floorColor = COLOR_BLUE;
  short handColor = COLOR_YELLOW;
  for (int shade = 0; shade < RENDER_WALL_SHADE_COUNT; shade++) {
    wallColors[shade] = COLOR_WHITE;
  }

  if (can_change_color() && COLORS >= PALETTE_WALL_START + RENDER_WALL_SHADE_COUNT) {
    for (int shade = 0; shade < RENDER_WALL_SHADE_COUNT; shade++) {
      short color = PALETTE_WALL_START + shade;
      short gray = (short)(1000 - (shade * 750 / (RENDER_WALL_SHADE_COUNT - 1)));

      if (init_color(color, gray, gray, gray) == OK) {
        wallColors[shade] = color;
      }
    }
  }

  if (can_change_color() && COLORS > PALETTE_WEAPON_HAND) {
    if (init_color(PALETTE_FLOOR, 180, 220, 350) == OK) {
      floorColor = PALETTE_FLOOR;
    }
    if (init_color(PALETTE_WEAPON_HAND, 650, 430, 260) == OK) {
      handColor = PALETTE_WEAPON_HAND;
    }
  }

  for (int shade = 0; shade < RENDER_WALL_SHADE_COUNT; shade++) {
    init_pair((short)(shade + 1), wallColors[shade], COLOR_BLACK);
  }
  init_pair(PAIR_FLOOR, floorColor, COLOR_BLACK);
  init_pair(PAIR_HUD, COLOR_WHITE, COLOR_BLACK);
  init_pair(PAIR_ENEMY, COLOR_RED, COLOR_BLACK);
  init_pair(PAIR_WEAPON_TOP, COLOR_WHITE, COLOR_BLACK);
  init_pair(PAIR_WEAPON_FLASH, COLOR_YELLOW, COLOR_BLACK);
  init_pair(PAIR_WEAPON_HAND, handColor, COLOR_BLACK);
  init_pair(PAIR_CEILING, wallColors[RENDER_WALL_SHADE_COUNT - 2], COLOR_BLACK);
  init_pair(PAIR_WEAPON_SIDE, wallColors[4], COLOR_BLACK);
  init_pair(PAIR_WEAPON_DARK, wallColors[9], COLOR_BLACK);
  colorsEnabled = 1;
}

static void RenderVictorySystem(int frame) {
  int screenHeight;
  int screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);
  erase();

  int lineCount = (int)(sizeof(VICTORY_BANNER) / sizeof(VICTORY_BANNER[0]));
  int bannerWidth = 0;
  for (int line = 0; line < lineCount; line++) {
    int width = (int)strlen(VICTORY_BANNER[line]);
    if (width > bannerWidth) bannerWidth = width;
  }

  int fullBanner = screenWidth >= bannerWidth;
  int bannerHeight = fullBanner ? lineCount : 1;
  int targetY = (screenHeight - bannerHeight) / 2;
  if (targetY < 0) targetY = 0;
  int bannerY = frame / RENDER_VICTORY_FRAMES_PER_ROW - bannerHeight;
  if (bannerY > targetY) bannerY = targetY;

  chtype style = A_BOLD;
  if (colorsEnabled) style |= COLOR_PAIR(PAIR_WEAPON_FLASH);
  attron(style);
  for (int line = 0; line < bannerHeight; line++) {
    const char *text = fullBanner ? VICTORY_BANNER[line] : "*** VICTORY ***";
    int row = bannerY + line;
    int column = (screenWidth - (int)strlen(text)) / 2;
    if (column < 0) column = 0;
    if (row >= 0 && row < screenHeight) mvaddnstr(row, column, text, screenWidth - column);
  }
  attroff(style);

  if (bannerY == targetY) {
    const char *message = "ALL ENEMIES DEFEATED - PRESS Q OR ESC TO QUIT";
    int row = targetY + bannerHeight + 2;
    int column = (screenWidth - (int)strlen(message)) / 2;
    if (column < 0) column = 0;
    if (row < screenHeight) mvaddnstr(row, column, message, screenWidth - column);
  }
  refresh();
}

void RenderSystem(const World *world, Entity playerEntity, const GameMap *map) {
  if (!EcsHas(
        world,
        playerEntity,
        COMPONENT_POSITION | COMPONENT_DIRECTION | COMPONENT_PLAYER
      )) return;

  Vec2 pos = world->position[playerEntity];
  Vec2 dir = world->direction[playerEntity];
  PlayerState player = world->player[playerEntity];
  int screenHeight;
  int screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);

  if (screenWidth < RENDER_MIN_SCREEN_WIDTH || screenHeight < RENDER_MIN_SCREEN_HEIGHT) {
    erase();
    refresh();
    return;
  }

  erase();

  Vec2 plane = {-dir.y * PLAYER_SETTINGS.cameraFov, dir.x * PLAYER_SETTINGS.cameraFov};
  double zBuffer[screenWidth];

  for (int x = 0; x < screenWidth; x++) {
    double cameraX = 2.0 * x / screenWidth - 1.0;
    Vec2 rayDir = {
      dir.x + plane.x * cameraX,
      dir.y + plane.y * cameraX
    };

    RayHit hit = CastRay(map, pos, rayDir);
    double distance = hit.distance;
    if (distance < RENDER_MIN_WALL_DISTANCE) distance = RENDER_MIN_WALL_DISTANCE;
    zBuffer[x] = distance;

    int wallHeight = (int)(screenHeight / distance);
    int wallTop = screenHeight / 2 - wallHeight / 2;
    int wallBottom = screenHeight / 2 + wallHeight / 2;

    if (wallTop < 0) wallTop = 0;
    if (wallBottom >= screenHeight) wallBottom = screenHeight - 1;

    const char shades[RENDER_WALL_SHADE_COUNT + 1] = "@%#8O0o*+=-;:,.`";
    int shade = (int)(distance * RENDER_WALL_SHADE_DISTANCE_SCALE) +
                hit.side * RENDER_SIDE_SHADE_PENALTY;
    if (shade >= RENDER_WALL_SHADE_COUNT) shade = RENDER_WALL_SHADE_COUNT - 1;

    for (int y = 0; y < screenHeight; y++) {
      char pixel = ' ';

      if (y >= wallTop && y <= wallBottom) {
        pixel = shades[shade];
      } else if (y > wallBottom) {
        pixel = y > screenHeight * RENDER_FLOOR_DETAIL_START_RATIO ? '.' : '-';
      } else if (y > screenHeight * RENDER_CEILING_DETAIL_START_RATIO &&
                 (x + y * RENDER_CEILING_PATTERN_Y_SCALE) %
                   RENDER_CEILING_PATTERN_SPACING == 0) {
        pixel = '.';
      }
      
      chtype style = 0;
      if (colorsEnabled && y >= wallTop && y <= wallBottom) {
        style = COLOR_PAIR(shade + 1);
      } else if (colorsEnabled && y > wallBottom) {
        style = COLOR_PAIR(PAIR_FLOOR);
      } else if (colorsEnabled && pixel == '.') {
        style = COLOR_PAIR(PAIR_CEILING);
      }

      mvaddch(y, x, pixel | style);
    }
  }

  RenderEnemySystem(
    world, pos, dir, plane, screenHeight, screenWidth, zBuffer, colorsEnabled
  );

  chtype markerStyle = colorsEnabled ? COLOR_PAIR(PAIR_WEAPON_FLASH) : A_BOLD;
  char marker = player.hitMarkerTicks > 0 ? 'X' :
                player.shotTicks > 0 ? '*' : '+';
  mvaddch(screenHeight / 2, screenWidth / 2,
          marker | markerStyle | A_BOLD);

  DrawWeapon(screenHeight, screenWidth, player.shotTicks, colorsEnabled);

  char hud[128];
  snprintf(hud, sizeof(hud),
           "AMMO %02d  SCORE %04d  ENEMIES %d  POS %.1f,%.1f",
           player.ammo, player.score, CountEnemies(world), pos.x, pos.y);

  chtype hudStyle = colorsEnabled ? COLOR_PAIR(PAIR_HUD) : A_NORMAL;
  attron(hudStyle);
  move(0, 0);
  clrtoeol();
  mvaddnstr(0, 0, hud, screenWidth - 1);
  attroff(hudStyle);
  refresh();
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
  timeout(RENDER_INPUT_TIMEOUT_MS);

  int victoryFrame = 0;
  GameState gameState = GAME_PLAYING;
  while (1) {
    int key = getch();

    if (key == 'q' || key == 'Q' || key == 27) break;
    if (gameState == GAME_VICTORY) {
      RenderVictorySystem(victoryFrame++);
      continue;
    }

    switch (key) {
      case 'w':
      case 'W':
      case KEY_UP:
        MovementSystem(&world, playerEntity, &map, PLAYER_SETTINGS.moveSpeed);
        break;
      case 's':
      case 'S':
      case KEY_DOWN:
        MovementSystem(&world, playerEntity, &map, -PLAYER_SETTINGS.moveSpeed);
        break;
      case 'a':
      case 'A':
      case KEY_LEFT:
        TurnSystem(&world, playerEntity, -PLAYER_SETTINGS.turnSpeed);
        break;
      case 'd':
      case 'D':
      case KEY_RIGHT:
        TurnSystem(&world, playerEntity, PLAYER_SETTINGS.turnSpeed);
        break;
      case ' ':
      case 'f':
      case 'F':
        CombatSystem(&world, playerEntity, &map);
        break;
    }

    EnemyMovementSystem(&world, playerEntity, &map);
    if (CountEnemies(&world) == 0) {
      gameState = GAME_VICTORY;
      RenderVictorySystem(victoryFrame++);
    } else {
      RenderSystem(&world, playerEntity, &map);
      CombatTimerSystem(&world);
    }
  }

  endwin();
  return 0;
}
