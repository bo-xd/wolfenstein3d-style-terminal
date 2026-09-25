#include <math.h>

#include "raycast.h"

RayHit CastRay(const GameMap *map, Vec2 pos, Vec2 rayDir) {
  int mapX = (int)pos.x;
  int mapY = (int)pos.y;
  double deltaX = rayDir.x == 0.0 ? HUGE_VAL : fabs(1.0 / rayDir.x);
  double deltaY = rayDir.y == 0.0 ? HUGE_VAL : fabs(1.0 / rayDir.y);
  int stepX = rayDir.x < 0.0 ? -1 : 1;
  int stepY = rayDir.y < 0.0 ? -1 : 1;
  double sideX = rayDir.x < 0.0
    ? (pos.x - mapX) * deltaX
    : (mapX + 1.0 - pos.x) * deltaX;
  double sideY = rayDir.y < 0.0
    ? (pos.y - mapY) * deltaY
    : (mapY + 1.0 - pos.y) * deltaY;
  RaySide side = RAY_SIDE_X;

  // Prevent warning (;;) loop
  for (;;) {
    if (sideX < sideY) {
      sideX += deltaX;
      mapX += stepX;
      side = RAY_SIDE_X;
    } else {
      sideY += deltaY;
      mapY += stepY;
      side = RAY_SIDE_Y;
    }

    if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT ||
        map->tiles[mapY][mapX] == '#') {
      double distance = side == RAY_SIDE_X ? sideX - deltaX : sideY - deltaY;
      return (RayHit){distance, side};
    }
  }
}
