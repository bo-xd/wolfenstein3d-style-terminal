#pragma once

typedef struct Vec2 {
  double x;
  double y;
} Vec2;

typedef struct Enemy {
  Vec2 pos;
  int health;
  int alive;
} Enemy;

typedef struct RayHit {
  double distance;
  int side;
} RayHit;

typedef enum ShotResult {
  SHOT_MISS,
  SHOT_HIT,
  SHOT_KILL
} ShotResult;
