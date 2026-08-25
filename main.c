#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>


// macros
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define FOV 0.66
#define MOVE_SPEED 0.20
#define TURN_SPEED 0.12
#define WALL_SHADE_COUNT 16
#define CUSTOM_WALL_COLOR_START 16
#define FLOOR_PAIR 17
#define HUD_PAIR 18

typedef struct vec2 {
  double x;
  double y;
} vec2;

char map[MAP_HEIGHT][MAP_WIDTH + 1];
int colorsEnabled = 0;


void InitColors(void) {
  if (!has_colors() || start_color() == ERR) return;

  short wallColors[WALL_SHADE_COUNT];
  for (int shade = 0; shade < WALL_SHADE_COUNT; shade++) {
    wallColors[shade] = COLOR_WHITE;
  }

  if (can_change_color() && COLORS >= CUSTOM_WALL_COLOR_START + WALL_SHADE_COUNT) {
    for (int shade = 0; shade < WALL_SHADE_COUNT; shade++) {
      short color = CUSTOM_WALL_COLOR_START + shade;
      short gray = (short)(800 - (shade * 740 / (WALL_SHADE_COUNT - 1)));

      if (init_color(color, gray, gray, gray) == OK) {
        wallColors[shade] = color;
      }
    }
  }

  for (int shade = 0; shade < WALL_SHADE_COUNT; shade++) {
    init_pair((short)(shade + 1), wallColors[shade], COLOR_BLACK);
  }
  init_pair(FLOOR_PAIR, COLOR_BLUE, COLOR_BLACK);
  init_pair(HUD_PAIR, COLOR_WHITE, COLOR_BLACK);
  colorsEnabled = 1;
}

// dit opened en leest de file
int ReadFile(const char *path, vec2 *player) {
  FILE *file = fopen(path, "rb");
  int foundPlayer = 0;

  if (!file) return 0;

  for (int y = 0; y < MAP_HEIGHT; y++) {
    if (fscanf(file, "%16s", map[y]) != 1 || strlen(map[y]) != MAP_WIDTH) {
      fclose(file);
      return 0;
    }

    for (int x = 0; x < MAP_WIDTH; x++) {
      if (map[y][x] == 'P') {
        player->x = x + 0.5;
        player->y = y + 0.5;
        map[y][x] = '.';
        foundPlayer = 1;
      }
    }
  }

  fclose(file);
  return foundPlayer;
}


int IsWall(double x, double y) {
  int mapX = (int)x;
  int mapY = (int)y;

  if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
    return 1;
  }

  return map[mapY][mapX] == '#';
}


void raytrace(vec2 pos, vec2 dir) {
  int screenHeight;
  int screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);

  if (screenWidth < 20 || screenHeight < 10) {
    erase();
    refresh();
    return;
  }

  erase();

  vec2 plane = {-dir.y * FOV, dir.x * FOV};

  for (int x = 0; x < screenWidth; x++) {
    double cameraX = 2.0 * x / screenWidth - 1.0;
    vec2 rayDir = {
      dir.x + plane.x * cameraX,
      dir.y + plane.y * cameraX
    };

    int mapX = (int)pos.x;
    int mapY = (int)pos.y;

    double deltaX = rayDir.x == 0.0 ? 1e30 : fabs(1.0 / rayDir.x);
    double deltaY = rayDir.y == 0.0 ? 1e30 : fabs(1.0 / rayDir.y);
    double sideX;
    double sideY;
    int stepX;
    int stepY;

    if (rayDir.x < 0.0) {
      stepX = -1;
      sideX = (pos.x - mapX) * deltaX;
    } else {
      stepX = 1;
      sideX = (mapX + 1.0 - pos.x) * deltaX;
    }

    if (rayDir.y < 0.0) {
      stepY = -1;
      sideY = (pos.y - mapY) * deltaY;
    } else {
      stepY = 1;
      sideY = (mapY + 1.0 - pos.y) * deltaY;
    }

    int side = 0;
    int hit = 0;

    while (!hit) {
      if (sideX < sideY) {
        sideX += deltaX;
        mapX += stepX;
        side = 0;
      } else {
        sideY += deltaY;
        mapY += stepY;
        side = 1;
      }

      if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
        hit = 1;
      } else if (map[mapY][mapX] == '#') {
        hit = 1;
      }
    }

    double distance = side == 0 ? sideX - deltaX : sideY - deltaY;
    if (distance < 0.001) distance = 0.001;

    int wallHeight = (int)(screenHeight / distance);
    int wallTop = screenHeight / 2 - wallHeight / 2;
    int wallBottom = screenHeight / 2 + wallHeight / 2;

    if (wallTop < 0) wallTop = 0;
    if (wallBottom >= screenHeight) wallBottom = screenHeight - 1;

    const char shades[WALL_SHADE_COUNT + 1] = "@%#8&$0?*+=-:,.`";
    int shade = (int)(distance * 1.5) + side * 2;
    if (shade >= WALL_SHADE_COUNT) shade = WALL_SHADE_COUNT - 1;

    for (int y = 0; y < screenHeight; y++) {
      char pixel = ' ';

      if (y >= wallTop && y <= wallBottom) {
        pixel = shades[shade];
      } else if (y > wallBottom) {
        pixel = y > screenHeight * 3 / 4 ? '.' : '-';
      }
      
      chtype style = 0;
      if (colorsEnabled && y >= wallTop && y <= wallBottom) {
        style = COLOR_PAIR(shade + 1);
      } else if (colorsEnabled && y > wallBottom) {
        style = COLOR_PAIR(FLOOR_PAIR);
      }

      mvaddch(y, x, pixel | style);
    }
  }

  attron(colorsEnabled ? COLOR_PAIR(HUD_PAIR) : A_NORMAL);
  mvprintw(0, 0, "(%.1f, %.1f)", pos.x, pos.y);
  attroff(colorsEnabled ? COLOR_PAIR(HUD_PAIR) : A_NORMAL);
  refresh();
}

void MovePlayer(vec2 *pos, vec2 dir, double amount) {
  double nextX = pos->x + dir.x * amount;
  double nextY = pos->y + dir.y * amount;

  if (!IsWall(nextX, pos->y)) pos->x = nextX;
  if (!IsWall(pos->x, nextY)) pos->y = nextY;
}

void TurnPlayer(vec2 *dir, double angle) {
  double oldX = dir->x;
  dir->x = dir->x * cos(angle) - dir->y * sin(angle);
  dir->y = oldX * sin(angle) + dir->y * cos(angle);
}

int main(void) {
  vec2 player;
  vec2 direction = {1.0, 0.0};

  if (!ReadFile("map.txt", &player)) {
    fprintf(stderr, "Could not read map.txt or find P\n");
    return 1;
  }

  initscr();
  InitColors();

  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(16);

  int running = 1;
  while (running) {
    int key = getch();

    switch (key) {
      case 'w':
      case 'W':
      case KEY_UP:
        MovePlayer(&player, direction, MOVE_SPEED);
        break;
      case 's':
      case 'S':
      case KEY_DOWN:
        MovePlayer(&player, direction, -MOVE_SPEED);
        break;
      case 'a':
      case 'A':
      case KEY_LEFT:
        TurnPlayer(&direction, -TURN_SPEED);
        break;
      case 'd':
      case 'D':
      case KEY_RIGHT:
        TurnPlayer(&direction, TURN_SPEED);
        break;
      case 'q':
      case 'Q':
      case 27:
        running = 0;
        break;
    }

    raytrace(player, direction);
  }

  endwin();
  return 0;
}
