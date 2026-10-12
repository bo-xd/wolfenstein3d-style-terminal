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
  EnemyMovementSystem(&world, player, &map, 1);
  assert(world.position[enemy].x < 4.5);
  assert(fabs(world.position[enemy].y - 2.5) < 0.0001);

  MakeOpenMap(&map);
  map.tiles[2][3] = '#';
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){4.5, 2.5});
  EnemyMovementSystem(&world, player, &map, 1);
  assert(fabs(world.position[enemy].x - 4.5) < 0.0001);
  assert(fabs(world.position[enemy].y - 2.5) > 0.0001);
  assert(!IsWall(&map, world.position[enemy].x, world.position[enemy].y));

  MakeOpenMap(&map);
  map.tiles[3][4] = '#';
  map.tiles[4][3] = '#';
  map.tiles[4][5] = '#';
  map.tiles[5][4] = '#';
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){4.5, 4.5});
  EnemyMovementSystem(&world, player, &map, 1);
  assert(fabs(world.position[enemy].x - 4.5) < 0.0001);
  assert(fabs(world.position[enemy].y - 4.5) < 0.0001);

  MakeOpenMap(&map);
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){3.1, 2.5});
  EnemyMovementSystem(&world, player, &map, 1);
  assert(fabs(world.position[enemy].x - 3.1) < 0.0001);

  MakeOpenMap(&map);
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){12.5, 2.5});
  EnemyMovementSystem(&world, player, &map, 1);
  assert(fabs(world.position[enemy].x - 12.5) < 0.0001);

  world.health[enemy] = CombatEnemyStartingHealth - 1;
  EnemyMovementSystem(&world, player, &map, 1);
  assert(world.position[enemy].x < 12.5);

  MakeOpenMap(&map);
  map.tiles[2][3] = '#';
  EcsInit(&world);
  player = EcsCreate(&world, ComponentPosition | ComponentPlayer);
  (void)EcsCreate(&world, ComponentPosition | ComponentEnemy);
  Entity enemyB = EcsCreate(&world, ComponentPosition | ComponentEnemy);
  Entity enemyC = EcsCreate(&world, ComponentPosition | ComponentEnemy);
  world.position[player] = (Vec2){2.5, 2.5};
  world.position[enemyB] = (Vec2){4.5, 2.5};
  world.position[enemyC] = (Vec2){4.5, 2.5};
  EnemyMovementSystem(&world, player, &map, 1);
  assert(fabs(world.position[enemyB].y - world.position[enemyC].y) > 0.0001);

  MakeOpenMap(&map);
  map.tiles[2][3] = '#';
  EcsInit(&world);
  player = EcsCreate(&world, ComponentPosition | ComponentPlayer);
  Entity blocker = EcsCreate(&world, ComponentPosition | ComponentEnemy);
  Entity chaser = EcsCreate(&world, ComponentPosition | ComponentEnemy);
  world.position[player] = (Vec2){2.5, 2.5};
  world.position[blocker] = (Vec2){4.5, 1.5};
  world.position[chaser] = (Vec2){4.5, 2.5};
  EnemyMovementSystem(&world, player, &map, 1);
  assert(world.position[chaser].y > 2.5);

  EcsInit(&world);
  player = EcsCreate(&world, ComponentPosition | ComponentHealth | ComponentPlayer);
  enemy = EcsCreate(&world, ComponentPosition | ComponentHealth | ComponentEnemy);
  world.position[player] = (Vec2){2.5, 2.5};
  world.health[player] = 100;
  world.position[enemy] = (Vec2){2.5, 3.0};
  world.health[enemy] = CombatEnemyStartingHealth;
  EnemyAttackSystem(&world, player);
  assert(world.health[player] == 100 - CombatEnemyDamage);
  assert(world.direction[enemy].y > 0.0);

  EnemyAttackSystem(&world, player);
  assert(world.health[player] == 100 - CombatEnemyDamage);

  world.health[player] = 5;
  world.direction[enemy].y = 0.0;
  EnemyAttackSystem(&world, player);
  assert(world.health[player] == 0);

  world.health[player] = 100;
  world.position[enemy] = (Vec2){2.5, 5.0};
  world.direction[enemy].y = 0.0;
  EnemyAttackSystem(&world, player);
  assert(world.health[player] == 100);

  MakeOpenMap(&map);
  map.enemySpawnCount = 2;
  map.enemySpawns[0] = (Vec2){3.5, 4.5};
  map.enemySpawns[1] = (Vec2){7.5, 8.5};
  EcsInit(&world);
  SpawnEnemyWave(&world, &map);
  assert(CountEnemies(&world) == 2);

  MakeOpenMap(&map);
  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){5.5, 2.5});
  EnemyMovementSystem(&world, player, &map, 1);
  double xWave1 = world.position[enemy].x;

  AddPlayerAndEnemy(&world, &player, &enemy, (Vec2){2.5, 2.5}, (Vec2){5.5, 2.5});
  EnemyMovementSystem(&world, player, &map, 3);
  double xWave3 = world.position[enemy].x;
  assert(xWave3 < xWave1);

  return 0;
}
