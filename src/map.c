#include "map.h"

int MapIsWall(const GameMap *map, double x, double y) {
  if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 1;
  return map->tiles[(int)y][(int)x] == '#';
}
