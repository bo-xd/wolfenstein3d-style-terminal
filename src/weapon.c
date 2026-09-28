#include <ncurses.h>
#include <string.h>

#include "game_config.h"
#include "weapon.h"

static const char *const weaponArt[] = {
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
  WeaponHeight = sizeof(weaponArt) / sizeof(weaponArt[0])
} WeaponSize;

static ColorPairId ColorPairForWeaponPixel(char pixel) {
  if (pixel == '@') {
    return PairWeaponHand;
  }
  if (pixel == '%') {
    return PairWeaponDark;
  }
  if (pixel == '#') {
    return PairWeaponSide;
  }
  return PairWeaponTop;
}

void DrawWeapon(int screenHeight, int screenWidth, int shotTicks, int colorsEnabled) {
  int recoil = shotTicks >= CombatShotAnimationTicks - 1;
  int top = screenHeight - WeaponHeight + recoil;
  int left = screenWidth / 20 - recoil;

  for (int row = 0; row < WeaponHeight; row++) {
    size_t width = strlen(weaponArt[row]);
    for (size_t column = 0; column < width; column++) {
      char pixel = weaponArt[row][column];
      int y = top + row;
      int x = left + (int)column;
      int outsideScreen = y < 0 || y >= screenHeight ||
                          x < 0 || x >= screenWidth;
      if (pixel == ' ' || outsideScreen) {
        continue;
      }
      chtype style = colorsEnabled ?
        COLOR_PAIR(ColorPairForWeaponPixel(pixel)) : A_BOLD;
      mvaddch(y, x, pixel | style | A_BOLD);
    }
  }

  if (shotTicks > 0) {
    int flashX = left + (int)strlen(weaponArt[1]);
    int flashY = top + 1;
    chtype style = colorsEnabled ? COLOR_PAIR(PairWeaponFlash) : A_BOLD;
    mvaddch(flashY, flashX, '*' | style | A_BOLD);
  }
}
