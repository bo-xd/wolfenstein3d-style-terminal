#include <math.h>

#include "raycast.h"

RayHit CastRay(const GameMap *map, Vec2 position, Vec2 rayDirection) {
  int mapTileX = (int)position.x;
  int mapTileY = (int)position.y;
  double distancePerTileX = rayDirection.x == 0.0 ?
    HUGE_VAL : fabs(1.0 / rayDirection.x);
  double distancePerTileY = rayDirection.y == 0.0 ?
    HUGE_VAL : fabs(1.0 / rayDirection.y);
  int stepX = rayDirection.x < 0.0 ? -1 : 1;
  int stepY = rayDirection.y < 0.0 ? -1 : 1;
  double distanceToNextX = rayDirection.x < 0.0
    ? (position.x - mapTileX) * distancePerTileX
    : (mapTileX + 1.0 - position.x) * distancePerTileX;
  double distanceToNextY = rayDirection.y < 0.0
    ? (position.y - mapTileY) * distancePerTileY
    : (mapTileY + 1.0 - position.y) * distancePerTileY;
  RaySide wallSide = RaySideX;

  // DDA visits one map tile at a time, always crossing the nearest grid line.
  for (;;) {
    if (distanceToNextX < distanceToNextY) {
      distanceToNextX += distancePerTileX;
      mapTileX += stepX;
      wallSide = RaySideX;
    } else {
      distanceToNextY += distancePerTileY;
      mapTileY += stepY;
      wallSide = RaySideY;
    }

    int outsideMap = mapTileX < 0 || mapTileX >= MapWidth ||
                     mapTileY < 0 || mapTileY >= MapHeight;
    if (outsideMap || map->tiles[mapTileY][mapTileX] == '#') {
      double wallDistance = wallSide == RaySideX ?
        distanceToNextX - distancePerTileX :
        distanceToNextY - distancePerTileY;
      return (RayHit){wallDistance, wallSide};
    }
  }
}
