#pragma once

#include "game_config.h"

typedef struct GameMap {
  char tiles[MAP_HEIGHT][MAP_WIDTH + 1];
} GameMap;

int MapIsWall(const GameMap *map, double x, double y);
