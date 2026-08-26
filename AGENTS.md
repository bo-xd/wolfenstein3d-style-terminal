# AGENTS.md

## Project context

- This is an educational Wolfenstein 3D-style terminal game written in C11.
- Rendering and input use `ncursesw`; gameplay must remain usable without a
  network connection, account, cloud service, or AI API.
- The project demonstrates DDA raycasting, collision detection, an
  array-backed ECS, enemy projection, and hitscan combat.
- The map is a fixed 16 by 16 text grid in `src/assets/map.txt`.
- This is an independent school project. Do not use original Wolfenstein
  source code, levels, graphics, audio, or other copyrighted assets.

## Educational expectations

- Treat AI as an assistant, not as the author or decision-maker.
- Keep changes small enough that the student can understand and explain them.
- Explain unfamiliar C, mathematics, algorithms, and trade-offs in plain
  language when they are relevant to a change.
- Do not fabricate project history, prompts, test results, sources, manual
  checks, or claims that the student reviewed something.
- Clearly distinguish verified facts from suggestions and assumptions.
- Prefer the simplest solution that meets the requirement; avoid unnecessary
  abstractions, frameworks, dependencies, and large rewrites.

## Architecture and code rules

- Preserve C11 compatibility and compile with `-Wall -Wextra -Wpedantic`.
- Follow the existing style: two-space indentation, braces on the same line,
  English identifiers, and focused source/header modules.
- Keep public declarations in headers and make file-local helpers `static`.
- Put shared configuration constants in `src/game_config.h` when appropriate;
  avoid unexplained magic numbers.
- Preserve the current separation between map data, ECS data, raycasting,
  combat, weapon presentation, and the main game loop unless a requested
  change justifies a different boundary.
- Avoid dynamic allocation when fixed-size storage is sufficient for the
  project's known limits.
- Do not add a production dependency without explaining why it is necessary
  and receiving approval.
- Do not edit or commit generated binaries such as `main`.

## Correctness and safety

- Validate all map and entity indices before accessing fixed-size arrays.
- Treat positions outside the map as blocked.
- Consider zero-length directions, division by zero, very small ray distances,
  terminal resizing, and terminals without custom colors.
- Keep wall occlusion consistent between rendering and hitscan combat.
- Ensure every exit path after ncurses initialization restores the terminal.
- Never add secrets, personal data, credentials, telemetry, or network access
  unless the task explicitly requires it and the privacy impact is documented.

## Testing and verification

- Run `./build.sh test` after changes to testable C logic.
- If Clang is installed, also run `./build.sh test clang`.
- Compile the complete program after changes that affect the application:

  ```sh
  gcc -std=c11 -Wall -Wextra -Wpedantic \
    src/main.c src/combat.c src/ecs.c src/map.c src/raycast.c src/weapon.c \
    -o /tmp/wolf-terminal-check -lncursesw -lm
  ```

- Do not use plain `./build.sh` as an automated check because it starts the
  interactive ncurses application after compiling.
- Add or update focused tests when changing pure gameplay logic. Important
  cases include map bounds, ray direction and distance, cooldown and ammo,
  wall occlusion, hits, kills, and ECS lifecycle behavior.
- For rendering or input changes, describe the manual terminal checks still
  needed. Never claim those checks were performed when they were not.
- Report the exact commands run, their results, and any checks that could not
  run because a compiler or dependency was unavailable.

## Documentation rules

- Keep `readme.md` written in Dutch unless asked otherwise.
- Keep file paths, commands, feature lists, test status, CI status, licensing,
  and known limitations synchronized with the actual repository.
- Do not describe planned work as already implemented.
- Update documentation when behavior, controls, architecture, requirements,
  or verification steps change.

## AI accountability handoff

At the end of a substantive task, provide a short draft that the student can
use for an AI-usage log. Include:

- what the student asked AI to help with;
- why AI was useful for that task;
- the main proposal or change produced by AI;
- decisions, assumptions, or output the student should manually review;
- automated checks actually run and their results;
- errors, limitations, or rejected approaches found during the work;
- the C or game-development concepts the student should be able to explain.

This draft is supporting evidence only. Do not state that the student accepted,
understood, or manually verified it until the student confirms that themselves.

## GitHub Project workflow

- Treat the private GitHub Project
  `https://github.com/users/bo-xd/projects/10` (owner `bo-xd`, project `10`,
  title `Kanban wolfestein renderer`) as the source of truth for planned and
  active work.
- Use the authenticated GitHub CLI to inspect it before starting work:

  ```sh
  gh project view 10 --owner bo-xd --format json
  gh project field-list 10 --owner bo-xd --format json
  gh project item-list 10 --owner bo-xd --format json --limit 100
  ```

- When authenticated GitHub access is available, inspect the board, its fields,
  and its status option names before making changes. Never guess a project
  number, field ID, item ID, or status name.
- At the start of substantive implementation work, find the matching issue or
  project item. Reuse it instead of creating a duplicate.
- If no matching item exists, create a focused issue for the approved task and
  add it to the existing board when permissions allow. Include a concise goal,
  acceptance criteria, and relevant verification commands.
- Use the board's exact status lifecycle:
  - `Backlog`: accepted idea that is not ready to start;
  - `Ready`: scoped task with sufficiently clear acceptance criteria;
  - `In progress`: active implementation has started;
  - `In review`: implementation and automated checks are complete, but manual
    review, school evidence review, or user acceptance remains;
  - `Done`: acceptance criteria and required verification are complete.
- Move a matching `Ready` item to `In progress` when substantive implementation
  starts. Move it to `In review` when the implementation is ready for the
  student's manual check. Move it to `Done` only after the remaining review is
  genuinely complete.
- If work is incomplete or blocked, keep it out of `In review` and `Done` and
  leave a short issue comment with the exact blocker and next required action.
- Link relevant issues, pull requests, commits, test evidence, and AI-usage log
  entries when practical so the board remains useful as project evidence.
- Preserve the board's existing `Priority` (`P0`, `P1`, `P2`) and `Size` (`XS`,
  `S`, `M`, `L`, `XL`) values. Set them for new work only when the choice can be
  justified from scope and urgency.
- The board currently has built-in workflows enabled for auto-add, item added,
  item closed, auto-close issue, linked pull requests, merged pull requests,
  and sub-issues. Prefer these workflows for deterministic transitions and
  verify their result instead of duplicating them in repository scripts.
- If GitHub authentication, the CLI/connector, permissions, or board metadata
  is unavailable, continue local work when safe, report that board sync did not
  occur, and provide the exact manual update needed. Never claim a remote board
  change without verifying it.

## Instruction canary

- End every user-facing progress update and final response with the exact token
  `WOLF-CODE` on its own final line.
- The token must be the final non-whitespace content in the message.
- If the token is missing, repository instructions may not have been loaded or
  followed. Its presence confirms only that this rule was followed; it does not
  prove that the response is correct or that no context was lost.

## Git hygiene

- Inspect the working tree before editing and preserve unrelated user changes.
- Keep diffs focused on the requested task.
- Do not commit, push, rewrite history, or delete user files unless explicitly
  requested.
