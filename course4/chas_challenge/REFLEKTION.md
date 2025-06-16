# Reflektion Chas Challenge
Chas Challenge-grupp 2 (Kusten är klar)  
Jennifer Gott  
2025-06-16  
## Beskriv hur ditt teams arbete fungerade under Chas Challenge
Det var väldigt spretigt. Vi valde projekt efter votering i stora gruppen och både jag och Erik kände att som det var definerat för vår subgrupp så var det på det stora hela ganska lite jobb. Det gjorde oss oroliga att vi skulle hamna i liknande problem som tidigare grupparbeten i den här konstellationen där det inte finns jobb att dela upp på fem personer. Och så var det. Det ledde till svåra planeringsproblem och känsla av deltagande i gruppen.
Vi satte upp ett gruppkontrakt för hur arbetet skulle bedrivas, men efterföljandet var inte bra.
### Vad bidrog du med?
Jag hjälpte till att leda ceremonierna i början av projektet, som inledande planering och arkitekturdiskution. Jag tog på mig att vara huvudansvarig för hårdvarukoppling samt att lösa att ha flera oilka nätverksinterface eftersom vi ville ha både Ethernet PoE och WiFi tillsammans med Sabina. Idén var att man kunde ha PoE som matning men valbart om WiFi eller Ethernet för kund så man kan fastmontera enheten och inte behöver bry sig om signalstyrka och dylikt, vilket kan vara lämpligt för en fast installation.
I slutändan skrev jag om hela *backend*-kopplingen på inlämningsdagen då den tidigare lösningen inte hanterade https eller *backend*-teamets API.
## Beskriv hur samarbetet med resten av teamen fungerade
Vi hade synk-möten varje tisdag och ibland extra möten med *backend* efter deras *boiler room* på fredagar för att synka vad vi skulle göra på måndagen. På det stora hela tycker jag att det samarbetet funkade bra, även om jag önskar att några detaljer kanske hanterades lite bättre.
### Vad var din roll och hur påverkade du samarbetet?
Jag var lite informell *scrum master* på våra tisdagsmöten och såg till att synkmöten blev bokade när de behövdes. Jag deltog också på synkmötena för att vara införstådd på innehållet för att kunna hjälpa till.
## Vad hade ni för utmaningar under Chas Challenge?
### Tekniska utmaningar
Jag upptäckte när vi fick hårdvaran att den PoE-shield vi köpt inte funkade med vår Arduino. En annan Ethernet-sköld som jag införskaffade själv fungerade i första skedet men fick styrka på foten när vi behövde gå till enbart https eftersom den inte hade stöd för det.
Vår föredragna luftkvalitetssensor försvann från lager mellan vår beställningsinlämning och slutbeställningen, vilket ledde till att vi fick köpa en annan med ett annat interface.
Temperatursensorn vi först valde hade för stor avvikelse och fick bytas till en annan som vi hade till hands.
*backend*-API:t utvecklades över tid och speciellt nära inpå slutet.
### Kommunikationen mellan medlemmar
Jag och Erik gjorde en hel del arbete tillsammans och vårt samarbete fungerade mycket bra.
Drygt halva teamet jobbade på distans och trots att de behövde mycket hjälp för att vara delaktiga över huvud taget tog de inga initativ till detta. Jag hörde knappt av dem alls. Johan deltog i viss utsträckning när vi hade gemensamt arbete utanför *boiler room*. 
Oscar var mycket territoriell över det han såg som sina delar och mycket negativ mot resten av gruppen. Han slutade komma på *boiler room* på slutet.
*Boiler rooms* blev inte produktiva då mycket tid lades på problem med kommunikationsteknik och liknande, och de sm var på distans och minst engagerade var de som hade behövt deltagandet mest. Mycket tid lades på diskutioner utan substans.
### Vad gjorde du för att hjälpa till med att lösa problemen?
Jag erbjöd mig att vara partner till vem som helst som behövde hjälp med att komma igång att producera. Jag försökte ge konstruktiv feedback på metod och genomförande och på ett produktperspektiv. jag försökte förankra vårt arbete i den överenskomna projektplanen.
Jag hade fått feedback tidigare på att inte vara för drivande i projektet, så jag försökte släppa fram andra att ta för sig.
Eftersom jag inte hörde mycket från de andra var det svårt att förhålla sig till detta. Jag kände till att det fanns personliga omständigheter som gjorde det svårt att planera in arbete alla dagar i veckan, så jag kände att de behövde ta initativet och föreslå tillfällen som passade dem. Detta skedde inte.
## Vilka framsteg gjorde ni under Chas Challenge?
Vi gjorde en ganska bra ansats till en produkt. Den fungerar i stort som vi hade föreställt oss i början. Med lite massage skulle den vara riktigt bra.
### Vad lärde du dig under arbetet?
Jättemycket! Jag tycker att jag har en mycket bättre förståelse för hur man ska ta sig an en sån här uppgift i framtiden.
#### Kunskapsmässigt
- Skriva hårdvaruabstraktionslager:
Jag ville att vi skulle uppfylla vår testbarhetsambition från projektplanen och gjorde en ansträngning att göra vår kod testbar. Jag ville kunna köra testerna direkt på datorn utan hårdvaran inkopplad, och då behövde jag abstrahera Arduino-funktionerna som `delay()`, `millis()` och objekt som `Wire`.
- Tillståndshantering i system:
Jag kom fram till att vi borde ha olika tillstånd för olika intervall av mätning beroende på tid på dagen, helger, osv. det var en jättespännande utforskning som tyvärr inte kom med i slutprodukten.
Däremot lade jag till tillstånd för enhetens uppstart för att hantera olika felfall. Exempelvis kan ett fel som timeout från nätverket vara nåt man bara kan vänta ut och försöka igen senare, medan felaktig JWT-login betyder att det bara är att avbryta allt och försätta sig i passivt läge.
- Hantering av feltillstånd i nätverkskommunikation:
Syntesen av dessa två blev att jag började utförska enum som returtyper, som t.ex. `EepromError` eller `HttpError`, som man också kan ha en `operator bool` på för att se om returvärdet är ett fel eller inte. Det gjorde genast att koden blev mer uttrycksfull och lättare att resonera om.
- Typning av hårdvaruspecifika värden:
`PinNumber`, `EepromAddress`, `HttpRequest` och liknande typer istället för integraltyper eller strängar. 
- Verktyg:
PlatformIO, automatiserade tester, att göra en *debug-environment* med mer utskrift än release, att hantera konfigurationer osv. Verkligen jättespännande!
- Skriva bibliotek
Jag portade mitt abstraktionslager till ett platformIO-bibliotek vilket var helt nytt för mig.
#### På det personliga planet
Det är jättesvårt att planera nånting på det här planet när kunskaps- och ambitionsnivån skiljer sig så mycket. Vi hade behövt sitta i stor grupp tillsammans mycket mer och mob-programmera istället för att dela upp oss i mindre ansvarsområden. Men omständigheterna i gruppen gjorde detta svårt att realisera, det fanns väldigt lite utrymme för gemensamt arbete.
Jag tycker att det blir ännu mer ojämt när jag nu efter Chas Challenge känner att jag har lärt mig så himla mycket och fått en väldigt djup insikt i hela verktygskedjan från hårdvara till bibliotek, när som de som hade svårt att hänga med i början inte kan hänga med alls längre.
Det räcker också inte att bara erbjuda att hjälpa till, de andra gruppmedlemmarna behöver vilja vara med också. Jag fick uppfattningen att de hade gett upp lite innan vi redan hade börjat.
## Vad jag hade gjort annorlunda
Om jag hade kunnat hoppa tillbaka till början och göra på mitt sätt så hade jag försökt få med det här:
### Rolldefinition
Vi borde haft en tydligare rollfördelning med t.ex. produktansvarig för helheten (*product manager*), produktledare för varje leveransområde (*product owner* SUVX, *product owner* FSJX osv.) och ceremoniledare (typ *scrum master*). Nu var vår uppdelning mycket mer *ad hoc*. Det gjorde det för otydligt hur vi skulle utveckla arbetet under projektets gång.
#### Planering
- Tydliga *feature*-beskrivningar, skrivna på ett konsekvent sätt (*user story* t.ex.)
- Använda dessa *feature*-beskrivningar för att definera acceptans-tester
- Strukturera arbetet kring uppfyllandet av acceptans-testerna
### Uppföljning
Vi borde haft en tydligare uppföljningsstruktur än den vecko-checkin vi hade på tisdagar. Vi borde haft ett löpande arbete med att skapa tydligt avgränsade arbetsuppgifter.
## Sammanfattning
Vårt teams arbete blev spretigt då uppgifterna var få och svåra att fördela; jag försökte ta en samordnande roll och försökte andra leda olika teknikdelar. Tekniskt stötte vi på problem som inkompatibla Ethernet-sköldar, sensorbrist och ett föränderligt backend-API, medan kommunikationen försvårades av distansarbete och ojämlik drivkraft. Jag försökte avlasta gruppen genom parprogrammering, konstruktiv feedback och förankring i projektplanen, men höll samtidigt igen för att ge andra utrymme. 
Trots utmaningar nådde vi en fungerande produkt och jag lärde mig enormt mycket om hårdvaruabstraktion, tillståndshantering, felhantering, PlatformIO, testbarhet och biblioteksskrivande. Erfarenheten visade behovet av mer mob-programmering när kunskapsnivåerna spretar, då klyftorna annars snabbt växer under projektets gång.