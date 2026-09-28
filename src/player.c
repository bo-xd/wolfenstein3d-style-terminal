#include <math.h>

#include "player.h"

const PlayerSettings defaultPlayerSettings = {
  .cameraFov = 0.66,
  .moveSpeed = 0.20,
  .turnSpeed = 0.12
};

void MovementSystem(World *world, Entity playerEntity, const GameMap *map, double amount) {
  if (!EcsHas(
        world,
        playerEntity,
        ComponentPosition | ComponentDirection | ComponentPlayer
      )) {
    return;
  }

  Vec2 *position = &world->position[playerEntity];
  Vec2 direction = world->direction[playerEntity];
  double nextX = position->x + direction.x * amount;
  double nextY = position->y + direction.y * amount;

  if (!IsWall(map, nextX, position->y)) {
    position->x = nextX;
  }
  if (!IsWall(map, position->x, nextY)) {
    position->y = nextY;
  }
}

void TurnSystem(World *world, Entity playerEntity, double angle) {
  if (!EcsHas(
        world,
        playerEntity,
        ComponentDirection | ComponentPlayer
      )) {
    return;
  }

  Vec2 *direction = &world->direction[playerEntity];
  double oldX = direction->x;
  direction->x = direction->x * cos(angle) - direction->y * sin(angle);
  direction->y = oldX * sin(angle) + direction->y * cos(angle);
}
