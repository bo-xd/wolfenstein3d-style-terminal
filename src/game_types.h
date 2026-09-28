#pragma once

typedef struct Vec2 {
  double x;
  double y;
} Vec2;

typedef enum RaySide {
  RaySideX,
  RaySideY
} RaySide;

typedef struct RayHit {
  double distance;
  RaySide side;
} RayHit;

typedef enum ShotResult {
  ShotMiss,
  ShotHit,
  ShotKill
} ShotResult;
