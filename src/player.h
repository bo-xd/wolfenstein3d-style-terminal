#pragma once

#include "map.h"
#include <math.h>

// typedef enum PlayerSettings {
//   PLAYER_CAMERA_FOV = 0.66,
//   PLAYER_MOVE_SPEED = 0.20,
//   PLAYER_TURN_SPEED = 0.12
// } PlayerSettings;

typedef struct PlayerSettings {
  double cameraFov;
  double moveSpeed;
  double turnSpeed;
} PlayerSettings;

static const PlayerSettings PLAYER_SETTINGS = {
  .cameraFov = 0.66,
  .moveSpeed = 0.20,
  .turnSpeed = 0.12
}; 

void MovementSystem(World *world, Entity playerEntity, const GameMap *map, double amount);
void TurnSystem(World *world, Entity playerEntity, double angle);
