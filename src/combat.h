#pragma once

#include "ecs.h"
#include "map.h"

int CombatTryFire(PlayerState *player);
void CombatSystem(World *world, Entity playerEntity, const GameMap *map);
void CombatTimerSystem(World *world);
