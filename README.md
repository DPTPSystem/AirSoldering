# AirSoldering
* Forrólevegős SMD forrasztó állomás
* DPTP System 2023-05-24.
* Projekt keletkezése: 2014-05-16.
* 4 soros karakteres kijelző, PIC18F4550 és vagy PIC18F452, K típusú hőszenzor és MAX6675 - MPLAB IDE 8.91

![DPTP System BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/131744_dptp_bga_gep.jpg "DPTP System BGA soldering")

# A kezdet és figyelmeztetés
Anno 2014-ben kezdtem el komolyabban fogglalkozni a mikrokontrollerekkel és az azokra írható programokkal. Ezzel egyidőben az elektrokinával is 
fogglakoznom kellett mert a mikrokontrollerek önmagukban nem sokat érnek, kell egy jó elektronikai kialakítás mondhatni körítés, amely majd 
betölti azt a szerepet, amellyel a programunk képes a vezérlést elvégezni. Mivel a kezdet mindig nehéz és sok tanulással jár, sokáig nem készítettem
sematikus/kapcsolási rajzokat, szimplán elkészítettem fejből a nyáktervet és a nyáktervekre írtam fel az alkatrészek paramétereit. Amjd ezt használva
beültettem az elkészült panelekt. Ezért ez a projekt nem tartalmaz kapcsolási rajzot, viszont a nyákterven fel vannak tüntetve az alkatrészek
megnevezései. Az elkészült panel egyszerűsége olyan mértékű, hogy kis szakértelemmel nagyon könnyen át lehet látni. Kérek mindenkit, hogy ennek
tudatában álljon neki az utánépítésnek. Köszönöm.

# Miért
2005-ben megjelent Xbox360-as konzolok hirtelen, gyors és tömeges meghíbásodásai miatt, mondhatni kényszerhelyzetben egy prototípus gépen
kezdtünk el dolgozni, amely arra volt hivatott, hogy megoldja a BGA tokozott chippek házi körülmények közt történő ipari színtű javítását/cseréjét.
A BGA chippek cseréjéhez fontos volt egy alsó PCB és felső kimondottan a BGA chipekhez igazítható forrasztási mehanizmus megalkotásárára, így
született az első PID vezérlésű saját forrasztó államásunk, amellyel talán az országban elsőként kezdük el az Xbox360-as konzolok tömeges javítását.
Ezt követően a rengeteg tapsztalatra és a fejlődő tudásunkra támaszkodva készítettük el az ebben a szálban megosztott forrólevegős, hőprofilos
forrasztó berendezés terveit és programját.

# Célok
A célunk az volt, hogy a kisebb BGA tokozású, de akár SMD alkatrészeket, hőprofilt alkalmazva, optimális a lehető legkisebb hőstressz melett
legyünk képesek a javításokat elvégezni, BGA alkatrészeket cserélni. Fontos volt számunkra, hogy úgy tudjunk a javításokat végezni, hogy közben
a tapasztalatainkra tudjunk támaszkodni és ennek megfelelően a már kitesztelt és jól bevált hőprofilok mellett a lehető legkisebb károsodással
járjanak a kényes berenezések esetén is a beavatkozások.

# Elmélet a megoldás
A rendszer elméleti és egyben a megoldása is a sikeres utánépítésnek, hogy valamelyest képet alkossak arról, hogy milyen további eszközöket kell 
beszerezned, hogy megfelelő írányba indulhass el. Hogy mi a jó és mi nem azt nem akarom senkinek megírni, de hogy én milyen alkotó elemeket
használtam fel azt nyugodtan tudom leírni. 9 fő részre osztható a teljes állomás:
1. aluminium doboz, mérete függ majd a légszállítást végző ventillűtor méretétől.
2. légszállításra PWM vezérelhető 12v-os ventillátor (PS3 ventillátor, ~1.5A).
3. 3D nyomtatott vagy valamilyen kialakítással meg kell oldani a megfelelő légcsatornát
4. 4soros, karakteres kijelző HD44780
5. K-type sensor (80-100cm) vagy forrólevegős forrasztó fej, amelyben benne van a K-type szenzor
6. 12v-os kapcsolóüzemű tápegység. (fontos, hogy ipari vagy jó minőségú legyen, ne legyen zajos)
7. 4x4-es matrix gombsor, billenő bekapcsoló gomb, USB kimenet (opcionális)
8. 45A-es logikai szilárdtestrelé 600v-os (fűtőszál kapcsolgatására)
9. Buzzer (csipogó)

Elméleti működés: A gép billenő kapcsolóval felkapcsólva az előre elmentett hőprofilt betölti és várakozik a felhasználóra. A memória és a MAX6675
SPI kapcsolaton keresztül kommunikál. A memóriába összesen 15, egyenként hat lépésben megadott hőprofilt lehet elmenteni, amelyet egy menüből
később el lehet érni illetve be lehet tölteni. Minden betöltésnél automatikusan a PIC EEPROM memóriájába lementni a betöltött profil számát, amely
a memóriában lévő pozicióját határoza meg. Indulásnál ezt az EEPROM területet vizsgálja a program és ennek megfelelően tölti be az utoljára kiválasztott
profilt. Az eszközön 3 LED is található. Rendserint ezek különböző színűek, az egyik LED az üzemelést hivatott jelezni ez álltalában piros, a második
egy kék színű LED, amely a fejben lévő fűtőszál üzemidejét jelzi vissza. A harmadik LED egy villogó sárga vagy piros visszajelző a teszt vagy 
demó programot mutatja, amely majd egyszer az SMD forrasztás programmódja lesz. (egyszer)
A kezelőfelületnél mindenki eldöntheti mit mire használ, csak ebben az esetben a programban ezen részeket át kell írni, de úgy vélem a lehető 
legoptimálisabb és kézenfekvő funkciókat tettem a gombokra, amelyeket a kijelzőn jelzek is.
A MAX6675 egy K típusú szenzorhoz készült analog digitális converter, amely az alapvető kompenzációkat tartalmazza, és 12bit-es digitális jelre alakítja,
amelyekhez SPI-vel lehet hozzáférni. A PWM vezérlés a légáramlat függvényében adható meg figyelembe véve, a forrasztófejre helyzett szűktést.
A hőprofil elindulását követően 4 fontos értéket láthatunk a kijelzőn. Felüle a beállított hőmérségklet és idő látható, alattuk pedig az aktuális
adatokat írja ki. A programot bármikor meg lehet szakítani, majd azt újra indítani vagy módosítani a szerkesztés menüponton keresztül. Fontos lehet
megemlítenem, hogy a 4x4-es gombsort egy saját elképzelés alapján megírt program vezérli, amely a régi telefonoknál (NOKIA) megszokott SMS írásainál
hazsnálhattunk. Szöveg beírását követően fel fogja kínálni a lehetőségeket a program. A beírásnál elég jól sikerült behatárolnom és kitesztelnem az 
elkövethető direkt vagy véletlen hibákat, így a program majd ennek megfelelően korrígál. A megszakításokat kimértem, így az 1 másdoperc az pontosnak
nevezhető. A MAX6675 és a K-type szenzort valós körülmények közt teszteltem és ennek eredményeként és megfelelően állítottam be a szoftveres kompenzációt, 
amelyek különböző hőmérsékletek esetében más-más is lehet. Erre utóbbira külön korrekciós tálát is hoztam létre. 
A fűtőszálat egy erre alkalmas és gyors logikai szilárdtestrelével kapcsolgatom, fontos, hogy tényleg gyors legyen és a kapcsoló árama ne legyen nagyobb
egy LED-énél, vagy is 5v 15-30mA. Ha nagyobb, akkor szükséges az elektronikát egy megfelelő gyors tranzisztorral vagy FET-el kiegészíteni. Hang
visszajelzés 12v-al működik, egy tranzisztoros kapcsolással. Minden indulásnál és a menüpontok közti váltásnál illetve minden fontosabb funkciónál
visszajelzést ad az állapotról. Álltalában ezek a visszajelzések egy-egy rövidebb csippanással járnak. Úgy gondolom ennyi elég az elméletről, ha
további kérdések merülnek fel írj nyugodtan, ha időm engedi és időben észre veszem azonnal reagálok.

# Hiba
A képen egy hibás hőprofilos forrasztás következménye.:

![Faulty BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/131744_bga2.jpg "Faulty BGA soldering")

# Hardver
A hardver maga nincs túl bonyolítva, összeségében egy PCB készült, amely tartalmaz minden elektronikai részt kivéve a tápegységet. A vezérlés
központjában egy (az első típusoknál) PIC18F4550-es Microchip MCU helyezkedik el (későbbi verzióknál PIC18F452-est használtam), amely kommunikál
egy 64KByte-os memóriával, egy K típusú szenzorral, egy 4x4-es gombsorral, egy karakteres kijelzővel és egy PWM vezérelt ventillátorral. 
A PCB furatszerelt egyoldalas majd később felületszerelt egyoldalas és később felületszerelt kétoldalas kivitelben készült el.

![SMD BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/14484406329764_b.jpg "SMD BGA soldering")

# Nyáktervek
Furatszerelt verzió:

![BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/pcb1.PNG "BGA soldering")

![BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/pcb1_b.PNG "BGA soldering")

SMD változat:

![SMD BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/pcb2.PNG "SMD BGA soldering")

![SMD BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/pcb2_c.PNG "SMD BGA soldering")

# Program
A vezérlő programja még eléggé kezdetleges, de már elég kompleksz ahhoz, hogy releváns lehessen. Képes a vezérlő elmenteni 15 előre beállított
hőprofilt, képes az utolsó beállított hőprofilt megjegyezni és a következő indulásnál vissza tölteni azt, illetve üresjáratban képesek vagyunk,
új profilt betölteni. A húprofil beállítása összesen 6 lépésből áll, amelyek a következőek:
- 6 x lépés, maximum hőmérséklet, felmelegedési várakozás / kitartás

A hőprofil illetve a forrasztási program lejártát követően a visszahűtési eljárásra vált a program, amely ~6 fok/másodperc egészen 80 fokig, 
majd ezt követően a légszállítás a legalacsonyabb fokozatba lép majd 50 fokot elérve le is áll.

![Display](https://github.com/DPTPSystem/AirSoldering/blob/master/images/14484406336272_b.jpg "Display")

# Ahol megoldást jelentett a berendezés
Ebben a szakaszban és rövid felsorolásban néhány területet és célgépet említenék meg, ahol sikerrel tudtuk használni az elkészült gépet.
1. játék konzolok, Xbox, PS, SEGA, Nintendo, Arcade, Neo Geo és még sok más játék
2. nehéz gépek, targonca vezérlés, hasznon gépjárművek vezérlése
3. krumpli válogató gyártosori vezérlés
4. gyermekorvosi szív ultrahang és normál ultrahang gépek javítása
5. PC VGA kártyák javítása
6. Személyautók vezérlése
7. Hordozható számítógépek
8. és további BGA vagy SMD, esetenként diszkrét alaktarészekkel szerelt elektronikai berendezések javítása.

# Kapcsolodó videós tartalmak
DPTP System - PS3 ventilátor szabályozás
https://www.youtube.com/watch?v=6KxBIMjXPOY

DPTP System - Led digitális PWM vezérléssel
https://www.youtube.com/watch?v=YQ3GbyC51UQ

DPTP System - 4x4-es mátrix keypad PIC-el 
https://www.youtube.com/watch?v=JBfIr0ge0tk

DPTP System - 4x4 mátrix - PIC 
https://www.youtube.com/watch?v=hZN4geVwEG4

DPTP SYSTEM - Forrasztó vezérlő prototípus 
https://www.youtube.com/watch?v=Y2zX46Tqc7M

DPTP System - ZPM Forrasztó 
https://www.youtube.com/watch?v=1Q6x2KFnHvs

# Verseny
Tanulmányaim során volt lehetőségem versenyen is indulni, de ezen projekttel díjat nem sikerült bezsebelnem. (Korábbi évben egy kupak 
elektronikával, díjazott helyezést értem el, amelyet a muszakivagyok.hu oldalon napjainkban is el lehet érni)

DPTP System - Hőprofilos forrasztó - Működj Pályázat 2017
https://www.youtube.com/watch?v=RiwQ2Q4VDKY

# Továbbfejlesztés
A projekt elkészülte után 1 évvel további fejlesztések történtek, amelyek a készülékeket kiegészítette egy frissítési szolgáltatással, amely 
egy bootloader segítségével megoldja a már kész gépek új szoftververzióval történő frissítését, USB kábelen keresztül. A további szofveres
fejlesztések már csak akarat és idő kérdése. A gépen elkezdődőt egy SMD forrasztási mód megírása, de idő és érdeklődés hiányában ez a rész
a demón kívűl nem fejeződött be.

# Új verzió
Készült a készülékből egy komolyabb kijlezővel és új funkciókkal kibővített PIC18F46K22-es MCU-ra épített verzió, amely 80%-ban el is készült, de
az összeszerelés fázisában idő hiány miatt megrekedt. Erről az új kibővített verzióról, mivel nem készült el teljesen és nem lett tesztelve
éles helyzetben, nem osztom meg, de pár képet és videó anyagot tudok közzétenni. Az új verzió már több hőszenzort, több PWM vezérelt meghajtást,
grafikus kijelzőt és alsó IR panel vezérlést is tartalmaz, illetve további kisebb felhasználói funkciót tartalmaz.

![Air BGA Station 2.0](https://github.com/DPTPSystem/AirSoldering/blob/master/images/forraszto_v3.JPG "Air BGA Station 2.0")

DPTP System - V3 Forrasztást vezérlő prototípus 
https://www.youtube.com/watch?v=cjfGa7zWkMY