#include <assert.h>
#include <math.h>

#include "raycast.h"

int main(void) {
  GameMap map = {0};
  map.tiles[2][4] = '#';
  map.tiles[4][2] = '#';

  RayHit horizontal = CastRay(&map, (Vec2){2.5, 2.5}, (Vec2){1.0, 0.0});
  assert(fabs(horizontal.distance - 1.5) < 0.0001);
  assert(horizontal.side == RAY_SIDE_X);

  RayHit vertical = CastRay(&map, (Vec2){2.5, 2.5}, (Vec2){0.0, 1.0});
  assert(fabs(vertical.distance - 1.5) < 0.0001);
  assert(vertical.side == RAY_SIDE_Y);
  return 0;
}
