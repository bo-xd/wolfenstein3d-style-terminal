#!/bin/sh

set -e

if [ "${1:-}" = "test" ]; then
  compiler="${2:-gcc}"
  for testFile in tests/*.c; do
    [ -s "$testFile" ] || continue
    testName=${testFile##*/}
    testProgram="/tmp/wolf-${testName%.c}-test"
    echo "Testing $testFile"
    "$compiler" -std=c11 -Wall -Wextra -Wpedantic -Isrc "$testFile" src/combat.c src/ecs.c src/enemy.c src/map.c src/pickup.c src/raycast.c -o "$testProgram" -lncursesw -lm
    "$testProgram"
  done
  echo "All tests passed"
  exit 0
fi

compiler="${1:-gcc}"

"$compiler" -std=c11 -Wall -Wextra -Wpedantic src/main.c src/combat.c src/ecs.c src/enemy.c src/weapon.c src/raycast.c src/map.c src/player.c src/pickup.c -o main -lncursesw -lm

./main
