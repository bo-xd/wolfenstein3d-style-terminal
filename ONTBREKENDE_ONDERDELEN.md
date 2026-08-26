# Ontbrekende onderdelen

Dit document bevat onderdelen die nog niet daadwerkelijk in het project aanwezig of aantoonbaar zijn. De README beschrijft de huidige situatie, maar documentatie alleen bewijst niet dat een onderdeel ook is uitgevoerd.

## Hoogste prioriteit

- [ ] Een `LICENSE`-bestand met een bewust gekozen softwarelicentie toevoegen.
- [ ] Geautomatiseerde unit tests toevoegen.
- [ ] Integratietests voor map loading, raycasting en shooting toevoegen.
- [ ] Een CI-pipeline met GitHub Actions toevoegen.
- [ ] Een aantoonbaar projectproces bijhouden met planning, taken en reflectie.

## Testen en codekwaliteit

- [ ] Mapvalidatie automatisch testen.
- [ ] Collision detection aan alle kaartranden testen.
- [ ] `CastRay` testen op afstand en geraakte muurzijde.
- [ ] Hits, misses, wall occlusion, damage en kills testen.
- [ ] Gedrag bij nul munitie testen.
- [X] Builds uitvoeren met GCC en Clang.
- [ ] AddressSanitizer gebruiken.
- [ ] UndefinedBehaviorSanitizer gebruiken.
- [ ] Static analysis uitvoeren met bijvoorbeeld `clang-tidy`.
- [ ] Fuzztests uitvoeren met ongeldige kaartbestanden.
- [ ] Foutcodes van ncurses-functies controleren.

## Git en DevOps

- [X] Kleine en duidelijk beschreven commits maken.
- [ ] GitHub Issues of een backlog gebruiken.
- [ ] Featurebranches gebruiken voor grotere wijzigingen.
- [ ] Pull requests en code reviews aantonen.
- [ ] Versies taggen.
- [ ] GitHub Releases maken.
- [ ] Een `CHANGELOG.md` bijhouden.
- [ ] Automatische dependency- of kwetsbaarheidsscans toevoegen.
- [ ] Repositorymetadata toevoegen, zoals een beschrijving en topics.

## Projectproces en zelfstandigheid

- [ ] Een planning met taken en prioriteiten bijhouden.
- [ ] Tijdsinschattingen per taak vastleggen.
- [ ] Werkelijke uren of workload bijhouden.
- [ ] Ontvangen feedback vastleggen.
- [ ] Beschrijven hoe feedback is verwerkt.
- [ ] Iteraties en gemaakte ontwerpkeuzes bijhouden.
- [ ] Persoonlijke reflectie op problemen en oplossingen schrijven.
- [ ] Bij samenwerking de taakverdeling en eigen verantwoordelijkheid aantonen.

## Ontwerp en diagrammen

- [ ] Een formeel UML-componentdiagram maken.
- [ ] Het bestaande sequentiediagram controleren op UML-conventies.
- [ ] Modulegrenzen en publieke interfaces verder verduidelijken.
- [ ] Globale state verminderen of centraal beheren.
- [ ] Een ERD maken wanneer later een database wordt toegevoegd.

## Programmeertechnieken

- [ ] OOP aantonen wanneer dit verplicht is, inclusief encapsulation, inheritance en polymorphism.
- [ ] Een ECS-implementatie maken wanneer ECS verplicht moet worden aangetoond.
- [ ] Functionele programmeertechnieken aantonen wanneer dit een beoordelingscriterium is.
- [ ] Motiveren waarom procedureel C voor dit project is gekozen.

## Security en privacy

- [ ] Een formeel threat model opstellen.
- [ ] Een securitychecklist per release gebruiken.
- [ ] Dependencyversies en bekende kwetsbaarheden controleren.
- [ ] Controleren dat geen secrets of persoonsgegevens in Git staan.
- [ ] Securitybevindingen en oplossingen vastleggen.
- [ ] De privacyanalyse opnieuw uitvoeren wanneer accounts, telemetry of online scores worden toegevoegd.

## Toegankelijkheid

- [ ] Toetsen configureerbaar maken.
- [ ] Een alternatief kleurenschema of high-contrastmodus toevoegen.
- [ ] Een instelling voor minder beweging of flikkering toevoegen.
- [ ] De game op verschillende terminalthema’s testen.
- [ ] Beperkingen voor screenreaders verder onderzoeken en documenteren.

## Platform en infrastructuur

- [ ] De game met meerdere terminalemulators testen.
- [ ] Ondersteuning met GCC en Clang controleren.
- [ ] Ondersteuning op andere besturingssystemen onderzoeken.
- [ ] Minimale dependencyversies documenteren.
- [ ] CI als passende cloudtoepassing inzetten.
- [ ] Uitleggen waarom SaaS, PaaS en IaaS voor de runtime niet nodig zijn.

## IDE en AI

- [ ] Vastleggen welke IDE of editor is gebruikt.
- [ ] Vastleggen waarvoor AI is gebruikt.
- [ ] Voorbeelden geven van AI-uitvoer die handmatig is gecontroleerd of aangepast.
- [ ] Beschrijven hoe fouten uit AI-uitvoer zijn herkend.
- [ ] Controleren of het AI-gebruik voldoet aan het beleid van school of organisatie.

## Juridisch en licenties

- [ ] Een passende licentie kiezen en toevoegen.
- [ ] De licentievoorwaarden van ncurses controleren en vermelden.
- [ ] Controleren dat geen originele Wolfenstein-assets worden gebruikt.
- [ ] De disclaimer over Wolfenstein en de rechthebbenden behouden.
- [ ] Gebruiksrechten van toekomstige afbeeldingen, audio en dependencies vastleggen.

## Mogelijke inhoudelijke uitbreidingen

Deze punten zijn niet noodzakelijk voor de beroepscriteria, maar kunnen het spel completer maken:

- [ ] Bewegende enemies en eenvoudige AI toevoegen.
- [ ] Deuren toevoegen.
- [ ] Ammo- en healthpickups toevoegen.
- [ ] Meerdere levels ondersteunen.
- [ ] Instelbare kaartgroottes ondersteunen.
- [ ] Geluid toevoegen.
- [ ] Savegames toevoegen.
- [ ] Instellingen voor toetsen en kleuren opslaan.

## Definition of Done voor bovenstaande taken

Een taak mag worden afgevinkt wanneer:

- het resultaat daadwerkelijk in de repository aanwezig is;
- de wijziging zonder waarschuwingen compileert;
- relevante tests zijn uitgevoerd;
- documentatie is bijgewerkt;
- de wijziging een duidelijke Git-commit heeft;
- bewijs beschikbaar is voor een beoordelaar.
