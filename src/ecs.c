#include <string.h>

#include "ecs.h"

void EcsInit(World *world) {
  memset(world, 0, sizeof(*world));
}

Entity EcsCreate(World *world, uint32_t components) {
  if (components == 0) return ENTITY_NONE;

  for (Entity entity = 0; entity < ECS_MAX_ENTITIES; entity++) {
    if (world->mask[entity] == 0) {
      world->mask[entity] = components;
      return entity;
    }
  }

  return ENTITY_NONE;
}

void EcsDestroy(World *world, Entity entity) {
  if (entity >= ECS_MAX_ENTITIES) return;

  world->mask[entity] = 0;
  world->position[entity] = (Vec2){0.0, 0.0};
  world->direction[entity] = (Vec2){0.0, 0.0};
  world->health[entity] = 0;
  world->player[entity] = (PlayerState){0};
}

int EcsHas(const World *world, Entity entity, uint32_t components) {
  return components != 0 && entity < ECS_MAX_ENTITIES &&
         (world->mask[entity] & components) == components;
}
