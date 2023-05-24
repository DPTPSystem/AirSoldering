/************************
+ SMD DPTP SOLDERING
+ don_peter@freemail.hu
+ 2014-11-06 - Budapest
************************/

/*********************************************
+		Külsõ fájlok beemelése
**********************************************/
#include <p18F452.h>			//PIC beálltása
#include <stdio.h>
#include <delays.h>				//Késleltetések
#include <stdlib.h>
#include <timers.h>				//Timer-ek
#include <pwm.h>				//PWM fügvénykönyvtár
#include <spi.h>				//SPI fügvénykönyvtár
#include "header.h"				//Álltalános header beállítások
#include "lcd.c"				//LDC meghajtás függvényei

/*********************************************
+		Definciciók deklarálása
**********************************************/

#define SPI_CS_TRIS		TRISCbits.TRISC1	//C1 chip választás (Memória ChipSelect)
#define SPI_CS			LATCbits.LATC1		//C1 chip választás (Memória ChipSelect)
#define SPI_SO_TRIS		TRISCbits.TRISC5	//C5-ás bit
#define SPI_SCK_TRIS	TRISCbits.TRISC3	//C3-es bit órajel
#define SPI_SI			TRISCbits.TRISC4	//C4 adat ki
//----------- Max6675 vezérlõlábai -----------------------//
#define MAX1CS			LATCbits.LATC0		// MAX6675 ChipSelect láb
//----------- ShifRegiszter vezérlõlábai -----------------------//
#define SRCLK			LATBbits.LATB6
#define	SRCS			LATBbits.LATB5
#define	SRADAT			LATBbits.LATB4
#define SBIT(x){SRCS=0; SRADAT=x; SRCLK=1; SRCLK=0; SRCS=1;}
//----------- Power led vezérlõ láb -----------------------//
#define LED_P			LATDbits.LATD3		// Piros LED
//----------- BUZZER vezérlõ láb -----------------------//
#define BUZZER			LATBbits.LATB7		//Buzzer (csipogó)
//----------- Fûtõbetétek vezérlõlábai -----------------------//
#define FelsoFutesRele	LATAbits.LATA0		//Felsõ fûtés relét vezérlõ láb (A0)
//-- A 25LC640 EEPROM által elfogadott parancsok
//----------- Parancsok 25LC460 memóriához -----------------------//
#define CMD_WRSR  		0x01		//Státuszregiszter írása
#define CMD_WRITE 		0x02		//Írás a megadott címtõl kezdõdõen
#define CMD_READ  		0x03		//Olvasás a megadott címtõl kezdõdõen
#define CMD_WRDI  		0x04		//Letiltja az írást
#define CMD_RDSR  		0x05		//Státuszregiszter olvasása
#define CMD_WREN  		0x06		//Engedélyezi az írást
/*Memória blokk kiszámolása: 		1 bájt a sor érvényesítõ, (#)
									14 bájt a név,			(min 3 - max 14)
							 		12 bájt a hõprofil,		(1-500)
									6 bájt az idõprofil,	(1-255)*/
#define PCHARSIZE	14				//18 bájt profil unsigned char buffer (8bit)
#define PINTSIZE	12				//18 bájt profil short int buffer (16bit)
#define PMEMBLOCK	32				//Profil memória block (32bájt)
#define PPROGSZAM	15				//Megengedett profilok száma (32bájt x 15 == 480bájt)
//------------------------------ 4x4 MATRIX -----------------------------------//
//#define SOROK 			LATA 		//Sorok 4x4 matrix
#define oszlop1 		PORTBbits.RB0 	//Oszlopok bemenetek 4x4 matrix
#define oszlop2 		PORTBbits.RB1	//-"-
#define oszlop3 		PORTBbits.RB2	//-"-
#define oszlop4 		PORTBbits.RB3	//-"-
#define KeyNumeric		0			//Csak numerikus gombok engedélyezése
#define KeyAlfanum		1			//Alfanumerikus gombok engedélyezése
#define Cursors			10			//Kursor villogásának gyakorisága
#define WhileTime		20			//While ciklus azonos idõzítése (miliszekondumban)
#define AtlagTMeret		4			//Átlagoló buffer tömb mérete
/*********************************************
+		Globális változók deklarálása
**********************************************/
//----------- Profil adatok tárolása -----------------// 
unsigned char 	ERVPROFCHAR[PCHARSIZE+1] = {0};				//8bites adatok (#név)
short int 		ERVPROFINT[PINTSIZE+1] = {0};				//16bites adatok
//----------- Program futása közbeni adatok ------------//
unsigned char Tido;											//timer0 idõ méréséhez
unsigned char Timer0Count;									//timer0 számolása
unsigned char Hofoklepes;									//Hõfokot léptetjük globálisan a timer0 miatt
unsigned short int HoBufferTomb[AtlagTMeret+1] = {0};	//Hõmérsékleteket fogja tárolni az átlagoláshoz
unsigned char AtlagBTCount = 0;								//Átlagoló tömb számoló
//----------- 4x4 MATRIX változók -----------------//
unsigned char Navigacio;									//Navigacios változó (0-9)

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
	union16 MemCim, ReadCim;					//16bit-es struct változók
	unsigned char Membuffer[PMEMBLOCK+1] = {0}; //Profil blokk buffer
	
	unsigned char ciklus = 0;
	
	init();
	
	/*while(1){
		printf("K-type Sensor       ");
		lcd_write(0xC0,0,1,1);   
		printf("%i                  ", Max6675());
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("Ciklus: %i          ", ciklus);
		lcd_write(0x80,0,1,1);
		LED_P = !LED_P;
		Delay_ms(255);
	ciklus++;
	}*/


	RomStringWrite(szovegek[14]);
	lcd_write(0xC0,0,1,1);   
	RomStringWrite(szovegek[15]);
	lcd_write(0x94,0,1,1);
	RomStringWrite(szovegek[16]);
	lcd_write(0xD4,0,1,1);
	RomStringWrite(szovegek[17]);
	lcd_write(0x80,0,1,1); 
	Buzzer(1, 100);
	Delay_ms(1000*5);	//kb ~5mp
	
	lcd_clear();	//Töröljük az LCD kijelzõt
		
	//Beolvassuk az Eeprom elsõ 1 bájt-ját ami tárolja az utolsó elmentett profil számát (1-15)
	if((int)EepromRead()>0 && (int)EepromRead()<=PPROGSZAM){
		if(profilok_szama(EepromRead())){
			profilt_betolt(EepromRead());	//Betöltjük az elmentett profilt
			
			RomStringWrite(szovegek[18]);
			lcd_write(0xC0,0,1,1);   
			RomStringWrite(szovegek[19]);
			lcd_write(0x94,0,1,1);
			RomStringWrite(szovegek[20]);
			lcd_write(0xD4,0,1,1);
			betoltes();
			lcd_write(0x80,0,1,1);
			Delay_ms(500);	//Várunk ~2mp-et
		
			Navigacio=1;
		}else{
			//Ha nincs érvényes adat a memória címen hibával térünk vissza
			RomStringWrite(szovegek[21]);
			lcd_write(0xC0,0,1,1);   
			RomStringWrite(szovegek[22]);
			lcd_write(0x94,0,1,1);
			RomStringWrite(szovegek[20]);
			lcd_write(0xD4,0,1,1);
			RomStringWrite(szovegek[20]);
			lcd_write(0x80,0,1,1);
			Delay_ms(1000*2);	//Várunk ~2mp-et
			
			Navigacio=0;
		}
	}else{
		//Ha még nem történ mentés az Eepromba (elsõ indítás vagy hiba esetén fórdul elõ)
		RomStringWrite(szovegek[23]);
		lcd_write(0xC0,0,1,1);   
		RomStringWrite(szovegek[24]);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0x80,0,1,1);
		Delay_ms(1000*2);	//Várunk ~2mp-et

		Navigacio=0;
	}
	while(1){
		if(Navigacio==1){	//Profil betöltve == 1
			if(profilok_szama(EepromRead())!=0){
				profilt_mutat();
			}else{
				RomStringWrite(szovegek[25]);
				lcd_write(0xC0,0,1,1);   
				RomStringWrite(szovegek[26]);
				lcd_write(0x94,0,1,1);
				RomStringWrite(szovegek[20]);
				lcd_write(0xD4,0,1,1);
				RomStringWrite(szovegek[20]);
				lcd_write(0x80,0,1,1);
				Delay_ms(1000*2);

				Navigacio=0;
			}
		}else if(Navigacio==0){	//Menü == 0
			LED_P = 0;	//Piros LED tiltása
			HighInterruptIni();	// Magas piorítású interrupt engedélyezése
			menu();
		}else if(Navigacio==2){	//Új profil felvétele
			if(profilok_szama(0)<PPROGSZAM){
				uj_profil(0);
			}else{
				RomStringWrite(szovegek[27]);
				lcd_write(0xC0,0,1,1);   
				RomStringWrite(szovegek[28]);
				lcd_write(0x94,0,1,1);
				RomStringWrite(szovegek[20]);
				lcd_write(0xD4,0,1,1);
				RomStringWrite(szovegek[20]);
				lcd_write(0x80,0,1,1);
				Delay_ms(1000*2);
			}
		}else if(Navigacio==3){	//Profil szerkesztése
			uj_profil(1);
		}else if(Navigacio==4){	//Profilok listája
			profil_lista();
		}else if(Navigacio==5){	//Start
			start();
		}else if(Navigacio==6){	//Stop
			stop();
		}else if(Navigacio==7){	//SMD forrasztó
			SMD_forraszto();
		}
		
	}//while()

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
