#pragma once

#include "ecs.h"
#include "game_types.h"

void PickupSystem(World *world, Entity playerEntity);
void RenderPickupSystem(
  const World *world,
  Vec2 playerPosition,
  Vec2 playerDirection,
  Vec2 cameraPlane,
  int screenHeight,
  int screenWidth,
  const double *zBuffer,
  int colorsEnabled
);
