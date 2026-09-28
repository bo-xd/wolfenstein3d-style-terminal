#include <assert.h>

#include "map.h"

int main(void) {
  GameMap map = {0};
  map.tiles[2][3] = '#';

  assert(IsWall(&map, 3.5, 2.5));
  assert(!IsWall(&map, 2.5, 2.5));
  assert(IsWall(&map, -0.1, 2.0));
  assert(IsWall(&map, MapWidth, 2.0));
  assert(IsWall(&map, 2.0, -0.1));
  assert(IsWall(&map, 2.0, MapHeight));
  return 0;
}
