#pragma once

#include <stdint.h>

#include "game_types.h"

#define EcsMaxEntities 128
#define EntityNone UINT16_MAX

typedef uint16_t Entity;

typedef enum ComponentMask {
  ComponentPosition = 1u << 0,
  ComponentDirection = 1u << 1,
  ComponentHealth = 1u << 2,
  ComponentPlayer = 1u << 3,
  ComponentEnemy = 1u << 4,
  ComponentPickup = 1u << 5
} ComponentMask;

typedef enum PickupType {
  PickupNone,
  PickupAmmo,
  PickupHealth
} PickupType;

typedef struct PlayerState {
  int ammo;
  int score;
  int shotCooldown;
  int shotTicks;
  int hitMarkerTicks;
} PlayerState;

typedef struct World {
  uint32_t mask[EcsMaxEntities];
  Vec2 position[EcsMaxEntities];
  Vec2 direction[EcsMaxEntities];
  int health[EcsMaxEntities];
  PlayerState player[EcsMaxEntities];
  PickupType pickup[EcsMaxEntities];
} World;

void EcsInit(World *world);
Entity EcsCreate(World *world, uint32_t components);
void EcsDestroy(World *world, Entity entity);
int EcsHas(const World *world, Entity entity, uint32_t components);
