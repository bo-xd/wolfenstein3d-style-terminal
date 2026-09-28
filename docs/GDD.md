# GDD van mijn terminalgame

## Het idee

Mijn game is een simpele shooter die in de terminal draait. De speler loopt door een doolhof en zoekt vijanden. Het doel is om alle vijanden neer te schieten en punten te krijgen.

De game lijkt een beetje op oude 3D-games. Alles wordt gemaakt met letters en tekens in plaats van normale afbeeldingen.

De game is vooral bedoeld voor mensen die van oude games houden of willen zien hoe een simpele 3D-game werkt.

## Hoe de game werkt

De speler kan lopen, draaien en schieten. Je begint met 100 gezondheid en 48 kogels. Een vijand is dood na twee keer raken. Voor elke verslagen vijand krijg je 100 punten. Vijanden zoeken via de vrije vakken van de map een korte route naar de speler.

In de map liggen munitie- en gezondheidspickups. De speler pakt ze automatisch op door over hetzelfde kaartvak te lopen. Een munitiepickup geeft 12 kogels. Een gezondheidspickup herstelt 25 gezondheid, maar nooit tot boven 100. Een gezondheidspickup blijft liggen wanneer de speler al de maximale gezondheid heeft.

Muren houden de speler en vijanden tegen. Je kunt ook niet door een muur heen schieten. Als alle vijanden weg zijn, verschijnt het overwinningsscherm.

## Besturing

- `W` of pijltje omhoog: vooruit lopen
- `S` of pijltje omlaag: achteruit lopen
- `A` of pijltje links: links draaien
- `D` of pijltje rechts: rechts draaien
- `Spatie` of `F`: schieten
- `Q` of `Esc`: afsluiten

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

## Testen

Er zijn automatische tests voor munitie, pickups, raycasting, botsingen met muren en het laden van de map. Deze tests start ik met:

```bash
./build.sh test
```

Ik moet de game ook zelf testen. Ik controleer dan het lopen, draaien, schieten, verslaan van vijanden en het afsluiten van de game.

## Wat er nog niet in zit

- Er is maar één level.
- Vijanden schieten niet terug.
- Er zijn geen deuren.
- Vijanden brengen nog geen schade toe, waardoor de gezondheidspickup tijdens normaal spelen nog niet nodig is.
- Er is geen geluid.
- Er is geen verliesscherm.
- Je kunt de toetsen en kleuren niet aanpassen.
