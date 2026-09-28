#include <assert.h>
#include <math.h>

#include "enemy.h"

static void MakeOpenMap(GameMap *map) {
  for (int y = 0; y < MapHeight; y++) {
    for (int x = 0; x < MapWidth; x++) {
      int isBorder = x == 0 || x == MapWidth - 1 ||
                     y == 0 || y == MapHeight - 1;
      map->tiles[y][x] = isBorder ? '#' : '.';
    }
    map->tiles[y][MapWidth] = '\0';
  }
}

static void AddPlayerAndEnemy(
  World *world,
  Entity *player,
  Entity *enemy,
  Vec2 playerPosition,
  Vec2 enemyPosition
) {
  EcsInit(world);
  *player = EcsCreate(world, ComponentPosition | ComponentPlayer);
  *enemy = EcsCreate(world, ComponentPosition | ComponentEnemy);
  world->position[*player] = playerPosition;
  world->position[*enemy] = enemyPosition;
}

int main(void) {
  GameMap map;
  World world;
  Entity player;
  Entity enemy;

  MakeOpenMap(&map);
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){4.5, 2.5});
  EnemyMovementSystem(&world, player, &map);
  assert(world.position[enemy].x < 4.5);
  assert(fabs(world.position[enemy].y - 2.5) < 0.0001);

  MakeOpenMap(&map);
  map.tiles[2][3] = '#';
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){4.5, 2.5});
  EnemyMovementSystem(&world, player, &map);
  assert(fabs(world.position[enemy].x - 4.5) < 0.0001);
  assert(fabs(world.position[enemy].y - 2.5) > 0.0001);
  assert(!IsWall(&map, world.position[enemy].x, world.position[enemy].y));

  MakeOpenMap(&map);
  map.tiles[3][4] = '#';
  map.tiles[4][3] = '#';
  map.tiles[4][5] = '#';
  map.tiles[5][4] = '#';
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){4.5, 4.5});
  EnemyMovementSystem(&world, player, &map);
  assert(fabs(world.position[enemy].x - 4.5) < 0.0001);
  assert(fabs(world.position[enemy].y - 4.5) < 0.0001);

  MakeOpenMap(&map);
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){3.1, 2.5});
  EnemyMovementSystem(&world, player, &map);
  assert(fabs(world.position[enemy].x - 3.1) < 0.0001);

  return 0;
}
