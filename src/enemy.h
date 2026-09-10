#pragma once

#include "map.h"

int CountEnemies(const World *world);
void EnemyMovementSystem(World *world, Entity playerEntity, const GameMap *map);
void RenderEnemySystem(
  const World *world,
  Vec2 playerPosition,
  Vec2 playerDirection,
  Vec2 cameraPlane,
  int screenHeight,
  int screenWidth,
  const double *zBuffer,
  int colorsEnabled
);
