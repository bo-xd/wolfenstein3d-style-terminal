#include <stdio.h>
#include <string.h>

#include "map.h"

int IsWall(const GameMap *map, double x, double y) {
  if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 1;
  return map->tiles[(int)y][(int)x] == '#';
}

static int HasClosedBorders(const GameMap *map) {
  for (int x = 0; x < MAP_WIDTH; x++) {
    if (!IsWall(map, x, 0) || !IsWall(map, x, MAP_HEIGHT - 1)) return 0;
  }
  for (int y = 0; y < MAP_HEIGHT; y++) {
    if (!IsWall(map, 0, y) || !IsWall(map, MAP_WIDTH - 1, y)) return 0;
  }
  return 1;
}

int ReadMap(const char *path, GameMap *map, World *world, Entity *playerEntity) {
  FILE *file = fopen(path, "rb");
  int foundPlayer = 0;
  int enemyCount = 0;

  EcsInit(world);
  *playerEntity = ENTITY_NONE;
  if (!file) return 0;

  for (int y = 0; y < MAP_HEIGHT; y++) {
    if (fscanf(file, "%16s", map->tiles[y]) != 1 || strlen(map->tiles[y]) != MAP_WIDTH) goto invalid;

    for (int x = 0; x < MAP_WIDTH; x++) {
      char tile = map->tiles[y][x];
      if (tile == 'P') {
        if (foundPlayer) goto invalid;
        *playerEntity = EcsCreate(world, COMPONENT_POSITION | COMPONENT_DIRECTION | COMPONENT_PLAYER);
        if (*playerEntity == ENTITY_NONE) goto invalid;
        world->direction[*playerEntity] = (Vec2){1.0, 0.0};
        world->player[*playerEntity].ammo = COMBAT_STARTING_AMMO;
        world->position[*playerEntity] = (Vec2){x + 0.5, y + 0.5};
        map->tiles[y][x] = '.';
        foundPlayer = 1;
      } else if (tile == 'E') {
        if (enemyCount >= MAP_MAX_ENEMIES) goto invalid;
        Entity enemy = EcsCreate(world, COMPONENT_POSITION | COMPONENT_HEALTH | COMPONENT_ENEMY);
        if (enemy == ENTITY_NONE) goto invalid;
        world->position[enemy] = (Vec2){x + 0.5, y + 0.5};
        world->health[enemy] = COMBAT_ENEMY_STARTING_HEALTH;
        enemyCount++;
        map->tiles[y][x] = '.';
      } else if (tile != '#' && tile != '.') goto invalid;
    }
  }

  char extra[2];
  if (fscanf(file, "%1s", extra) == 1 || !foundPlayer || !HasClosedBorders(map)) goto invalid;
  fclose(file);
  return 1;

invalid:
  fclose(file);
  EcsInit(world);
  *playerEntity = ENTITY_NONE;
  return 0;
}
