#include "player.h"

void MovementSystem(World *world, Entity playerEntity, const GameMap *map, double amount) {
  if (!EcsHas(
        world,
        playerEntity,
        COMPONENT_POSITION | COMPONENT_DIRECTION | COMPONENT_PLAYER
      )) return;

  Vec2 *pos = &world->position[playerEntity];
  Vec2 dir = world->direction[playerEntity];
  double nextX = pos->x + dir.x * amount;
  double nextY = pos->y + dir.y * amount;

  if (!IsWall(map, nextX, pos->y)) pos->x = nextX;
  if (!IsWall(map, pos->x, nextY)) pos->y = nextY;
}

void TurnSystem(World *world, Entity playerEntity, double angle) {
  if (!EcsHas(
        world,
        playerEntity,
        COMPONENT_DIRECTION | COMPONENT_PLAYER
      )) return;

  Vec2 *dir = &world->direction[playerEntity];
  double oldX = dir->x;
  dir->x = dir->x * cos(angle) - dir->y * sin(angle);
  dir->y = oldX * sin(angle) + dir->y * cos(angle);
}
