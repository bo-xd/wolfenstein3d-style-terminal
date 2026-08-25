#include <ncurses.h>
#include <string.h>

#include "game_config.h"
#include "weapon.h"

static const char *const WEAPON_ART[] = {
  "                      ______",
  "      _______________/_____/|",
  " ____/====================| |",
  "|#########################|/",
  "|_________________________/",
  "         \\%%%%%%%\\",
  "        @@\\%%%%%%%\\@@",
  "       @@@\\_______\\@@@"
};

typedef enum WeaponSize {
  WEAPON_HEIGHT = sizeof(WEAPON_ART) / sizeof(WEAPON_ART[0])
} WeaponSize;

static ColorPairId WeaponPair(char pixel) {
  if (pixel == '@') return PAIR_WEAPON_HAND;
  if (pixel == '%') return PAIR_WEAPON_DARK;
  if (pixel == '#') return PAIR_WEAPON_SIDE;
  return PAIR_WEAPON_TOP;
}

void DrawWeapon(int screenHeight, int screenWidth, int shotTicks, int colorsEnabled) {
  int recoil = shotTicks >= COMBAT_SHOT_ANIMATION_TICKS - 1;
  int top = screenHeight - WEAPON_HEIGHT + recoil;
  int left = screenWidth / 20 - recoil;

  for (int row = 0; row < WEAPON_HEIGHT; row++) {
    size_t width = strlen(WEAPON_ART[row]);
    for (size_t column = 0; column < width; column++) {
      char pixel = WEAPON_ART[row][column];
      int y = top + row;
      int x = left + (int)column;
      if (pixel == ' ' || y < 0 || y >= screenHeight || x < 0 || x >= screenWidth) continue;
      chtype style = colorsEnabled ? COLOR_PAIR(WeaponPair(pixel)) : A_BOLD;
      mvaddch(y, x, pixel | style | A_BOLD);
    }
  }

  if (shotTicks > 0) {
    int flashX = left + (int)strlen(WEAPON_ART[1]);
    int flashY = top + 1;
    chtype style = colorsEnabled ? COLOR_PAIR(PAIR_WEAPON_FLASH) : A_BOLD;
    mvaddch(flashY, flashX, '*' | style | A_BOLD);
  }
}
