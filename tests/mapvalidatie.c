#include <assert.h>

#include "map.h"

int main(void) {
  GameMap map;
  World world;
  Entity player;

  assert(ReadMap("src/assets/map.txt", &map, &world, &player));
  assert(player != ENTITY_NONE);
  assert(!ReadMap("tests/assets/map-zonder-speler.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/map-verkeerde-breedte.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/map-open-rand.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/map-onbekend-teken.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/bestaat-niet.txt", &map, &world, &player));
  return 0;
}
