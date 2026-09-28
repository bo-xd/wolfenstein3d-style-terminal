#pragma once

#include "map.h"

typedef struct PlayerSettings {
  double cameraFov;
  double moveSpeed;
  double turnSpeed;
} PlayerSettings;

extern const PlayerSettings defaultPlayerSettings;

void MovementSystem(World *world, Entity playerEntity, const GameMap *map, double amount);
void TurnSystem(World *world, Entity playerEntity, double angle);
