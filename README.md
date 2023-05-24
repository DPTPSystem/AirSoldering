# AirSoldering
* Forrólevegős SMD forrasztó állomás
* DPTP System 2014-05-16.
* 4 soros karakteres kijelző, PIC18F4550 és vagy PIC18F452, K típusú hőszenzor

# A dokumentum most készül a hiba javítása folyamatban és folyamatos 2023-05-24.

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

# Hardver
A hardver maga nincs túl bonyolítva, összeségében egy PCB készült, amely tartalmaz minden elektronikai részt kivéve a tápegységet. A vezérlés
központjában egy (az első típusoknál) PIC18F4550-es Microchip MCU helyezkedik el (későbbi verzióknál PIC18F452-est használtam), amely kommunikál
egy 64KByte-os memóriával, egy K típusú szenzorral, egy 4x4-es gomsorral, egy karakteres kijelzővel és egy PWM vezérelt ventillátorral. 

# Ahol megoldást jelentettünk
Ebben a szakaszban és rövid felsorolásban néhány területet és célgépet említenék meg, ahol sikerrel tudtuk használni az elkészült gépet:
1. játék konzolok, Xbox, PS3, SEGA, Nintendo, Arcade, Neo Geo és még sok más játék
2. nehéz gépek, targonca vezérlés, hasznon gépjárművek vezérlése
3. krumpli válogató gyártosori vezérlés
4. gyermekorvosi szív ultrahang és normál ultrahang gépek javítása
5. PC VGA kártyák javítása
6. Személyautók vezérlése
7. Hordozható számítógépek
8. és további BGA vagy SMD, esetenként diszkrét alaktarészekkel szerelt elektronikai berendezések javítása.
