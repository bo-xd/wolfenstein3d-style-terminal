# GDD van mijn terminalgame

## Het idee

Mijn game is een simpele shooter die in de terminal draait. De speler loopt door een doolhof en zoekt vijanden. Het doel is om alle vijanden neer te schieten en punten te krijgen.

De game lijkt een beetje op oude 3D-games. Alles wordt gemaakt met letters en tekens in plaats van normale afbeeldingen.

De game is vooral bedoeld voor mensen die van oude games houden of willen zien hoe een simpele 3D-game werkt.

## Hoe de game werkt

De speler kan lopen, draaien en schieten. Je begint met 100 gezondheid en 48 kogels. Een vijand is dood na twee keer raken. Voor elke verslagen vijand krijg je 100 punten. Vijanden activeren wanneer de speler binnen bereik komt of wanneer ze geraakt worden. Ze zoeken via de vrije vakken een route naar de speler, kiezen bij gelijke afstanden verschillende kanten rond muren, wijken uit bij bezette vakken en bewegen met variërende snelheden zodat ze niet allemaal tegelijk aankomen. Van dichtbij brengen vijanden 10 schade per aanval toe. Wanneer de gezondheid 0 bereikt, verlies je en verschijnt het Game Over-scherm.

Het spel bevat een wave-systeem van 3 rondes. Wanneer alle vijanden in een ronde zijn verslagen, start automatisch de volgende wave. Bij elke nieuwe wave spawnen de vijanden opnieuw, bewegen ze merkbaar sneller en ontvangt de speler 24 bonuskogels. De HUD toont de huidige ronde (bijvoorbeeld WAVE 1/3). Na het voltooien van Wave 3 behaal je de overwinning.

In de map liggen munitie- en gezondheidspickups. De speler pakt ze automatisch op door over hetzelfde kaartvak te lopen. Een munitiepickup geeft 12 kogels. Een gezondheidspickup herstelt 25 gezondheid, maar nooit tot boven 100. Gezondheidspickups zijn essentieel om vijandelijke aanvallen te overleven. Een gezondheidspickup blijft liggen wanneer de speler al de maximale gezondheid heeft.

Muren houden de speler en vijanden tegen. Je kunt ook niet door een muur heen schieten. Zowel na een overwinning als na een nederlaag (en tijdens de game) kan het spel direct opnieuw worden gestart met `R`.

## Besturing

- `W` of pijltje omhoog: vooruit lopen
- `S` of pijltje omlaag: achteruit lopen
- `A` of pijltje links: links draaien
- `D` of pijltje rechts: rechts draaien
- `Spatie` of `F`: schieten
- `R`: de game herstarten
- `Q` of `Esc`: afsluiten

## User stories

Elke user story heeft acceptatiecriteria. Een story is klaar als alle criteria kloppen en de testcases uit het [testverslag](testverslag.md#testplan) slagen.

| # | User story | Acceptatiecriteria | Prioriteit | Status |
| --- | --- | --- | --- | --- |
| US1 | Als speler wil ik door een 3D-doolhof kunnen lopen en draaien, zodat ik het level kan verkennen. | Lopen met W/S of pijltjes, draaien met A/D of pijltjes, muren worden in 3D getekend en verder weg donkerder. | P0 | Klaar |
| US2 | Als speler wil ik niet door muren kunnen lopen, zodat het level klopt. | De speler stopt bij een muur en bij de rand van de map. | P0 | Klaar |
| US3 | Als speler wil ik op vijanden kunnen schieten, zodat ik ze kan verslaan. | Schieten met Spatie of F kost 1 kogel, zonder kogels kan je niet schieten, een muur houdt een schot tegen, een vijand is dood na 2 keer raken en geeft 100 punten. | P0 | Klaar |
| US4 | Als speler wil ik dat vijanden mij zoeken en aanvallen, zodat de game een uitdaging is. | Vijanden lopen om muren heen naar de speler, nemen verschillende paden, komen niet allemaal tegelijk aan en doen 10 schade van dichtbij. | P1 | Klaar |
| US5 | Als speler wil ik munitie en gezondheid kunnen oppakken, zodat ik langer kan overleven. | Over een pickup lopen geeft 12 kogels of 25 gezondheid, gezondheid gaat nooit boven 100, een gezondheidspickup blijft liggen als je al 100 hebt. | P2 | Klaar, HUD nog handmatig controleren |
| US6 | Als speler wil ik meerdere rondes spelen die moeilijker worden, zodat de game niet te kort is. | 3 waves, elke wave snellere vijanden en 24 bonuskogels, de HUD toont WAVE 1/3, na wave 3 win je. | P1 | Klaar |
| US7 | Als speler wil ik in één oogopslag zien hoe ik ervoor sta, zodat ik weet wanneer ik moet oppassen. | De HUD toont gezondheid, kogels, score, wave en hoeveel vijanden nog leven. | P1 | Klaar |
| US8 | Als speler wil ik een overwinnings- en Game Over-scherm zien en opnieuw kunnen starten, zodat ik niet de hele game opnieuw hoef op te starten. | VICTORY-scherm na wave 3, Game Over bij 0 gezondheid, herstarten met R tijdens en na het spelen, afsluiten met Q of Esc. | P1 | Klaar |
| US9 | Als speler wil ik de game op elke computer kunnen spelen, zodat ik niet per se Linux nodig heb. | De game draait via Docker, ook op Windows en macOS. | P1 | Klaar |
| US10 | Als ontwikkelaar wil ik het level in een tekstbestand kunnen aanpassen, zodat ik geen code hoef te veranderen. | De map wordt uit `src/assets/map.txt` gelezen, een map zonder speler, met een open rand, een verkeerde breedte of een onbekend teken wordt geweigerd. | P0 | Klaar |
| US11 | Als speler wil ik deuren kunnen openen, zodat het level uit kamers bestaat. | Zie de kaart "Deuren toevoegen" op het kanban-bord. | P2 | Backlog |

## Hoe het eruitziet

De muren, vloer, vijanden en het wapen bestaan uit ASCII-tekens. Muren die verder weg zijn worden donkerder. Hierdoor lijkt de wereld 3D.

Boven in beeld staat hoeveel gezondheid, kogels en punten je hebt. Daar staat ook hoeveel vijanden nog leven. De game heeft geen geluid of muziek.

## Techniek

Ik heb de game gemaakt in C11. Ik heb hiervoor Neovim gebruikt met de LazyVim-configuratie. Ik gebruik GCC en Clang om de code te compileren en te testen. Voor het terminalvenster en de toetsen gebruik ik ncursesw. Voor het 3D-effect gebruik ik raycasting. De docent heeft toestemming gegeven om de game zonder bestaande game-engine te maken.

De code is verdeeld over meerdere bestanden:

- `main.c` regelt de game en het beeld.
- `player.c` regelt lopen en draaien.
- `map.c` laadt de map en houdt de speler tegen bij muren.
- `raycast.c` berekent welke muren je ziet.
- `ecs.c` bewaart de speler en vijanden.
- `enemy.c` berekent routes en regelt beweging en rendering van vijanden.
- `pickup.c` regelt het oppakken en renderen van voorwerpen.
- `combat.c` regelt schieten, schade en punten.
- `weapon.c` tekent het wapen.

De map staat in `src/assets/map.txt`.

## Code conventions

De opmaak staat vast in `.clang-format` (LLVM-stijl) en de naamgeving in `.clang-tidy`. GitHub Actions controleert bij elke push of de code zich hieraan houdt.

Hoe je clang-tidy instelt met een `.clang-tidy`-bestand heb ik opgezocht op https://sarcasm.github.io/notes/dev/clang-tidy.html.

| Onderdeel | Regel | Voorbeeld |
| --- | --- | --- |
| Functies | CamelCase met een hoofdletter | `CastRay`, `EnemyMovementSystem` |
| Systems in de ECS | eindigen op `System` | `PickupSystem`, `RenderVictorySystem` |
| Structs, enums en typedefs | CamelCase met een hoofdletter | `Vec2`, `RayHit`, `ShotResult` |
| Enum-waarden | CamelCase, beginnen met de naam van de enum | `RaySideX`, `ShotKill` |
| Variabelen, parameters en struct-velden | camelCase met een kleine letter | `playerTileX`, `distanceToPlayer` |
| Constanten | `static const` in camelCase | `enemyBaseMoveSpeed` |
| Macros | CamelCase | `EcsMaxEntities`, `EntityNone` |
| Bestanden | kleine letters, één module per bestand met een eigen `.h` | `enemy.c`, `enemy.h`, `game_config.h` |
| Opmaak | 2 spaties inspringen, maximaal 80 tekens per regel, `{` op dezelfde regel | |
| Commentaar | alleen uitleggen waarom iets zo werkt, niet wat er staat | `// Breadth-first search stores how many open tiles lead from each tile to the player.` |
| Geheugen | geen `malloc`, entities staan in vaste arrays van `EcsMaxEntities` | `world->position[entity]` |

Controleren kan lokaal met:

```bash
clang-format --dry-run --Werror src/*.c src/*.h tests/*.c
clang-tidy src/*.c -- -std=c11 -Isrc
```

## Testen

Er zijn automatische tests voor munitie, pickups, raycasting, botsingen met muren en het laden van de map. Deze tests start ik met:

```bash
./build.sh test
```

Ik moet de game ook zelf testen. Ik controleer dan het lopen, draaien, schieten, verslaan van vijanden en het afsluiten van de game.

## Wat er nog niet in zit

- Er is maar één level.
- Vijanden schieten niet terug (ze vallen alleen van dichtbij aan).
- Er zijn geen deuren.
- Er is geen geluid.
- Je kunt de toetsen en kleuren niet aanpassen.
