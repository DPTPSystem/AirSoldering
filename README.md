# AirSoldering
* Forrólevegős SMD forrasztó állomás
* DPTP System 2023-05-24.
* Projekt keletkezése: 2014-05-16.
* 4 soros karakteres kijelző, PIC18F4550 és vagy PIC18F452, K típusú hőszenzor és MAX6675 - MPLAB IDE 8.91

![DPTP System BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/131744_dptp_bga_gep.jpg "DPTP System BGA soldering")

# A kezdet és figyelmeztetés
Anno 2014-ben kezdtem el komolyabban fogglalkozni a mikrokontrollerekkel és az azokra írható programokkal. Ezzel egyidőben az elektrokinával is 
fogglakoznom kellett mert a mikrokontrollerek önmagukban nem sokat érnek, kell egy jó elektronikai kialakítás mondhatni körítés, amely majd 
betölti azt a szerepet, amellyel a programunk képes a vezérlést elvégezni. Mivel a kezdet mindig nehéz és sok tanulással jár sokáig nem készítettem
sematikus/kapcsolási rajzokat, szimplán elkészítettem fejből a nyáktervet és a nyáktervekre írtam fel az alkatrészek paramétereit. Amjd ezt használva
beültettem az elkészült panelekt. Ezért ez a projekt nem tartalmaz kapcsolási rajzot, viszont a nyákterven fel vannak tüntetve az alkatrészek
megnevezései. Az elkészült panel egyszerűsége olyan mértékű, hogy kis szakértelemmel, nagyon könnyen át lehet látni. Kérek mindenkit, hogy ennek
tudatában álljon neki az utánépítésnek. Köszönöm.

# Miért
2005-ben megjelent Xbox360-as konzolok hirtelen, gyors és tömeges meghíbásodásai miatt, mondhatni kényszerhelyzetben egy prototípus gépen
kezdtünk el dolgozni, amely arra volt hivatott, hogy megoldja a BGA tokozott chippek házi körülmények közt is az ipari színtű javítását/cseréjét.
A BGA chippek cseréjéhez fontos volt egy alsó PCB és felső kimondottan a BGA chipekhez igazítható forrasztási mehanizmus megalkotásárára, így
született az első PID vezérlésű saját forrasztó államásunk, amellyel talán az országban elsőként kezdük el az Xbox360-as konzolok tömeges javítását.
Ezt követően a rengeteg tapsztalatra és a fejlődő tudásunkra támaszkodva készítettük el az ebben a szálban megosztott forrólevegős, hőprofilos
forrasztó berendezés terveit és programját.

# Célok
A célunk az volt, hogy a kisebb BGA tokozású, de akár SMD alkatrészeket, hőprofilt alkalmazva, optimális a lehető legkisebb hőstressz melett
legyünk képesek a javításokat elvégezni, BGA alkatrészeket cserélni. Fontos volt számunkra, hogy úgy tudjunk javításokat végezni, hogy közben
a tapasztalatainkra tudjunk támaszkodni és ennek megfelelően a már kitesztelt és jól bevált hőprofilok mellett a lehető legkisebb károsodással
járjanak a kényes berenezések esetén is a bevatkozások.

# Hiba
A képen egy hibás hőprofilos forrasztás következménye.:

![Faulty BGA soldering](https://github.com/DPTPSystem/AirSoldering/blob/master/images/131744_bga2.jpg "Faulty BGA soldering")

# Hardver
A hardver maga nincs túl bonyolítva, összeségében egy PCB készült, amely tartalmaz minden elektronikai részt kivéve a tápegységet. A vezérlés
központjában egy (az első típusoknál) PIC18F4550-es Microchip MCU helyezkedik el (későbbi verzióknál PIC18F452-est használtam), amely kommunikál
egy 64KByte-os memóriával, egy K típusú szenzorral, egy 4x4-es gomsorral, egy karakteres kijelzővel és egy PWM vezérelt ventillátorral. 
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
A vezérlő programja még eléggé kezdetleges, de már elég komleksz ahhoz, hogy releváns lehessen. Képes a vezérlő elmenteni 15 előre beállított
hőprofilt, képes az utolsó beállított hőprofilt megjegyezni és a következő indulásnál az vissza tölteni, illetve üresjáratban képesek vagyunk,
új profilt betölteni. A húprofil beállítása összesen 6 lépésből áll, amelyek a következőek:
- 6 x lépés, maximum hőmérséklet, felmelegedési várakozás / kitartás

A hőprofil illetve a forrasztási program lejártát követően a visszahútési eljárásra vált a program, amely ~6 fok/másodperc egészen 80 fokig, 
majd ezt követően a légszállítás a legalacsonyabb fokozatba lép majd 50 fokot elérve le is áll.

![Display](https://github.com/DPTPSystem/AirSoldering/blob/master/images/14484406336272_b.jpg "Display")

# Ahol megoldást jelentett a berendezés
Ebben a szakaszban és rövid felsorolásban néhány területet és célgépet említenék meg, ahol sikerrel tudtuk használni az elkészült gépet:
1. játék konzolok, Xbox, PS3, SEGA, Nintendo, Arcade, Neo Geo és még sok más játék
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

DPTP System - Hőprofilos forrasztó - Működj Pályázat 2017 
https://www.youtube.com/watch?v=RiwQ2Q4VDKY

# Továbbfejlesztés
A projekt elkészülte után 1 évvel, további fejlesztések történtek, amelyek a készülékeket kiegészítette egy frissítési szolgáltatással, amely 
a bootloader segítségével megoldja a már kész gépek új szoftververzióval történő frissítését USB kábelen keresztül. A további szofveres
fejlesztések már csak akarat és idő kérdése. A gépen elkezdődőt egy SMD forrasztási mód megírása, de idő és érdeklődés hiányában ez a rész
a demón kívűl nem készűlt el. 

# Új verzió
Készült a készülékből egy komolyabb kijlezővel és funkciókkal kibővített PIC18F46K22-es MCU-ra épített verzió, amely 80%-ban el is készült, de
az összeszerelés fázisában idő hiány miatt megrekedt. Erről az új kibővített verzióról, mivel nem készült el teljesen és nem lett tesztelve
éles helyzetben nem osztom meg, de pár képet és videó anyagot tudok közzétenni. Az új verzió már több höszenzort, több PWM vezérelt meghajtást
grafikus kijelzőt és alsó IR panel vezérlést is tartalmaz illetve további kisebb felhasználói funkciót tartalmaz.

![Air BGA Station 2.0](https://github.com/DPTPSystem/AirSoldering/blob/master/images/forraszto_v3.JPG "Air BGA Station 2.0")

DPTP System - V3 Forrasztást vezérlő prototípus 
https://www.youtube.com/watch?v=cjfGa7zWkMY