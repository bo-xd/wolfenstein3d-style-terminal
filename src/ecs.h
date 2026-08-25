#pragma once

#include <stdint.h>

#include "game_types.h"

#define ECS_MAX_ENTITIES 128
#define ENTITY_NONE UINT16_MAX

typedef uint16_t Entity;

typedef enum ComponentMask {
  COMPONENT_POSITION = 1u << 0,
  COMPONENT_DIRECTION = 1u << 1,
  COMPONENT_HEALTH = 1u << 2,
  COMPONENT_PLAYER = 1u << 3,
  COMPONENT_ENEMY = 1u << 4
} ComponentMask;

typedef struct PlayerState {
  int ammo;
  int score;
  int shotCooldown;
  int shotTicks;
  int hitMarkerTicks;
} PlayerState;

typedef struct World {
  uint32_t mask[ECS_MAX_ENTITIES];
  Vec2 position[ECS_MAX_ENTITIES];
  Vec2 direction[ECS_MAX_ENTITIES];
  int health[ECS_MAX_ENTITIES];
  PlayerState player[ECS_MAX_ENTITIES];
} World;

void EcsInit(World *world);
Entity EcsCreate(World *world, uint32_t components);
void EcsDestroy(World *world, Entity entity);
int EcsHas(const World *world, Entity entity, uint32_t components);
