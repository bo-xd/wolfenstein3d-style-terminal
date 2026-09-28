#include <math.h>

#include "combat.h"
#include "game_config.h"
#include "raycast.h"

static const double enemyRadius = 0.30;

static ShotResult Shoot(
  World *world,
  Vec2 playerPosition,
  Vec2 shotDirection,
  const GameMap *map
) {
  double closestHitDistance =
    CastRay(map, playerPosition, shotDirection).distance;
  Entity targetEntity = EntityNone;
  const uint32_t requiredComponents =
    ComponentPosition | ComponentHealth | ComponentEnemy;

  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (!EcsHas(world, entity, requiredComponents)) {
      continue;
    }

    double relativeX = world->position[entity].x - playerPosition.x;
    double relativeY = world->position[entity].y - playerPosition.y;
    double distanceForward =
      relativeX * shotDirection.x + relativeY * shotDirection.y;
    double distanceSideways = fabs(
      relativeX * shotDirection.y - relativeY * shotDirection.x
    );

    if (distanceForward <= 0.0 || distanceSideways > enemyRadius) {
      continue;
    }

    double hitOffset = sqrt(
      enemyRadius * enemyRadius - distanceSideways * distanceSideways
    );
    double hitDistance = distanceForward - hitOffset;
    if (hitDistance >= 0.0 && hitDistance < closestHitDistance) {
      closestHitDistance = hitDistance;
      targetEntity = entity;
    }
  }

  if (targetEntity == EntityNone) {
    return ShotMiss;
  }

  world->health[targetEntity]--;
  if (world->health[targetEntity] > 0) {
    return ShotHit;
  }

  EcsDestroy(world, targetEntity);
  return ShotKill;
}

int CombatTryFire(PlayerState *player) {
  if (player->shotCooldown > 0 || player->ammo <= 0) {
    return 0;
  }

  player->ammo--;
  player->shotCooldown = CombatShotCooldown;
  player->shotTicks = CombatShotAnimationTicks;
  return 1;
}

void CombatSystem(World *world, Entity playerEntity, const GameMap *map) {
  const uint32_t requiredComponents =
    ComponentPosition | ComponentDirection | ComponentPlayer;
  if (!EcsHas(world, playerEntity, requiredComponents) ||
      !CombatTryFire(&world->player[playerEntity])) {
    return;
  }

  PlayerState *player = &world->player[playerEntity];
  ShotResult shotResult = Shoot(
    world,
    world->position[playerEntity],
    world->direction[playerEntity],
    map
  );
  if (shotResult == ShotKill) {
    player->score += CombatKillScore;
  }
  if (shotResult != ShotMiss) {
    player->hitMarkerTicks = CombatShotAnimationTicks;
  }
}

void CombatTimerSystem(World *world) {
  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (!EcsHas(world, entity, ComponentPlayer)) {
      continue;
    }

    PlayerState *player = &world->player[entity];
    if (player->shotCooldown > 0) {
      player->shotCooldown--;
    }
    if (player->shotTicks > 0) {
      player->shotTicks--;
    }
    if (player->hitMarkerTicks > 0) {
      player->hitMarkerTicks--;
    }
  }
}
