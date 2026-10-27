# Testverslag

## Gebruikte omgeving

- **Datum:** 2 september 2026
- **Besturingssysteem:** Linux
- **Compilers:** GCC en Clang
- **Editor:** Neovim
- **Editorconfiguratie:** LazyVim

LazyVim is geen losse extensie. Het is een kant-en-klare configuratie voor Neovim met handige plugins en instellingen.

## Automatische tests

Laatste automatische controle: 28 september 2026.

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
