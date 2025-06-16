# Reflektion
Jennifer Gott
2025-06-16
## Beskriv hur din subgrupps arbete fungerade under CC.
Det var väldigt spretigt. Vi valde projekt efter votering i stora gruppen och både jag och Erik kände att som det var definerat för vår subgrupp så var det på det stora hela ganska lite jobb, och vi skulle hamna i liknande problem som tidigare, där det inte finns jobb att dela upp på fem personer. Det ledde till svåra planeringsproblem och känsla av deltagande i gruppen.
### Vad bidrog du med?
Jag hjälpte till att leda ceremonierna i början av projektet, som inledande planering och arkitekturdiskution. Jag tog på mig att vara huvudansvarig för hårdvarukoppling samt att lösa att ha flera oilka nätverksinterface eftersom vi ville ha både Ethernet PoE och WiFi tillsammans med Sabina. Idén var att man kunde ha PoE som matning men valbart om WiFi eller Ethernet för kund så man kan fastmontera enheten och inte behöver bry sig om signalstyrka och dylikt, vilket kan vara lämpligt för en fast installation.
I slutändan skrev jag om hela backend-kopplingen på inlämningsdagen då den tidigare lösningen inte hanterade https eller backend-teamets API.
## Beskriv hur samarbetet med resten av teamen fungerade?
Vi hade synk-möten varje tisdag och ibland extra möten med backend efter deras boiler room på fredagar för att synka vad vi skulle göra på måndagen. På det stora hela tycker jag att det samarbetet funkade bra, även om jag önskar att några detaljer kanske hanterades lite bättre.
### Vad var din roll och hur påverkade du samarbetet?
Jag var lite informell scrum master på våra tisdagsmöten och såg till att synkmöten blev bokade när de behövdes. Jag deltog också på synkmötena för att vara införstådd på innehållet.
## Vad hade ni för utmaningar under CC?
### Tekniska
Jag upptäckte när vi fick hårdvaran att den PoE-shield vi köpt inte funkade med vår Arduino. En annan Ethernet-sköld som jag införskaffade själv fungerade i första skedet men fick styrka på foten när vi behövde gå till enbart https eftersom den inte hade stöd för det.
Vår föredragna luftkvalitetssensor försvann från lager mellan vår beställningsinlämning och slutbeställningen, vilket ledde till att vi fick köpa en annan med ett annat interface.
Temperatursensorn vi först valde hade för stor avvikelse och fick bytas till en annan som vi hade till hands.
Backends API utvecklades över tid och speciellt nära inpå slutet.
### Kommunikationen mellan medlemmar
Jag och Erik gjorde en hel del arbete tillsammans och vårt samarbete fungerade mycket bra.
Drygt halva teamet jobbade på distans och trots att de behövde mycket hjälp för att vara delaktiga över huvud taget tog de sällan initativ till detta. 
Oscar var mycket territoriell över det han såg som sina delar och mycket negativ mot resten av gruppen. Han slutade komma på boiler room.
### Vad gjorde du för att hjälpa till med att lösa problemen?
Jag erbjöd mig att vara partner till vem som helst som behövde hjälp med att komma igång att producera. Jag försökte ge konstruktiv feedback på metod och genomförande och på ett produktperspektiv. jag försökte förankra vårt arbete i den överenskomna projektplanen.
Jag hade fått feedback tidigare på att inte vara för drivande i projektet, så jag försökte släppa fram andra att ta för sig. 
## Vilka framsteg gjorde ni under CC?
Vi gjorde en ganska bra ansats till en produkt. Den fungerar i stort som vi hade föreställt oss i början. 
### Vad lärde du dig under arbetet?
Jättemycket! Jag tycker att jag har en mycket bättre förståelse för hur man ska ta sig an en sån här uppgift i framtiden.
#### Kunskapsmässigt
- Skriva hårdvaruabstraktionslager
Jag ville att vi skulle uppfylla vår testbarhetsambition från projektplanen och gjorde en ansträngning att göra vår kod testbar.
- Tillståndshantering i system
Jag kom fram till att vi borde ha olika tillstånd för olika intervall av mätning beroende på tid på dagen, helger, osv. det var en jättespännande utforskning som tyvärr inte kom med i slutprodukten.
- Hantering av feltillstånd i nätverkskommunikation
Syntesen av dessa två blev att jag började utförska enum som returtyper, som t.ex. EepromError eller HttpError, som man också kan ha en operator bool på för att se om returvärdet är ett fel eller inte. Det gjorde genast att koden blev mer uttrycksfull och lättare att resonera om.
- Verktyg
PlatformIO, automatiserade tester, att göra en debug-environment med mer utskrift än release, att hantera konfigurationer osv. Verkligen jättespännande!
- Skriva bibliotek
Jag portade mitt abstraktionslager till ett platformIO-bibliotek vilket var helt nytt för mig.
#### På det personliga planet
Det är jättesvårt att planera nånting på det här planet när kunskaps- och ambitionsnivån skiljer sig så mycket. Vi hade behövt sitta i stor grupp tillsammans mycket mer och mob-programmera istället för att dela upp oss i mindre ansvarsområden.
Jag tycker att det blir ännu mer ojämt när jag nu efter chas challenge känner att jag har lärt mig så himla mycket och fått en väldigt djup insikt i hela verktygskedjan från hårdvara till bibliotek, när som de som hade svårt att hänga med i början inte kan hänga med alls längre.
## Sammanfattning
Vårt teams arbete blev spretigt då uppgifterna var få och svåra att fördela; jag försökte ta en samordnande roll och försökte andra leda olika teknikdelar. Tekniskt stötte vi på problem som inkompatibla Ethernet-sköldar, sensorbrist och ett föränderligt backend-API, medan kommunikationen försvårades av distansarbete och ojämlik drivkraft. Jag försökte avlasta gruppen genom parprogrammering, konstruktiv feedback och förankring i projektplanen, men höll samtidigt igen för att ge andra utrymme. 
Trots utmaningar nådde vi en fungerande produkt och jag lärde mig enormt mycket om hårdvaruabstraktion, tillståndshantering, felhantering, PlatformIO, testbarhet och biblioteksskrivande. Erfarenheten visade behovet av mer mob-programmering när kunskapsnivåerna spretar, då klyftorna annars snabbt växer under projektets gång.