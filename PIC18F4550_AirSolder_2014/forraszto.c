/*********************************************
+		PIC18F4550 Lábkiosztás
+		DPTP SYSTEM 2014-05-16
+		TÓTH PÉTER (don_peter@freemail.hu)
**********************************************
		A PORT
A0	-->		4x4 matrik sorok 1 (kimenet)
A1	-->		4x4 matrik sorok 2 (kimenet)
A2	-->		4x4 matrik sorok 3 (kimenet)
A3	-->		4x4 matrik sorok 4 (kimenet)
A4	-->		NC
A5	-->		NC
A6	-->		Kristály
A7	-->		Kristály

		B PORT
B0	-->		SPI SO
B1	-->		SPI SCK
B2	-->		Memória ChipSelect (25LC640)
B3	-->		NC
B4	-->		4x4 matrik sorok 1 (bemenet)
B5	-->		4x4 matrik sorok 2 (bemenet)
B6	-->		4x4 matrik sorok 3 (bemenet)
B7	-->		4x4 matrik sorok 4 (bemenet)

		C PORT
C0	-->		Alsó fûtés relé
C1	-->		PWM 2
C2	-->		PWM 1 PS3 ventillátor meghajtás
C3	-->		VUSB (nem használható)
C4	-->		USB D- (nem használható)
C5	-->		USB D+ (nem használható)
C6	-->		Felsõ fûtés relé
C7	-->		SPI SDO

		D PORT
D0	-->		LCD1
D1	-->		LCD2
D2	-->		LCD3
D3	-->		NC
D4	-->		LCD4
D5	-->		LCD5
D6	-->		LCD6
D7	-->		LCD7

		E PORT
E0	-->		MAX6675 1 ChipSelect
E1	-->		MAX6675 2 ChipSelect
E2	-->		MAX6675 3 ChipSelect
E3	-->		VDD 5v

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
#define SPI_CS_TRIS		TRIS_B2		//B2 chip választás (Memória ChipSelect)
#define SPI_CS			LAT_B2		//B2 chip választás (Memória ChipSelect)
#define SPI_SO_TRIS		TRIS_B0		//B0-ás bit
#define SPI_SCK_TRIS	TRIS_B1		//B1-es bit órajel
#define SPI_SI			TRIS_C7		//C7 adat ki
//--	MAX 6675 --//
/***************************************************************************************
PIC18F4550 SPI lábkiosztása MAX6675-ös chip-re
7. (RE0:RE2)[SS - SPI másodlagos input választó]	->	MAX6675 6.láb (CS) [Chip választás]
33.(RBO)[SDI - SPI adat bemenet]					->	MAX6675 7.láb (SO) [Adat kimenet]
34.(RB1)[SCK - szinkor óra be/ki SPI módban]		->	MAX6675 5.láb (SCK)[Óra bemenet]
26.(RC7)[SDO - SPI adat kimenet]
*****************************************************************************************/
#define MAX1CS			LAT_E0		//MAX6675 1. chipselect láb
#define MAX2CS			LAT_E1		//MAX6675 2. chipselect láb (Felsõ fûtést mérõ pont)
#define MAX3CS			LAT_E2		//MAX6675 3. chipselect láb
//----------- Fûtõbetétek vezérlõlábai -----------------------//
#define AlsoFutesRele	LAT_C0		//Alsó fûtés relét vezérlõ láb (C0)
#define FelsoFutesRele	LAT_D3		//Felsõ fûtés relét vezérlõ láb (C6)
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
#define SOROK 			LATA 		//Sorok 4x4 matrix
#define oszlop1 		PORT_B4 	//Oszlopok bemenetek 4x4 matrix
#define oszlop2 		PORT_B5		//-"-
#define oszlop3 		PORT_B6		//-"-
#define oszlop4 		PORT_B7		//-"-
#define KeyNumeric		0			//Csak numerikus gombok engedélyezése
#define KeyAlfanum		1			//Alfanumerikus gombok engedélyezése
#define Cursors			20			//Kursor villogásának gyakorisága
#define AtlagTMeret		4			//Átlagoló buffer tömb mérete

/*********************************************
+		Globális változók deklarálása
**********************************************/
//----------- Profil adatok tárolása -----------------// 
unsigned char 	ERVPROFCHAR[PCHARSIZE+1] = {0};		//8bites adatok (#név)
short int 		ERVPROFINT[PINTSIZE+1] = {0};		//16bites adatok
//----------- Program futása közbeni adatok ------------//
unsigned char Tido;									//timer0 idõ méréséhez
unsigned char Hofoklepes;							//Hõfokot léptetjük globálisan a timer0 miatt
short int	  HoBufferTomb[AtlagTMeret];			//Hõmérsékleteket fogja tárolni az átlagoláshoz
unsigned char AtlagBTCount = 0;						//Átlagoló tömb számoló
//----------- 4x4 MATRIX változók -----------------//
unsigned char Navigacio;							//Navigacios változó (0-9)

/*********************************************
+		Külsõ fájlok beemelése 2 (ide kerülnek azon fájlok melyek definiciókat vagy globális változókat használnak)
**********************************************/
#include "init.c"				//Fontosabb függvények egy csoportba
#include "fuggvenyek.c"			//Függvény könyvtár
#include "interrupt.c"			//Megszaktások függvényei

void main(void){
	
	/*********************************************
	+		Változók deklarálása
	**********************************************/	
	union16 MemCim, ReadCim, AlsoFutes;			//16bit-es struct változók
	unsigned char Membuffer[PMEMBLOCK+1] = {0}; //Profil blokk buffer
	
	//Teszthez
	//short int ciklus=0;

	init();
	
	printf("DPTP SYSTEM 14' 5 01");
	lcd_write(0xC0,0,1,1);   
	printf("   F27SZERVIZ.HU    ");
	lcd_write(0x94,0,1,1);
	printf(" FORRASZTO ALLOMAS  ");
	lcd_write(0xD4,0,1,1);
	printf("  VERZIO SZAM: 1.0  ");
	lcd_write(0x80,0,1,1); 
	Delay_ms(600*5);	//kb ~5mp
	
	lcd_clear();	//Töröljük az LCD kijelzõt
		
	//Beolvassuk az Eeprom elsõ 1 bájt-ját ami tárolja az utolsó elmentett profil számát (1-15)
	if((int)EepromRead()>0 && (int)EepromRead()<=PPROGSZAM){
		if(profilok_szama(EepromRead())){
			profilt_betolt(EepromRead());	//Betöltjük az elmentett profilt
			
			printf("Utolso elmentett    ");
			lcd_write(0xC0,0,1,1);   
			printf("profil betoltese... ");
			lcd_write(0x94,0,1,1);
			printf("Profil: [%i]        ", EepromRead());
			lcd_write(0xD4,0,1,1);
			printf("Ossz profil: [%i]   ", profilok_szama(0));
			lcd_write(0x80,0,1,1);
			Delay_ms(600*2);	//Várunk ~2mp-et
		
			Navigacio=1;
		}else{
			//Ha nincs érvényes adat a memória címen hibával térünk vissza
			printf("Hiba: Memoria, meg  ");
			lcd_write(0xC0,0,1,1);   
			printf("nincs v. hibas adat!");
			lcd_write(0x94,0,1,1);
			printf("                    ");
			lcd_write(0xD4,0,1,1);
			printf("                    ");
			lcd_write(0x80,0,1,1);
			Delay_ms(600*2);	//Várunk ~2mp-et
			
			Navigacio=0;
		}
	}else{
		//Ha még nem történ mentés az Eepromba (elsõ indítás vagy hiba esetén fórdul elõ)
		printf("Hiba: Eeprom, nincs ");
		lcd_write(0xC0,0,1,1);   
		printf("ertekelheto adat!   ");
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("                    ");
		lcd_write(0x80,0,1,1);
		Delay_ms(600*2);	//Várunk ~2mp-et

		Navigacio=0;
	}
	while(1){
		if(Navigacio==1){	//Profil betöltve == 1
			//if(profilok_szama(EepromRead())!=0){
				profilt_mutat();
			/*}else{
				printf("Hiba: Nincs profil  ");
				lcd_write(0xC0,0,1,1);   
				printf("betoltve!           ");
				lcd_write(0x94,0,1,1);
				printf("                    ");
				lcd_write(0xD4,0,1,1);
				printf("                    ");
				lcd_write(0x80,0,1,1);
				Delay_ms(600*2);

				Navigacio=0;
			}*/
		}else if(Navigacio==0){	//Menü == 0
			menu();
		}else if(Navigacio==2){	//Új profil felvétele
			if(profilok_szama(0)<PPROGSZAM){
				uj_profil();
			}else{
				printf("Hiba: Memoria       ");
				lcd_write(0xC0,0,1,1);   
				printf("megtelt. Max 15.    ");
				lcd_write(0x94,0,1,1);
				printf("                    ");
				lcd_write(0xD4,0,1,1);
				printf("                    ");
				lcd_write(0x80,0,1,1);
				Delay_ms(600*2);
			}
		}else if(Navigacio==3){	//Profil szerkesztése
			profil_szerk();
		}else if(Navigacio==4){	//Profilok listája
			profil_lista();
		}else if(Navigacio==5){	//Start
			start();
		}else if(Navigacio==6){	//Stop
			stop();
		}

	}//while()
	
	/*while(1){
		printf("K-type Sensor       ");
		lcd_write(0xC0,0,1,1);   
		printf("%i                  ", Max6675(2));
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("Ciklus: %i          ", ciklus);
		lcd_write(0x80,0,1,1);
		Delay_ms(300);
	ciklus++;
	}*/

}//main

/***********************************
+			Hõfok teszt			   +
***********************************/
/*
Víz:			Forrasztó páka		SMD Forrasztó		Forrasztó páka	-	KID-R480A
0	==	0
				50	==	84								50					89
100	==	100		100	==	134			100	==	130			100					144
				150	==	185			150	==	185			150					190
				200	==	224			200	==	234			200					237
				250	==	264			250	==	284			250					282
				300	==	300			300	==	335			300					324
				350	==	337			350	==	384			350					360
				400	==	374			400	==	444			400					400
				422	==	384			450	==	504			422					---
									480	==	534
*/
