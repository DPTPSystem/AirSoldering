/*********************************************
+		Külsõ fájlok beemelése
**********************************************/
#include <p18F4550.h>			//PIC beálltása
#include <adc.h>				//Analog függvénykönyvtár
#include <stdio.h>
#include <delays.h>				//Késleltetések
#include <stdlib.h>
#include <timers.h>				//Timer-ek
#include <pwm.h>				//PWM fügvénykönyvtár
#include <spi.h>				//SPI fügvénykönyvtár
#include "header.h"				//Álltalános header beállítások
#include "p18f4550_pindef.h"	//P18F4550 saját definicióim
#include "lcd.c"				//LDC meghajtás függvényei

/*********************************************
+		Definciciók deklarálása
**********************************************/
//-------------------------- Definciciók ----------------------------//
//PIC18F4550 SPI lábkiosztása
//7. (RA5)[SS - SPI másodlagos input választó]
//33.(RBO)[SDI - SPI adat bemenet]
//34.(RB1)[SCK - szinkron óra be/ki SPI módban]
//26.(RC7)[SDO - SPI adat kimenet]
#define SPI_CS_TRIS		TRIS_A5		//A5 chip választás
#define SPI_CS			LAT_A5		//A5 chip választás
#define SPI_SO_TRIS		TRIS_B0		//B0-ás bit
#define SPI_SCK_TRIS	TRIS_B1		//B1-es bit órajel
#define SPI_SI			TRIS_C7		//C7 adat ki
//-- A 25LC640 EEPROM által elfogadott parancsok
//----------- Parancsok 25LC460 memóriához -----------------------//
//Parancs	Bináris		Hexa		Lerás
//READ 		0000 0011 	0x03		A memória olvasása adott címtõl kezdõdõen
//WRITE 	0000 0010 	0x02		A memória írása adott címtõl kezdõdõen
//WREN 		0000 0110 	0x06		Az írást engedélyezõ bit törlése (letiltja az írást)
//WRDI 		0000 0100 	0x04		Az írást engedélyezõ bit beállítása (engedélyezi az írást)
//RDSR 		0000 0101 	0x05		STATUS register olvasása
//WRSR 		0000 0001 	0x01		STATUS register írása
#define CMD_WRSR  		0x01		//Státuszregiszter írása
#define CMD_WRITE 		0x02		//Írás a megadott címtõl kezdõdõen
#define CMD_READ  		0x03		//Olvasás a megadott címtõl kezdõdõen
#define CMD_WRDI  		0x04		//Letiltja az írást
#define CMD_RDSR  		0x05		//Státuszregiszter olvasása
#define CMD_WREN  		0x06		//Engedélyezi az írást
/*Memória blokk kiszámolása: 	1 bájt a sor érvényesítõ, (#)
								17 bájt a név,			(min 3 - max 17)
							 	6 bájt a hõprofil,		(1-255)
								6 bájt az idõprofil,	(1-255)
								2 bájt az alsó fûtés	(1-1023)*/
#define PCHARSIZE	18			//18 bájt profil unsigned char buffer (8bit)
#define PINTSIZE	13			//14 bájt profil short int buffer (16bit)
#define PMEMBLOCK	32			//Profil memória block (32bájt)
#define PPROGSZAM	15			//Megengedett profilok száma (32bájt x 15 == 480bájt)
//------------------------------ 4x4 MATRIX -----------------------------------//
#define SOROK 			LATB 		//Sorok 4x4 matrix
#define oszlop1 		PORT_B4 	//Oszlopok bemenetek 4x4 matrix
#define oszlop2 		PORT_B5		//-"-
#define oszlop3 		PORT_B6		//-"-
#define oszlop4 		PORT_B7		//-"-

/*********************************************
+		Külsõ fájlok beemelése 2 (ide kerülnek azon fájlok melyek definiciókat használnak)
**********************************************/
#include "init.c"				//Fontosabb függvények egy csoportba
#include "fuggvenyek.c"			//Függvény könyvtár
#include "interrupt.c"			//Megszaktások függvényei

/*********************************************
+		Globális változók deklarálása
**********************************************/
//----------- Profil adatk tárolása -----------------// 
unsigned char 	ERVPROFCHAR[PCHARSIZE] = {0};	//8bites adatok (#név)
short int 		ERVPROFINT[PINTSIZE] = {0};		//16bites adatok
//----------- 4x4 MATRIX változók -----------------//
unsigned char keyval;	

void main(void){
	
	/*********************************************
	+		Változók deklarálása
	**********************************************/	
	union16 MemCim, ReadCim, AlsoFutes;					//16bit-es struct változók
	unsigned char pmemblockbuffer[PMEMBLOCK] = {0}; 	//Profil blokk buffer
	unsigned char EepromTrue = 0;						//Eeprom vizsgálatához
	unsigned char i, kitoltes;							//i nõvekszik, kitoltos : profilnevet üres hellyel tölti ki ha az kevesebb mint 18karakter
	unsigned char KeyAlfOrNum = 0;						//Alfanumerikus vagy Numerikus gombok
	unsigned int ciklus = 0, x;							//cilus perges, számoló

	init();
 
	printf("DPTP SYSTEM 14' 5 01");
	lcd_write(0xC0,0,1,1);   
	printf("   F27SZERVIZ.HU    ");
	lcd_write(0x94,0,1,1);
	printf("FORRASZTO ALLOMAS   ");
	lcd_write(0xD4,0,1,1);
	printf("VERZIO SZAM: 1.0    ");
	lcd_write(0x80,0,1,1); 
	Delay_ms(600*5);	//kb ~5mp
	
	lcd_clear();	//Töröljük az LCD kijelzõt

	//Beolvassuk az Eeprom elsõ 1 bájt-ját ami tárolja az utolsó elmentett profil számát (1-15)
	if((int)EepromRead()>0 && (int)EepromRead()<=PPROGSZAM){
		//Ha érvényes adatot kaptunk tovább léphetünk
		ReadCim.value = PMEMBLOCK*(int)EepromRead()-PMEMBLOCK;	//Az Eepromban elmentett címzést beállítjuk (32bájt*0-14)
		memRead25LC640(ReadCim, pmemblockbuffer);				//kiolvassuk a memóriából az eeproba tárolt profil azonosítót
		//Megvizsgáljuk, hogy van e érvényesített adat a címen
		if(pmemblockbuffer[0]=='#'){
			//Ha van érvényes adat
			printf("Utolso elmentett    ");
			lcd_write(0xC0,0,1,1);   
			printf("profil betoltese... ");
			lcd_write(0x94,0,1,1);
			//Név letárolása
			kitoltes=1;	//Ha nincs rá szükség
			for(i=0; i<=PCHARSIZE; i++){
				if(pmemblockbuffer[i]!= '\0'){
					if(kitoltes){
						ERVPROFCHAR[i] = pmemblockbuffer[i];
					}else{
						ERVPROFCHAR[i] = ' ';
					}
				}else{
					kitoltes = 0;
					ERVPROFCHAR[i] = ' ';
				}
				if(i==PCHARSIZE) ERVPROFCHAR[i] = '\0';
			}
			//Hõprofil és idõk letárolása
			for(i=1; i<PINTSIZE; i++){
				ERVPROFINT[i-1] = pmemblockbuffer[PCHARSIZE+i];
			}
			//Alsó fûtés 2bájtjának összefûzése és letárolása
			AlsoFutes.value = 0;	//Nullázzuk az adatstruktúrát
			i = PCHARSIZE+i;
			AlsoFutes.low = pmemblockbuffer[i+1];
			AlsoFutes.high = pmemblockbuffer[i+2];
			ERVPROFINT[PINTSIZE] = AlsoFutes.value;	//Az összevont 16bites adatot beletõltjuk a változónkba
			memset(pmemblockbuffer, 0, sizeof(pmemblockbuffer));
			AlsoFutes.value = 0;
			EepromTrue=1;	//Van érvényes profil elmentve és betöltve a bufferbe
		}else{
			//Ha nincs érvényes adat a memória címen hibával térünk vissza
			printf("Hiba: Eeprom, meg   ");
			lcd_write(0xC0,0,1,1);   
			printf("nincs v. hibas adat!");
			lcd_write(0x94,0,1,1);
			memset(pmemblockbuffer, 0, sizeof(pmemblockbuffer));	//Töröljük a hibás profil buffert
			ReadCim.value = 0;	//Nullázzuk a memória címzést
			EepromTrue=0;		//Nincs érvényes Eeprom elmentés
		}
	}else{
		//Ha még nem történ mentés az Eepromba (elsõ indítás vagy hiba esetén fórdul elõ)
		printf("Hiba: Eeprom, nincs ");
		lcd_write(0xC0,0,1,1);   
		printf("ertekelheto adat!   ");
		lcd_write(0x94,0,1,1);
		EepromTrue=0;	//Nincs érvényes Eeprom mentés
	}
	printf("                    ");
	lcd_write(0xD4,0,1,1);
	printf("                    ");
	lcd_write(0x80,0,1,1);
	Delay_ms(600*3);	//Várunk ~3mp-et
	
	profil_betoltese:
	i=0, x=0;
	while(1){

		if(EepromTrue==1){
			//Ha van érvényes profil betöltve akkor kiírjuk az LCD-re és indítható egybõl
			keyval = get_key(KeyAlfOrNum);	//0 vagy is numerikus gombok engedélyezése
			lcd_clear();
			//ERVPROFCHAR név tároló adat tömb
			printf("%s", ERVPROFCHAR);
			lcd_write(0xC0,0,1,1);
   			
			if(ciklus%10==0){
				printf("%i. %i%c %imp       ", x, ERVPROFINT[i], 0xdf, ERVPROFINT[i+1]);
				if(i<7){
					i++; i++; x++;
				}else{
					i=0; x=0;
				}
			}else{
				printf("%i. %i%c %imp       ", x, ERVPROFINT[i], 0xdf, ERVPROFINT[i+1]);
			}

			lcd_write(0x94,0,1,1);

			printf("Prog inditasa: START");
			lcd_write(0xD4,0,1,1);
			printf("Tovabbi opcio: MENU  ");
			lcd_write(0x80,0,1,1);
		}else if(EepromTrue==0){
			//Nincs érvényes profil betöltve, ugrunk a menüre (elsõ indúlás és hiba esetén)
		}
		Delay10KTCYx(100);
	
	ciklus++;
	}//while
	

}//main
