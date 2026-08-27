#pragma once

#include "ecs.h"
#include "game_config.h"

typedef struct GameMap {
  char tiles[MAP_HEIGHT][MAP_WIDTH + 1];
} GameMap;

int IsWall(const GameMap *map, double x, double y);
int ReadMap(const char *path, GameMap *map, World *world, Entity *playerEntity);
