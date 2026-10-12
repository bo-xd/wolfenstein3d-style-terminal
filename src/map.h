#pragma once

#include "ecs.h"
#include "game_config.h"

typedef struct GameMap {
  char tiles[MapHeight][MapWidth + 1];
  Vec2 enemySpawns[MapMaxEnemies];
  int enemySpawnCount;
} GameMap;

int IsWall(const GameMap *map, double x, double y);
int ReadMap(const char *path, GameMap *map, World *world, Entity *playerEntity);
