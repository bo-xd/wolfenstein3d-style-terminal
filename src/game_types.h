#pragma once

typedef struct Vec2 {
  double x;
  double y;
} Vec2;

typedef enum RaySide {
  RAY_SIDE_X,
  RAY_SIDE_Y
} RaySide;

typedef struct RayHit {
  double distance;
  RaySide side;
} RayHit;

typedef enum ShotResult {
  SHOT_MISS,
  SHOT_HIT,
  SHOT_KILL
} ShotResult;
