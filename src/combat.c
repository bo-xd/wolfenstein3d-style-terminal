#include <math.h>

#include "combat.h"
#include "game_config.h"

static const double ENEMY_RADIUS = 0.30;

static ShotResult Shoot(World *world, Vec2 pos, Vec2 dir, RaycastFunction castRay) {
  double closestHit = castRay(pos, dir).distance;
  Entity target = ENTITY_NONE;
  const uint32_t required = COMPONENT_POSITION | COMPONENT_HEALTH | COMPONENT_ENEMY;

  for (Entity entity = 0; entity < ECS_MAX_ENTITIES; entity++) {
    if (!EcsHas(world, entity, required)) continue;

    double relativeX = world->position[entity].x - pos.x;
    double relativeY = world->position[entity].y - pos.y;
    double forward = relativeX * dir.x + relativeY * dir.y;
    double lateral = fabs(relativeX * dir.y - relativeY * dir.x);

    if (forward <= 0.0 || lateral > ENEMY_RADIUS) continue;

    double hitOffset = sqrt(ENEMY_RADIUS * ENEMY_RADIUS - lateral * lateral);
    double hitDistance = forward - hitOffset;
    if (hitDistance >= 0.0 && hitDistance < closestHit) {
      closestHit = hitDistance;
      target = entity;
    }
  }

  if (target == ENTITY_NONE) return SHOT_MISS;

  world->health[target]--;
  if (world->health[target] > 0) return SHOT_HIT;

  EcsDestroy(world, target);
  return SHOT_KILL;
}

int CombatTryFire(PlayerState *player) {
  if (player->shotCooldown > 0 || player->ammo <= 0) return 0;

  player->ammo--;
  player->shotCooldown = COMBAT_SHOT_COOLDOWN;
  player->shotTicks = COMBAT_SHOT_ANIMATION_TICKS;
  return 1;
}

void CombatSystem(World *world, Entity playerEntity, RaycastFunction castRay) {
  const uint32_t required = COMPONENT_POSITION | COMPONENT_DIRECTION | COMPONENT_PLAYER;
  if (!EcsHas(world, playerEntity, required) || !CombatTryFire(&world->player[playerEntity])) return;

  PlayerState *player = &world->player[playerEntity];
  ShotResult result = Shoot(world, world->position[playerEntity], world->direction[playerEntity], castRay);
  if (result == SHOT_KILL) player->score += COMBAT_KILL_SCORE;
  if (result != SHOT_MISS) player->hitMarkerTicks = COMBAT_SHOT_ANIMATION_TICKS;
}

void CombatTimerSystem(World *world) {
  for (Entity entity = 0; entity < ECS_MAX_ENTITIES; entity++) {
    if (!EcsHas(world, entity, COMPONENT_PLAYER)) continue;

    PlayerState *player = &world->player[entity];
    if (player->shotCooldown > 0) player->shotCooldown--;
    if (player->shotTicks > 0) player->shotTicks--;
    if (player->hitMarkerTicks > 0) player->hitMarkerTicks--;
  }
}
