#pragma once

typedef enum MapLimit {
  MapWidth = 16,
  MapHeight = 16,
  MapMaxEnemies = 16,
  MapMaxPickups = 16
} MapLimit;

typedef enum PlayerSetting {
  PlayerStartingHealth = 100,
  PlayerMaxHealth = 100
} PlayerSetting;

typedef enum PickupSetting {
  PickupAmmoAmount = 12,
  PickupHealthAmount = 25
} PickupSetting;

typedef enum CombatSetting {
  CombatStartingAmmo = 48,
  CombatShotCooldown = 8,
  CombatShotAnimationTicks = 5,
  CombatEnemyStartingHealth = 2,
  CombatKillScore = 100
} CombatSetting;

typedef enum RenderSetting {
  RenderMinScreenWidth = 20,
  RenderMinScreenHeight = 10,
  RenderInputTimeoutMilliseconds = 16,
  RenderWallShadeCount = 16,
  RenderSideShadePenalty = 2,
  RenderCeilingPatternYScale = 3,
  RenderCeilingPatternSpacing = 7,
  RenderVictoryFramesPerRow = 2
} RenderSetting;

typedef enum ColorPairId {
  PairFloor = 17,
  PairHud = 18,
  PairEnemy = 19,
  PairWeaponTop = 20,
  PairWeaponFlash = 21,
  PairWeaponHand = 22,
  PairCeiling = 23,
  PairWeaponSide = 24,
  PairWeaponDark = 25,
  PairPickupAmmo = 26,
  PairPickupHealth = 27
} ColorPairId;

typedef enum PaletteColorId {
  PaletteWallStart = 16,
  PaletteFloor = 32,
  PaletteWeaponHand = 33
} PaletteColorId;
