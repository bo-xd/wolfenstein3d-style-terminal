#include <math.h>
#include <ncurses.h>

#include "enemy.h"
#include "game_config.h"

static const double enemyMoveSpeed = 0.015;
static const double enemyStopDistance = 0.75;
static const int neighborOffsets[4][2] = {
  {1, 0},
  {-1, 0},
  {0, 1},
  {0, -1}
};

// Breadth-first search stores how many open tiles lead from each tile to the player.
static void BuildPathDistances(
  const GameMap *map,
  int playerTileX,
  int playerTileY,
  int distanceToPlayer[MapHeight][MapWidth]
) {
  int queueX[MapWidth * MapHeight];
  int queueY[MapWidth * MapHeight];
  int queueStart = 0;
  int queueEnd = 0;

  for (int y = 0; y < MapHeight; y++) {
    for (int x = 0; x < MapWidth; x++) {
      distanceToPlayer[y][x] = -1;
    }
  }
  if (IsWall(map, playerTileX, playerTileY)) {
    return;
  }

  distanceToPlayer[playerTileY][playerTileX] = 0;
  queueX[queueEnd] = playerTileX;
  queueY[queueEnd] = playerTileY;
  queueEnd++;

  while (queueStart < queueEnd) {
    int tileX = queueX[queueStart];
    int tileY = queueY[queueStart];
    queueStart++;

    for (int neighborIndex = 0; neighborIndex < 4; neighborIndex++) {
      int nextX = tileX + neighborOffsets[neighborIndex][0];
      int nextY = tileY + neighborOffsets[neighborIndex][1];
      if (IsWall(map, nextX, nextY) ||
          distanceToPlayer[nextY][nextX] >= 0) {
        continue;
      }

      distanceToPlayer[nextY][nextX] =
        distanceToPlayer[tileY][tileX] + 1;
      queueX[queueEnd] = nextX;
      queueY[queueEnd] = nextY;
      queueEnd++;
    }
  }
}

void EnemyMovementSystem(World *world, Entity playerEntity, const GameMap *map) {
  if (!EcsHas(world, playerEntity, ComponentPosition | ComponentPlayer)) {
    return;
  }

  Vec2 playerPosition = world->position[playerEntity];
  int distanceToPlayer[MapHeight][MapWidth];
  BuildPathDistances(
    map,
    (int)playerPosition.x,
    (int)playerPosition.y,
    distanceToPlayer
  );

  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (!EcsHas(world, entity, ComponentPosition | ComponentEnemy)) {
      continue;
    }

    Vec2 *position = &world->position[entity];
    double distanceX = playerPosition.x - position->x;
    double distanceY = playerPosition.y - position->y;
    if (hypot(distanceX, distanceY) <= enemyStopDistance) {
      continue;
    }

    int tileX = (int)position->x;
    int tileY = (int)position->y;
    int outsideMap = tileX < 0 || tileX >= MapWidth ||
                     tileY < 0 || tileY >= MapHeight;
    if (outsideMap) {
      continue;
    }

    int nextX = tileX;
    int nextY = tileY;
    int bestDistance = distanceToPlayer[tileY][tileX];
    if (bestDistance <= 0) {
      continue;
    }

    for (int neighborIndex = 0; neighborIndex < 4; neighborIndex++) {
      int candidateX = tileX + neighborOffsets[neighborIndex][0];
      int candidateY = tileY + neighborOffsets[neighborIndex][1];
      if (IsWall(map, candidateX, candidateY)) {
        continue;
      }

      int candidateDistance = distanceToPlayer[candidateY][candidateX];
      if (candidateDistance >= 0 && candidateDistance < bestDistance) {
        nextX = candidateX;
        nextY = candidateY;
        bestDistance = candidateDistance;
      }
    }

    double moveX = nextX + 0.5 - position->x;
    double moveY = nextY + 0.5 - position->y;
    double moveDistance = hypot(moveX, moveY);
    if (moveDistance <= 0.0) {
      continue;
    }

    double step = fmin(enemyMoveSpeed, moveDistance) / moveDistance;
    Vec2 newPosition = {
      position->x + moveX * step,
      position->y + moveY * step
    };
    if (!IsWall(map, newPosition.x, newPosition.y)) {
      *position = newPosition;
    }
  }
}

int CountEnemies(const World *world) {
  int enemyCount = 0;
  const uint32_t requiredComponents =
    ComponentPosition | ComponentHealth | ComponentEnemy;

  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (EcsHas(world, entity, requiredComponents)) {
      enemyCount++;
    }
  }
  return enemyCount;
}

static char EnemyPixelForHeight(double verticalRatio) {
  if (verticalRatio < 0.25) {
    return 'O';
  }
  if (verticalRatio < 0.75) {
    return '#';
  }
  return '|';
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
  if (fabs(determinant) < 0.0001) {
    return;
  }

  double inverseDeterminant = 1.0 / determinant;
  const uint32_t requiredComponents =
    ComponentPosition | ComponentHealth | ComponentEnemy;

  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (!EcsHas(world, entity, requiredComponents)) {
      continue;
    }

    double relativeX = world->position[entity].x - playerPosition.x;
    double relativeY = world->position[entity].y - playerPosition.y;
    double cameraSpaceX = inverseDeterminant *
      (playerDirection.y * relativeX - playerDirection.x * relativeY);
    double depth = inverseDeterminant *
      (-cameraPlane.y * relativeX + cameraPlane.x * relativeY);

    if (depth <= 0.1) {
      continue;
    }

    int spriteHeight = (int)fabs(screenHeight / depth);
    int spriteWidth = spriteHeight / 2;
    if (spriteWidth < 1) {
      spriteWidth = 1;
    }

    int screenX = (int)((screenWidth / 2.0) *
                        (1.0 + cameraSpaceX / depth));
    int top = screenHeight / 2 - spriteHeight / 2;
    int bottom = screenHeight / 2 + spriteHeight / 2;
    int left = screenX - spriteWidth / 2;
    int right = screenX + spriteWidth / 2;

    if (top < 0) {
      top = 0;
    }
    if (bottom >= screenHeight) {
      bottom = screenHeight - 1;
    }

    for (int x = left; x <= right; x++) {
      if (x < 0 || x >= screenWidth || depth >= zBuffer[x]) {
        continue;
      }

      double horizontalRatio = (x - left + 0.5) / (right - left + 1);
      for (int y = top; y <= bottom; y++) {
        double verticalRatio = (y - top + 0.5) / (bottom - top + 1);
        int outsideHead = verticalRatio < 0.25 &&
                          fabs(horizontalRatio - 0.5) > 0.25;
        int betweenLegs = verticalRatio > 0.75 &&
                          fabs(horizontalRatio - 0.5) < 0.15;
        if (outsideHead || betweenLegs) {
          continue;
        }

        char pixel = EnemyPixelForHeight(verticalRatio);
        chtype style = colorsEnabled ? COLOR_PAIR(PairEnemy) : A_BOLD;
        mvaddch(y, x, pixel | style | A_BOLD);
      }
    }
  }
}
