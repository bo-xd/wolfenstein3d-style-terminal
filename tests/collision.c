#include <assert.h>

#include "map.h"

int main(void) {
  GameMap map = {0};
  map.tiles[2][3] = '#';

  assert(MapIsWall(&map, 3.5, 2.5));
  assert(!MapIsWall(&map, 2.5, 2.5));
  assert(MapIsWall(&map, -0.1, 2.0));
  assert(MapIsWall(&map, MAP_WIDTH, 2.0));
  assert(MapIsWall(&map, 2.0, -0.1));
  assert(MapIsWall(&map, 2.0, MAP_HEIGHT));
  return 0;
}
