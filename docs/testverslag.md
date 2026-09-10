# Testverslag

## Gebruikte omgeving

- **Datum:** 2 september 2026
- **Besturingssysteem:** Linux
- **Compilers:** GCC en Clang
- **Editor:** Neovim
- **Editorconfiguratie:** LazyVim

LazyVim is geen losse extensie. Het is een kant-en-klare configuratie voor Neovim met handige plugins en instellingen.

## Automatische tests

Laatste automatische controle: 10 september 2026.

Ik heb de tests uitgevoerd met:

```bash
./build.sh test
```

| Test | Wat wordt getest? | Resultaat |
| --- | --- | --- |
| `ammo.c` | Munitie en tijd tussen schoten | Geslaagd |
| `castray.c` | Horizontale en verticale raycasting | Geslaagd |
| `collision.c` | Botsingen met muren en de rand van de map | Geslaagd |
| `enemy.c` | Routes, muren, onbereikbare vijanden en stopafstand | Geslaagd |
| `mapvalidatie.c` | Geldige en ongeldige mapbestanden | Geslaagd |

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
