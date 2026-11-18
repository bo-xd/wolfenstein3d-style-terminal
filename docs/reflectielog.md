# Reflectielog

Per periode schrijf ik kort op wat ik heb gedaan, wat goed ging, waar ik tegenaan liep en wat ik de volgende keer anders doe. De taken staan op het [kanban-bord](https://github.com/users/bo-xd/projects/10), de tijdsinschatting staat in het veld Estimate en de echte tijd in Werkelijke tijd (min). Een S is ongeveer 15 minuten, een M 30, een L 45 en een XL een uur of meer.

## 25 t/m 27 augustus

**Gedaan:** raycasting met shading, wapen en vijanden, de array-based ECS, unit tests met gcc en Clang, de Dockerfile, GitHub Actions met tests en clang-tidy en de GPL v3 licentie.

**Ging goed:** door CEngine wist ik al hoe een ECS werkt, dus dat ging snel.

**Liep tegenaan:** de bestandsindeling was rommelig, daarom heb ik de bestanden opnieuw ingedeeld.

**Volgende keer:** eerst nadenken over de mappenstructuur voordat ik begin.

## 29 en 30 augustus

**Gedaan:** de player code naar player.c verplaatst.

**Liep tegenaan:** lokaal werkte alles, maar de Docker-build faalde in de CI omdat player.c niet in de gcc-regel van de Dockerfile stond.

**Ging goed:** de CI heeft de fout meteen gevonden, daarom vind ik het automatisch checken echt nuttig.

**Volgende keer:** de buildconfiguratie gelijk aanpassen als ik een bestand toevoeg.

## 10 september

**Gedaan:** bewegende vijanden met pathfinding (breadth-first search) en game states, waaronder het overwinningsscherm.

**Tijd:** de vijanden had ik ingeschat op L (45 minuten), het werd bijna een uur. Het overwinningsscherm had ik op S (15 minuten) gezet en dat duurde ongeveer 20 minuten.

**Volgende keer:** taken met pathfinding of andere nieuwe dingen groter inschatten.

## 25 t/m 28 september

**Gedaan:** INFINITY vervangen door HUGE_VAL, munitie- en gezondheidspickups, de vijandcode naar een eigen module (issue #1) en de naamgeving verbeterd (issue #2).

**Feedback:** Timothey Hoff heeft de game via Docker gespeeld. Hij vond de game te makkelijk omdat alle vijanden hetzelfde pad namen, en na het winnen moest hij Docker opnieuw starten.

**Volgende keer:** eerder iemand anders laten testen, dan komt de feedback niet pas aan het eind.

## 12 t/m 15 oktober

**Gedaan:** de feedback van Timothey verwerkt (issue #3), vijanden kiezen nu verschillende paden en je kan herstarten met R. Ook het wapen verbeterd en een verliesscherm toegevoegd (issue #4) en een wave-systeem met 3 rondes gemaakt (issue #5).

**Gecontroleerd:** de vijandtest uitgebreid met wave-spawns en de snelheidsbonus, daarna opnieuw gespeeld en de vijanden komen nu verspreid aan.

## 26 oktober t/m 10 november

**Gedaan:** de code geformatteerd, de website op GitHub Pages gemaakt en de ASCII-art naar een eigen bestand verplaatst.

## 17 en 18 november

**Feedback:** in [issue #6](https://github.com/bo-xd/wolfenstein3d-style-terminal/issues/6) kreeg ik feedback dat lopen afhangt van de key repeat van het besturingssysteem, omdat ncurses geen verschil ziet tussen keydown en keyup. De tip was om SDL te gebruiken. De Docker-installatie werkte meteen op een MacBook.

**Feedback:** mijn coach heeft mijn kanban-bord bekeken en vond de indeling, de prioriteiten en de groottes goed, maar ik had te veel kaarten in de Backlog, geen tijdsinschattingen ingevuld, In review niet gebruikt en bij afgeronde taken niet laten zien wat het resultaat was.

**Wat ik ermee heb gedaan:**
- De Backlog teruggebracht naar 5 kaarten. Ik heb de kaarten gehouden die nodig zijn voor het examen (UML, cloudkeuzes, programmeertechnieken) en de feedback van issue #6 en deuren. Geluid, meerdere levels, toegankelijkheid, security, releases, metadata en platformonderzoek heb ik gearchiveerd omdat ze niet nodig zijn voor de game die ik in de GDD heb beschreven.
- Bij elke kaart een tijdsinschatting gezet en een veld voor de echte tijd toegevoegd.
- Bij de afgeronde kaarten een link naar de commit gezet en opgeschreven hoe ik het heb gecontroleerd.
- De pickups-kaart staat nu in In review, want de HUD moet ik nog met de hand in de terminal controleren. Deuren heb ik een eigen kaart gegeven.

**Volgende keer:** een kaart pas naar Done zetten als hij eerst in In review heeft gestaan, en bij het starten van een kaart meteen opschrijven wanneer hij klaar is.
