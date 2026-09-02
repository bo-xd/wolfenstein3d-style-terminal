# Wolfenstein 3D-stijl terminalgame

Dit is mijn simpele 3D-shooter in de terminal. De game is gemaakt in C en gebruikt ncursesw voor het beeld en de toetsen.

De game gebruikt geen echte 3D-modellen. Met raycasting en ASCII-tekens wordt een 3D-effect gemaakt.

> Dit is geen officieel Wolfenstein-spel. Ik gebruik geen originele code, levels, afbeeldingen of geluiden uit Wolfenstein.

## Wat zit er in?

- Een 3D-beeld met raycasting.
- Lopen en draaien.
- Muren waar je niet doorheen kunt lopen.
- Vijanden waarop je kunt schieten.
- Munitie, schade, score en een hitmarker.
- Een ASCII-wapen met een simpele schietanimatie.
- Een HUD met munitie, score en het aantal vijanden.
- Kleuren en donkere muren op afstand.
- Automatische tests in de map `tests/`.

## Wat heb je nodig?

- Linux of een andere POSIX-omgeving.
- Een C-compiler, GCC of Clang.
- De ontwikkelbibliotheek van ncursesw.
- Een terminal van minimaal 20 bij 10 tekens.

Op Arch Linux installeer je GCC, Clang en ncurses met:

```bash
sudo pacman -S gcc clang ncurses
```

## Starten

Ga in de hoofdmap van het project staan en voer dit uit:

```bash
chmod +x build.sh
./build.sh
```

Het script bouwt de game en start hem daarna meteen.

## Besturing

| Actie | Toets |
| --- | --- |
| Vooruit | `W` of pijltje omhoog |
| Achteruit | `S` of pijltje omlaag |
| Links draaien | `A` of pijltje links |
| Rechts draaien | `D` of pijltje rechts |
| Schieten | `Spatie` of `F` |
| Afsluiten | `Q` of `Esc` |

## Tests

De automatische tests staan al in de map `tests/`. Ze testen:

- munitie en de tijd tussen schoten;
- raycasting;
- botsingen met muren en de rand van de map;
- geldige en ongeldige mapbestanden.

Start alle tests met GCC:

```bash
./build.sh test
```

Testen met Clang kan ook als Clang is geïnstalleerd:

```bash
./build.sh test clang
```

GitHub Actions voert de tests automatisch uit met GCC en Clang. Er is ook een controle met `clang-tidy`.

De game is handmatig getest in Ghostty op Linux en in Windows Terminal via Docker. Bij Windows Terminal draait de game in een Linux-container.

## Docker

Je kunt de game ook met Docker starten:

```bash
docker build -t wolf-terminal .
docker run --rm -it wolf-terminal
```

## De map

De map staat in `src/assets/map.txt` en is 16 bij 16 tekens groot.

| Teken | Betekenis |
| --- | --- |
| `#` | Muur |
| `.` | Lege ruimte |
| `P` | Startplek van de speler |
| `E` | Startplek van een vijand |

De buitenkant van de map moet helemaal uit muren bestaan. Er moet precies één speler in de map staan.

## Belangrijkste bestanden

- `src/main.c`: start de game en tekent het beeld.
- `src/player.c`: regelt lopen en draaien.
- `src/map.c`: laadt de map en controleert muren.
- `src/raycast.c`: maakt het 3D-effect.
- `src/ecs.c`: bewaart de speler en vijanden.
- `src/combat.c`: regelt schieten, schade en punten.
- `src/weapon.c`: tekent het wapen.
- `src/assets/map.txt`: bevat het level.
- `tests/`: bevat de automatische tests.
- `docs/GDD.md`: bevat het korte ontwerp van de game.
- `docs/wat-ontbreekt.md`: bevat de taken die nog gedaan kunnen worden.

## Wat zit er nog niet in?

- Er is maar één level.
- Vijanden lopen niet en schieten niet terug.
- Er zijn geen deuren of voorwerpen om op te pakken.
- Er is geen geluid of muziek.
- Er is geen win- of verliesscherm.
- Je kunt de toetsen en kleuren niet aanpassen.

## Waarom C en ncursesw?

Met C kan ik zelf de berekeningen, game-loop en opslag van gegevens maken. ncursesw regelt de invoer, kleuren en tekst in de terminal. De docent heeft toestemming gegeven om deze game zonder bestaande game-engine te maken.

Voor het schrijven van de code heb ik Neovim gebruikt met LazyVim. LazyVim is een kant-en-klare configuratie voor Neovim met plugins en instellingen. Ik gebruik GCC en Clang om de code te compileren en te testen.

## Documentatie

- [Game Design Document](docs/GDD.md)
- [Testverslag](docs/testverslag.md)
- [Handleiding om de game te starten](docs/handleiding.md)
- [Wat ontbreekt er nog?](docs/wat-ontbreekt.md)

## Licentie

De code valt onder de GNU General Public License v3. Zie het bestand `LICENSE`.
