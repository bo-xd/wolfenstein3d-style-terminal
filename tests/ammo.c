#include <assert.h>

#include "combat.h"

int main(void) {
  PlayerState player = {.ammo = 2};

  assert(CombatTryFire(&player));
  assert(player.ammo == 1);
  assert(!CombatTryFire(&player));
  assert(player.ammo == 1);

  player.shotCooldown = 0;
  assert(CombatTryFire(&player));
  assert(player.ammo == 0);

  player.shotCooldown = 0;
  assert(!CombatTryFire(&player));
  assert(player.ammo == 0);
  return 0;
}
