# De game starten

Je kunt de game op twee manieren starten.

## Keuze 1: met GCC of Clang

Deze manier is bedoeld voor Linux. Je hebt nodig:

- GCC of Clang;
- ncursesw;
- een terminal.

Op Arch Linux kun je alles installeren met:

```bash
sudo pacman -S gcc clang ncurses
```

Start de game met GCC:

```bash
chmod +x build.sh
./build.sh
```

Start de game met Clang:

```bash
chmod +x build.sh
./build.sh clang
```

De tests starten met:

```bash
./build.sh test
./build.sh test clang
```

Je hebt niet per se GCC én Clang nodig. Eén compiler is genoeg om de game te bouwen. Beide gebruiken is wel handig om te controleren of de code bij allebei werkt.

## Keuze 2: alleen Docker

Als je Docker gebruikt, hoef je GCC, Clang en ncurses niet zelf te installeren. Je hebt dan alleen Docker nodig.

Bouw en start de game met:

```bash
docker build -t wolf-terminal .
docker run --rm -it wolf-terminal
```

Dit werkt bijvoorbeeld in Ghostty op Linux en in Windows Terminal met Docker Desktop. Op Windows draait de game dan in een Linux-container. Het is dus geen directe Windows-versie.

## Besturing

- `W` of pijltje omhoog: vooruit
- `S` of pijltje omlaag: achteruit
- `A` of pijltje links: links draaien
- `D` of pijltje rechts: rechts draaien
- `Spatie` of `F`: schieten
- `Q` of `Esc`: afsluiten

## Kort gezegd

- Wil je de code zelf bouwen en testen? Gebruik GCC of Clang met ncursesw.
- Wil je alleen de game starten? Dan is alleen Docker genoeg.
