#include <assert.h>

#include "map.h"

int main(void) {
  GameMap map;
  World world;
  Entity player;

  assert(ReadMap("src/assets/map.txt", &map, &world, &player));
  assert(player != EntityNone);
  assert(EcsHas(&world, player, ComponentHealth));
  assert(world.health[player] == PlayerStartingHealth);
  int ammoPickups = 0;
  int healthPickups = 0;
  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (!EcsHas(&world, entity, ComponentPickup)) {
      continue;
    }
    if (world.pickup[entity] == PickupAmmo) {
      ammoPickups++;
    }
    if (world.pickup[entity] == PickupHealth) {
      healthPickups++;
    }
  }
  assert(ammoPickups == 1);
  assert(healthPickups == 1);
  assert(!ReadMap("tests/assets/map-zonder-speler.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/map-verkeerde-breedte.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/map-open-rand.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/map-onbekend-teken.txt", &map, &world, &player));
  assert(!ReadMap("tests/assets/bestaat-niet.txt", &map, &world, &player));
  return 0;
}
