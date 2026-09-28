#include <assert.h>

#include "game_config.h"
#include "pickup.h"

static Entity AddPickup(World *world, PickupType type, Vec2 position) {
  Entity entity = EcsCreate(world, ComponentPosition | ComponentPickup);
  assert(entity != EntityNone);
  world->position[entity] = position;
  world->pickup[entity] = type;
  return entity;
}

int main(void) {
  World world;
  EcsInit(&world);
  Entity player = EcsCreate(
    &world,
    ComponentPosition | ComponentHealth | ComponentPlayer
  );
  assert(player != EntityNone);
  world.position[player] = (Vec2){2.2, 3.8};
  world.player[player].ammo = 3;
  world.health[player] = 60;

  Entity ammo = AddPickup(&world, PickupAmmo, (Vec2){2.5, 3.5});
  Entity health = AddPickup(&world, PickupHealth, (Vec2){2.7, 3.1});
  Entity farAmmo = AddPickup(&world, PickupAmmo, (Vec2){4.5, 3.5});
  PickupSystem(&world, player);

  assert(world.player[player].ammo == 3 + PickupAmmoAmount);
  assert(world.health[player] == 60 + PickupHealthAmount);
  assert(!EcsHas(&world, ammo, ComponentPickup));
  assert(!EcsHas(&world, health, ComponentPickup));
  assert(EcsHas(&world, farAmmo, ComponentPickup));

  world.health[player] = PlayerMaxHealth;
  Entity fullHealth = AddPickup(&world, PickupHealth, world.position[player]);
  PickupSystem(&world, player);
  assert(world.health[player] == PlayerMaxHealth);
  assert(EcsHas(&world, fullHealth, ComponentPickup));

  world.health[player] = PlayerMaxHealth - 5;
  PickupSystem(&world, player);
  assert(world.health[player] == PlayerMaxHealth);
  assert(!EcsHas(&world, fullHealth, ComponentPickup));
  return 0;
}
