#include <math.h>
#include <ncurses.h>

#include "enemy.h"
#include "game_config.h"

static const double ENEMY_MOVE_SPEED = 0.015;
static const double ENEMY_STOP_DISTANCE = 0.75;
static const int NEIGHBOR_OFFSETS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// BFS pathfinding https://nl.wikipedia.org/wiki/Breadth-first_search
static void BuildPathDistances(const GameMap *map, int goalX, int goalY, int distance[MAP_HEIGHT][MAP_WIDTH]) {
  int queueX[MAP_WIDTH * MAP_HEIGHT];
  int queueY[MAP_WIDTH * MAP_HEIGHT];
  int first = 0;
  int last = 0;

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) distance[y][x] = -1;
  }
  if (IsWall(map, goalX, goalY)) return;

  distance[goalY][goalX] = 0;
  queueX[last] = goalX;
  queueY[last++] = goalY;

  while (first < last) {
    int x = queueX[first];
    int y = queueY[first++];
    for (int direction = 0; direction < 4; direction++) {
      int nextX = x + NEIGHBOR_OFFSETS[direction][0];
      int nextY = y + NEIGHBOR_OFFSETS[direction][1];
      if (IsWall(map, nextX, nextY) || distance[nextY][nextX] >= 0) continue;
      distance[nextY][nextX] = distance[y][x] + 1;
      queueX[last] = nextX;
      queueY[last++] = nextY;
    }
  }
}

void EnemyMovementSystem(World *world, Entity playerEntity, const GameMap *map) {
  if (!EcsHas(world, playerEntity, COMPONENT_POSITION | COMPONENT_PLAYER)) return;

  Vec2 playerPosition = world->position[playerEntity];
  int distance[MAP_HEIGHT][MAP_WIDTH];
  BuildPathDistances(map, (int)playerPosition.x, (int)playerPosition.y, distance);

  for (Entity entity = 0; entity < ECS_MAX_ENTITIES; entity++) {
    if (!EcsHas(world, entity, COMPONENT_POSITION | COMPONENT_ENEMY)) continue;

    Vec2 *position = &world->position[entity];
    double playerX = playerPosition.x - position->x;
    double playerY = playerPosition.y - position->y;
    if (hypot(playerX, playerY) <= ENEMY_STOP_DISTANCE) continue;

    int tileX = (int)position->x;
    int tileY = (int)position->y;
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT) continue;

    int nextX = tileX;
    int nextY = tileY;
    int bestDistance = distance[tileY][tileX];
    if (bestDistance <= 0) continue;

    for (int direction = 0; direction < 4; direction++) {
      int candidateX = tileX + NEIGHBOR_OFFSETS[direction][0];
      int candidateY = tileY + NEIGHBOR_OFFSETS[direction][1];
      if (IsWall(map, candidateX, candidateY)) continue;
      if (distance[candidateY][candidateX] >= 0 && distance[candidateY][candidateX] < bestDistance) {
        nextX = candidateX;
        nextY = candidateY;
        bestDistance = distance[candidateY][candidateX];
      }
    }

    double moveX = nextX + 0.5 - position->x;
    double moveY = nextY + 0.5 - position->y;
    double moveDistance = hypot(moveX, moveY);
    if (moveDistance <= 0.0) continue;
    double step = fmin(ENEMY_MOVE_SPEED, moveDistance) / moveDistance;
    double newX = position->x + moveX * step;
    double newY = position->y + moveY * step;
    if (!IsWall(map, newX, newY)) *position = (Vec2){newX, newY};
  }
}

int CountEnemies(const World *world) {
  int alive = 0;
  const uint32_t required =
    COMPONENT_POSITION | COMPONENT_HEALTH | COMPONENT_ENEMY;

  for (Entity entity = 0; entity < ECS_MAX_ENTITIES; entity++) {
    if (EcsHas(world, entity, required)) alive++;
  }
  return alive;
}

void RenderEnemySystem(
  const World *world,
  Vec2 playerPosition,
  Vec2 playerDirection,
  Vec2 cameraPlane,
  int screenHeight,
  int screenWidth,
  const double *zBuffer,
  int colorsEnabled
) {
  double determinant = cameraPlane.x * playerDirection.y -
                       playerDirection.x * cameraPlane.y;
  if (fabs(determinant) < 0.0001) return;

  double inverseDeterminant = 1.0 / determinant;
  const uint32_t required =
    COMPONENT_POSITION | COMPONENT_HEALTH | COMPONENT_ENEMY;

  for (Entity entity = 0; entity < ECS_MAX_ENTITIES; entity++) {
    if (!EcsHas(world, entity, required)) continue;

    double relativeX = world->position[entity].x - playerPosition.x;
    double relativeY = world->position[entity].y - playerPosition.y;
    double transformX = inverseDeterminant *
      (playerDirection.y * relativeX - playerDirection.x * relativeY);
    double transformY = inverseDeterminant *
      (-cameraPlane.y * relativeX + cameraPlane.x * relativeY);

    if (transformY <= 0.1) continue;

    int spriteHeight = (int)fabs(screenHeight / transformY);
    int spriteWidth = spriteHeight / 2;
    if (spriteWidth < 1) spriteWidth = 1;

    int screenX = (int)((screenWidth / 2.0) *
                        (1.0 + transformX / transformY));
    int top = screenHeight / 2 - spriteHeight / 2;
    int bottom = screenHeight / 2 + spriteHeight / 2;
    int left = screenX - spriteWidth / 2;
    int right = screenX + spriteWidth / 2;

    if (top < 0) top = 0;
    if (bottom >= screenHeight) bottom = screenHeight - 1;

    for (int x = left; x <= right; x++) {
      if (x < 0 || x >= screenWidth || transformY >= zBuffer[x]) continue;

      double u = (x - left + 0.5) / (right - left + 1);
      for (int y = top; y <= bottom; y++) {
        double v = (y - top + 0.5) / (bottom - top + 1);
        if ((v < 0.25 && fabs(u - 0.5) > 0.25) ||
            (v > 0.75 && fabs(u - 0.5) < 0.15)) continue;

        char pixel = v < 0.25 ? 'O' : v < 0.75 ? '#' : '|';
        chtype style = colorsEnabled ? COLOR_PAIR(PAIR_ENEMY) : A_BOLD;
        mvaddch(y, x, pixel | style | A_BOLD);
      }
    }
  }
}
