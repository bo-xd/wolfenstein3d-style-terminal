#pragma once

#include "ecs.h"

typedef RayHit (*RaycastFunction)(Vec2 pos, Vec2 direction);

int CombatTryFire(PlayerState *player);
void CombatSystem(World *world, Entity playerEntity, RaycastFunction castRay);
void CombatTimerSystem(World *world);
