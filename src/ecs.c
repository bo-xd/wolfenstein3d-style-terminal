#include <string.h>

#include "ecs.h"

void EcsInit(World *world) {
  memset(world, 0, sizeof(*world));
}

Entity EcsCreate(World *world, uint32_t components) {
  if (components == 0) {
    return EntityNone;
  }

  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (world->mask[entity] == 0) {
      world->mask[entity] = components;
      return entity;
    }
  }

  return EntityNone;
}

void EcsDestroy(World *world, Entity entity) {
  if (entity >= EcsMaxEntities) {
    return;
  }

  world->mask[entity] = 0;
  world->position[entity] = (Vec2){0.0, 0.0};
  world->direction[entity] = (Vec2){0.0, 0.0};
  world->health[entity] = 0;
  world->player[entity] = (PlayerState){0};
  world->pickup[entity] = PickupNone;
}

int EcsHas(const World *world, Entity entity, uint32_t components) {
  return components != 0 && entity < EcsMaxEntities &&
         (world->mask[entity] & components) == components;
}
