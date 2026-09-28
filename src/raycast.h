#pragma once

#include "game_types.h"
#include "map.h"

RayHit CastRay(const GameMap *map, Vec2 position, Vec2 rayDirection);
