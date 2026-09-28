#include <stdio.h>
#include <string.h>

#include "map.h"

int IsWall(const GameMap *map, double x, double y) {
  if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
    return 1;
  }
  return map->tiles[(int)y][(int)x] == '#';
}

static int HasClosedBorders(const GameMap *map) {
  for (int x = 0; x < MapWidth; x++) {
    if (!IsWall(map, x, 0) || !IsWall(map, x, MapHeight - 1)) {
      return 0;
    }
  }
  for (int y = 0; y < MapHeight; y++) {
    if (!IsWall(map, 0, y) || !IsWall(map, MapWidth - 1, y)) {
      return 0;
    }
  }
  return 1;
}

static Entity CreatePlayer(World *world, int tileX, int tileY) {
  Entity playerEntity = EcsCreate(
    world,
    ComponentPosition | ComponentDirection | ComponentHealth | ComponentPlayer
  );
  if (playerEntity == EntityNone) {
    return EntityNone;
  }

  world->position[playerEntity] = (Vec2){tileX + 0.5, tileY + 0.5};
  world->direction[playerEntity] = (Vec2){1.0, 0.0};
  world->health[playerEntity] = PlayerStartingHealth;
  world->player[playerEntity].ammo = CombatStartingAmmo;
  return playerEntity;
}

static Entity CreateEnemy(World *world, int tileX, int tileY) {
  Entity enemyEntity = EcsCreate(
    world,
    ComponentPosition | ComponentHealth | ComponentEnemy
  );
  if (enemyEntity == EntityNone) {
    return EntityNone;
  }

  world->position[enemyEntity] = (Vec2){tileX + 0.5, tileY + 0.5};
  world->health[enemyEntity] = CombatEnemyStartingHealth;
  return enemyEntity;
}

static Entity CreatePickup(
  World *world,
  int tileX,
  int tileY,
  PickupType pickupType
) {
  Entity pickupEntity = EcsCreate(
    world,
    ComponentPosition | ComponentPickup
  );
  if (pickupEntity == EntityNone) {
    return EntityNone;
  }

  world->position[pickupEntity] = (Vec2){tileX + 0.5, tileY + 0.5};
  world->pickup[pickupEntity] = pickupType;
  return pickupEntity;
}

int ReadMap(const char *path, GameMap *map, World *world, Entity *playerEntity) {
  FILE *file = fopen(path, "rb");
  int foundPlayer = 0;
  int enemyCount = 0;
  int pickupCount = 0;

  EcsInit(world);
  *playerEntity = EntityNone;
  if (!file) {
    return 0;
  }

  for (int y = 0; y < MapHeight; y++) {
    // The map is fixed at 16 columns, so fscanf may read at most 16 characters.
    int readLine = fscanf(file, "%16s", map->tiles[y]);
    if (readLine != 1 || strlen(map->tiles[y]) != MapWidth) {
      goto invalidMap;
    }

    for (int x = 0; x < MapWidth; x++) {
      char tile = map->tiles[y][x];
      switch (tile) {
        case 'P':
          if (foundPlayer) {
            goto invalidMap;
          }
          *playerEntity = CreatePlayer(world, x, y);
          if (*playerEntity == EntityNone) {
            goto invalidMap;
          }
          foundPlayer = 1;
          map->tiles[y][x] = '.';
          break;

        case 'E': {
          if (enemyCount >= MapMaxEnemies ||
              CreateEnemy(world, x, y) == EntityNone) {
            goto invalidMap;
          }
          enemyCount++;
          map->tiles[y][x] = '.';
          break;
        }

        case 'A':
        case 'H': {
          PickupType pickupType = tile == 'A' ? PickupAmmo : PickupHealth;
          if (pickupCount >= MapMaxPickups ||
              CreatePickup(world, x, y, pickupType) == EntityNone) {
            goto invalidMap;
          }
          pickupCount++;
          map->tiles[y][x] = '.';
          break;
        }

        case '#':
        case '.':
          break;

        default:
          goto invalidMap;
      }
    }
  }

  char extra[2];
  if (fscanf(file, "%1s", extra) == 1 ||
      !foundPlayer ||
      !HasClosedBorders(map)) {
    goto invalidMap;
  }
  fclose(file);
  return 1;

invalidMap:
  fclose(file);
  EcsInit(world);
  *playerEntity = EntityNone;
  return 0;
}
