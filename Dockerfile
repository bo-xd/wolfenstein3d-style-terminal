FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y --no-install-recommends gcc libc6-dev libncurses-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY src ./src
RUN gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c src/combat.c src/ecs.c src/map.c src/raycast.c src/weapon.c src/player.c -o wolf-terminal -lncursesw -lm

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends libncursesw6 && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=build /app/wolf-terminal ./wolf-terminal
COPY src/assets ./src/assets
ENV TERM=xterm-256color
CMD ["./wolf-terminal"]
