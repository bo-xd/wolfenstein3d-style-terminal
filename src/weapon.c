#include <ncurses.h>
#include <string.h>

#include "game_config.h"
#include "weapon.h"

static const char *const weaponArt[] = {
  "         /--\\         ",
  "        | () |        ",
  "       /======\\       ",
  "      /|======|\\      ",
  "     | |######| |     ",
  "     | |######| |     ",
  "     | |%%%%%%| |     ",
  "     \\@@|%%%%|@@/     ",
  "      \\@@\\%%/@@/      ",
  "       \\@@@@@@/       "
};

static const char *const muzzleFlashArt[] = {
  "      \\  |  /      ",
  "     --- * ---     ",
  "      /  |  \\      "
};

typedef enum WeaponSize {
  WeaponHeight = sizeof(weaponArt) / sizeof(weaponArt[0]),
  WeaponWidth = 22,
  FlashHeight = sizeof(muzzleFlashArt) / sizeof(muzzleFlashArt[0]),
  FlashWidth = 19
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
  int recoil = shotTicks >= CombatShotAnimationTicks - 2 ? 1 : 0;
  int top = screenHeight - WeaponHeight + recoil;
  int left = (screenWidth - WeaponWidth) / 2;

  if (shotTicks > 0) {
    int flashTop = top - FlashHeight;
    int flashLeft = (screenWidth - FlashWidth) / 2;
    chtype flashStyle = colorsEnabled ?
      COLOR_PAIR(PairWeaponFlash) : A_BOLD;

    for (int row = 0; row < FlashHeight; row++) {
      size_t width = strlen(muzzleFlashArt[row]);
      for (size_t column = 0; column < width; column++) {
        char pixel = muzzleFlashArt[row][column];
        int y = flashTop + row;
        int x = flashLeft + (int)column;
        if (pixel == ' ' || y < 0 || y >= screenHeight || x < 0 || x >= screenWidth) {
          continue;
        }
        mvaddch(y, x, pixel | flashStyle | A_BOLD);
      }
    }
  }

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
}
