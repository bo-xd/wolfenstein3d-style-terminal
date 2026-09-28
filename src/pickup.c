#include <math.h>
#include <ncurses.h>

#include "game_config.h"
#include "pickup.h"

static int IsOnSameTile(Vec2 firstPosition, Vec2 secondPosition) {
  return (int)firstPosition.x == (int)secondPosition.x &&
         (int)firstPosition.y == (int)secondPosition.y;
}

static char PickupPixelForType(PickupType pickupType) {
  if (pickupType == PickupHealth) {
    return '+';
  }
  return 'A';
}

static ColorPairId PickupColorPairForType(PickupType pickupType) {
  if (pickupType == PickupHealth) {
    return PairPickupHealth;
  }
  return PairPickupAmmo;
}

void PickupSystem(World *world, Entity playerEntity) {
  const uint32_t playerComponents =
    ComponentPosition | ComponentHealth | ComponentPlayer;
  if (!EcsHas(world, playerEntity, playerComponents)) {
    return;
  }

  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    int isPickup = EcsHas(world, entity, ComponentPosition | ComponentPickup);
    if (!isPickup) {
      continue;
    }

    int isPlayerTile = IsOnSameTile(
      world->position[playerEntity],
      world->position[entity]
    );
    if (!isPlayerTile) {
      continue;
    }

    if (world->pickup[entity] == PickupAmmo) {
      world->player[playerEntity].ammo += PickupAmmoAmount;
    } else if (world->pickup[entity] == PickupHealth) {
      if (world->health[playerEntity] >= PlayerMaxHealth) {
        continue;
      }
      world->health[playerEntity] += PickupHealthAmount;
      if (world->health[playerEntity] > PlayerMaxHealth) {
        world->health[playerEntity] = PlayerMaxHealth;
      }
    } else {
      continue;
    }

    EcsDestroy(world, entity);
  }
}

void RenderPickupSystem(
  const World *world,
  Vec2 playerPosition,
  Vec2 playerDirection,
  Vec2 cameraPlane,
  int screenHeight,
  int screenWidth,
  const double *zBuffer,
  int colorsEnabled
) {
  double determinant = cameraPlane.x * playerDirection.y -
                       playerDirection.x * cameraPlane.y;
  if (fabs(determinant) < 0.0001) {
    return;
  }

  double inverseDeterminant = 1.0 / determinant;
  for (Entity entity = 0; entity < EcsMaxEntities; entity++) {
    if (!EcsHas(world, entity, ComponentPosition | ComponentPickup)) {
      continue;
    }

    double relativeX = world->position[entity].x - playerPosition.x;
    double relativeY = world->position[entity].y - playerPosition.y;
    double cameraSpaceX = inverseDeterminant *
      (playerDirection.y * relativeX - playerDirection.x * relativeY);
    double depth = inverseDeterminant *
      (-cameraPlane.y * relativeX + cameraPlane.x * relativeY);
    if (depth <= 0.1) {
      continue;
    }

    int spriteSize = (int)fabs(screenHeight / depth) / 3;
    if (spriteSize < 1) {
      spriteSize = 1;
    }
    int screenX = (int)((screenWidth / 2.0) *
                        (1.0 + cameraSpaceX / depth));
    int top = screenHeight / 2 + spriteSize / 2;
    int left = screenX - spriteSize / 2;
    char pixel = PickupPixelForType(world->pickup[entity]);
    ColorPairId colorPair = PickupColorPairForType(world->pickup[entity]);

    for (int x = left; x < left + spriteSize; x++) {
      if (x < 0 || x >= screenWidth || depth >= zBuffer[x]) {
        continue;
      }
      for (int y = top; y < top + spriteSize && y < screenHeight; y++) {
        if (y < 0) {
          continue;
        }
        chtype style = colorsEnabled && COLOR_PAIRS > (int)colorPair ?
          COLOR_PAIR(colorPair) : A_BOLD;
        mvaddch(y, x, pixel | style | A_BOLD);
      }
    }
  }
}
