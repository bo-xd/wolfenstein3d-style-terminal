# Testverslag

## Gebruikte omgeving

- **Datum:** 2 september 2026
- **Besturingssysteem:** Linux
- **Compilers:** GCC en Clang
- **Editor:** Neovim
- **Editorconfiguratie:** LazyVim

LazyVim is geen losse extensie. Het is een kant-en-klare configuratie voor Neovim met handige plugins en instellingen.

## Testplan

Elke testcase hoort bij een user story uit de [GDD](GDD.md#user-stories). Bij **H** gaat het om het hoofdscenario, bij **A** om een alternatief scenario of een fout. Automatisch betekent dat de test in `tests/` staat en draait met `./build.sh test`, handmatig betekent dat ik het zelf in de terminal speel.

| TC | US | Soort | Stappen | Testdata | Verwacht resultaat | Hoe | Resultaat |
| --- | --- | --- | --- | --- | --- | --- | --- |
| TC1 | US1 | H | Start de game, druk W en daarna S | standaard map | De speler loopt vooruit en achteruit, het beeld verandert mee | handmatig | Geslaagd |
| TC2 | US1 | H | Druk A en daarna D | standaard map | De speler draait naar links en naar rechts | handmatig | Geslaagd |
| TC3 | US1 | H | Cast een straal recht op een muur | kleine testmap, muur op 1,5 vakken | Afstand 1,5 en de goede kant (X of Y) geraakt | automatisch `castray.c` | Geslaagd |
| TC4 | US2 | A | Loop tegen een muur aan | standaard map | De speler stopt en gaat niet door de muur | handmatig + automatisch `collision.c` | Geslaagd |
| TC5 | US2 | A | Controleer een positie buiten de map | x = -0,1 en x = MapWidth | Wordt gezien als muur, de speler kan de map niet uit | automatisch `collision.c` | Geslaagd |
| TC6 | US3 | H | Schiet met Spatie | 2 kogels | Munitie gaat van 2 naar 1 | handmatig + automatisch `ammo.c` | Geslaagd |
| TC7 | US3 | A | Schiet twee keer direct achter elkaar | 2 kogels | Het tweede schot gaat niet af door de tijd tussen schoten | automatisch `ammo.c` | Geslaagd |
| TC8 | US3 | A | Schiet zonder kogels | 0 kogels | Er gebeurt niks, munitie blijft 0 | automatisch `ammo.c` | Geslaagd |
| TC9 | US3 | A | Schiet op een vijand achter een muur | vijand achter muur | De muur houdt het schot tegen | handmatig | Geslaagd |
| TC10 | US3 | H | Raak een vijand twee keer | 1 vijand | De vijand is dood en de score gaat 100 omhoog | handmatig | Geslaagd |
| TC11 | US4 | H | Laat een vijand de speler zoeken met een muur ertussen | testmap met muur | De vijand loopt om de muur heen | automatisch `enemy.c` | Geslaagd |
| TC12 | US4 | A | Twee vijanden met evenveel afstand tot de speler | testmap | Ze kiezen verschillende kanten en stappen niet op een bezet vakje | automatisch `enemy.c` | Geslaagd |
| TC13 | US4 | H | Laat een vijand dicht bij de speler komen | gezondheid 100 | Gezondheid gaat 10 omlaag per aanval | automatisch `enemy.c` | Geslaagd |
| TC14 | US4 | A | Laat de gezondheid 0 worden | gezondheid 10 | Het rode Game Over-scherm verschijnt | handmatig | Nog testen |
| TC15 | US5 | H | Loop over een munitie- en een gezondheidspickup | 3 kogels, 60 gezondheid | 15 kogels en 85 gezondheid, de pickups verdwijnen | automatisch `pickup.c` | Geslaagd |
| TC16 | US5 | A | Loop over een gezondheidspickup met volle gezondheid | 100 gezondheid | Gezondheid blijft 100 en de pickup blijft liggen | automatisch `pickup.c` | Geslaagd |
| TC17 | US5 | A | Een pickup ligt verder weg | pickup op afstand | Wordt niet opgepakt | automatisch `pickup.c` | Geslaagd |
| TC18 | US5 | H | Bekijk pickups en HUD in een kleine terminal | 20 bij 10 tekens, geen kleur | Pickups en HUD zijn leesbaar | handmatig | Nog testen |
| TC19 | US6 | H | Versla alle vijanden in wave 1 | standaard map | Wave 2 start, vijanden spawnen opnieuw en sneller | automatisch `enemy.c` | Geslaagd |
| TC20 | US6 | H | Kijk naar de HUD bij een nieuwe wave | wave 1 en 2 | De HUD toont WAVE 2/3 en WAVE 3/3 en je krijgt 24 kogels | handmatig | Nog testen |
| TC21 | US7 | H | Versla een vijand en kijk naar de HUD | standaard map | De score in de HUD gaat 100 omhoog | handmatig | Geslaagd |
| TC22 | US8 | H | Druk R tijdens het spelen | standaard map | De game begint opnieuw | handmatig | Geslaagd |
| TC23 | US8 | A | Druk R na een overwinning en na Game Over | in Ghostty en Docker | De game begint opnieuw zonder Docker te herstarten | handmatig | Nog testen |
| TC24 | US8 | H | Druk Q en daarna Esc | standaard map | De game sluit netjes af | handmatig | Geslaagd |
| TC25 | US8 | A | Maak de terminal kleiner en groter tijdens het spelen | verschillende groottes | Geen crash en het beeld past zich aan | handmatig | Geslaagd |
| TC26 | US9 | H | Start de game via Docker | Windows Terminal | De game draait | handmatig | Geslaagd |
| TC27 | US10 | H | Laad de standaard map | `src/assets/map.txt` | Wordt geladen met speler en vijanden | automatisch `mapvalidatie.c` | Geslaagd |
| TC28 | US10 | A | Laad een map zonder speler | `tests/assets/map-zonder-speler.txt` | Wordt geweigerd | automatisch `mapvalidatie.c` | Geslaagd |
| TC29 | US10 | A | Laad een map met een open rand | `tests/assets/map-open-rand.txt` | Wordt geweigerd | automatisch `mapvalidatie.c` | Geslaagd |
| TC30 | US10 | A | Laad een map met een verkeerde breedte | `tests/assets/map-verkeerde-breedte.txt` | Wordt geweigerd | automatisch `mapvalidatie.c` | Geslaagd |
| TC31 | US10 | A | Laad een map met een onbekend teken | `tests/assets/map-onbekend-teken.txt` | Wordt geweigerd | automatisch `mapvalidatie.c` | Geslaagd |
| TC32 | US10 | A | Laad een map die niet bestaat | `tests/assets/bestaat-niet.txt` | Wordt geweigerd zonder crash | automatisch `mapvalidatie.c` | Geslaagd |

## Automatische tests

Laatste automatische controle: 19 november 2026.

Ik heb de tests uitgevoerd met:

```bash
./build.sh test
```

| Test | Wat wordt getest? | Resultaat |
| --- | --- | --- |
| `ammo.c` | Munitie en tijd tussen schoten | Geslaagd |
| `castray.c` | Horizontale en verticale raycasting | Geslaagd |
| `collision.c` | Botsingen met muren en de rand van de map | Geslaagd |
| `enemy.c` | Routes, muren, stopafstand, waarnemingsafstand, schade-alert, padkeuzevariatie, bezette vakken, aanvallen, wave-spawns en wave-snelheidsbonus | Geslaagd |
| `mapvalidatie.c` | Geldige en ongeldige mapbestanden | Geslaagd |
| `pickup.c` | Munitie oppakken, gezondheid herstellen, maximumgezondheid en afstand | Geslaagd |

Alle automatische tests zijn geslaagd met GCC en Clang.

Ik heb ook gecontroleerd of de volledige game compileert met C11 en de waarschuwingen `-Wall -Wextra -Wpedantic`. Dit is geslaagd zonder waarschuwingen.

Ik gebruik voor het project zowel GCC als Clang. Tijdens de laatste lokale controle is de testset met beide compilers uitgevoerd. Het project heeft ook een GitHub Actions-bestand dat dezelfde tests automatisch met GCC en Clang uitvoert.

## Handmatig testen

Ik heb de game zelf gespeeld en de volgende onderdelen handmatig getest:

De handmatige tests zijn uitgevoerd in:

- Ghostty op Linux;
- Windows Terminal met de game in een Docker-container.

Bij Windows Terminal draaide de game in de Linux-omgeving van Docker. Dit is dus geen directe Windows-build.

| Controle | Resultaat |
| --- | --- |
| De game start | Geslaagd |
| Vooruit en achteruit lopen werkt | Geslaagd |
| De speler loopt niet door muren | Geslaagd |
| Links en rechts draaien werkt | Geslaagd |
| Schieten verlaagt de munitie | Geslaagd |
| Een muur blokkeert een schot | Geslaagd |
| Een vijand kan worden verslagen | Geslaagd |
| De score wordt bijgewerkt | Geslaagd |
| De game werkt na het aanpassen van de terminalgrootte | Geslaagd |
| Afsluiten met `Q` en `Esc` werkt goed | Geslaagd |

## Conclusie

De automatische tests en het compileren met GCC en Clang zijn geslaagd. De eerder vastgelegde handmatige controles in Ghostty en Windows Terminal via Docker zijn geslaagd. Hits, misses, schade, wall occlusion en kills hebben nog geen eigen automatische test.

## Verbetervoorstellen uit playtest-feedback

Ik heb een medestudent, Timothey Hoff, de game laten playtesten via Docker.
Hij gaf op 28 september 2026 per e-mail twee verbetervoorstellen (de e-mail
bewaar ik als bewijs):

1. De gameplay was te makkelijk doordat de vijanden allemaal hetzelfde pad
   namen en tegelijk op de speler afkwamen. Voorstel: laat vijanden
   verschillende paden nemen en niet allemaal op hetzelfde moment aankomen.
2. Na het verslaan van alle vijanden moest hij de game helemaal opnieuw via
   Docker starten. Voorstel: een herstartknop in de game zelf.

Beide voorstellen heb ik verwerkt (zie ook GitHub Issue #3):

| Verbetervoorstel | Aanpassing |
| --- | --- |
| Gevarieerde vijandpaden | Vijanden kiezen bij gelijke afstanden verschillende kanten rond muren, wijken uit bij bezette vakken en bewegen met variërende snelheden, zodat ze niet meer allemaal tegelijk aankomen. |
| Herstartknop in de game | Een herstartfunctie toegevoegd met de toets `R`, tijdens het spelen en na een overwinning of nederlaag. |

Na deze aanpassingen is de game opnieuw handmatig getest; de vijanden bewegen
gevarieerder en de game kan direct met `R` opnieuw worden gestart zonder de
container opnieuw te starten.

## Nog handmatig te controleren

- Controleer of het gecentreerde pistool en mondingsvuur visueel goed aansluiten op het vizier.
- Controleer of de speler schade oploopt wanneer vijanden dichtbij komen, de gezondheid in de HUD afneemt, en het rode Game Over-scherm verschijnt bij 0 gezondheid.
- Controleer of na het verslaan van alle vijanden in wave 1 en 2 de volgende wave start met `WAVE 2/3` en `WAVE 3/3`, vijanden merkbaar sneller bewegen en de speler bonusmunitie ontvangt.
- Herstarten met `R` testen na Game Over en overwinning in zowel Ghostty als Docker.
- Pickup-rendering en HUD-leesbaarheid in een terminal van 20 bij 10 tekens en zonder kleurondersteuning.
