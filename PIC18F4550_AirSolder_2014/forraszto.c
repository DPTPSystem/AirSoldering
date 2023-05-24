/*********************************************
+		PIC18F4550 L�bkioszt�s
+		DPTP SYSTEM 2014-05-16
+		T�TH P�TER (don_peter@freemail.hu)
**********************************************
		A PORT
A0	-->		4x4 matrik sorok 1 (kimenet)
A1	-->		4x4 matrik sorok 2 (kimenet)
A2	-->		4x4 matrik sorok 3 (kimenet)
A3	-->		4x4 matrik sorok 4 (kimenet)
A4	-->		NC
A5	-->		NC
A6	-->		Krist�ly
A7	-->		Krist�ly

		B PORT
B0	-->		SPI SO
B1	-->		SPI SCK
B2	-->		Mem�ria ChipSelect (25LC640)
B3	-->		NC
B4	-->		4x4 matrik sorok 1 (bemenet)
B5	-->		4x4 matrik sorok 2 (bemenet)
B6	-->		4x4 matrik sorok 3 (bemenet)
B7	-->		4x4 matrik sorok 4 (bemenet)

		C PORT
C0	-->		Als� f�t�s rel�
C1	-->		PWM 2
C2	-->		PWM 1 PS3 ventill�tor meghajt�s
C3	-->		VUSB (nem haszn�lhat�)
C4	-->		USB D- (nem haszn�lhat�)
C5	-->		USB D+ (nem haszn�lhat�)
C6	-->		Fels� f�t�s rel�
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
+		K�ls� f�jlok beemel�se
**********************************************/
#include <p18F4550.h>			//PIC be�llt�sa
#include <adc.h>				//Analog f�ggv�nyk�nyvt�r
#include <stdio.h>
#include <delays.h>				//K�sleltet�sek
#include <stdlib.h>
#include <timers.h>				//Timer-ek
#include <pwm.h>				//PWM f�gv�nyk�nyvt�r
#include <spi.h>				//SPI f�gv�nyk�nyvt�r
#include "header.h"				//�lltal�nos header be�ll�t�sok
#include "p18f4550_pindef.h"	//P18F4550 saj�t definici�im
#include "lcd.c"				//LDC meghajt�s f�ggv�nyei

/*********************************************
+		Defincici�k deklar�l�sa
**********************************************/
//-------------------------- Defincici�k ----------------------------//
//PIC18F4550 SPI l�bkioszt�sa
//7. (RA5)[SS - SPI m�sodlagos input v�laszt�]
//33.(RBO)[SDI - SPI adat bemenet]
//34.(RB1)[SCK - szinkron �ra be/ki SPI m�dban]
//26.(RC7)[SDO - SPI adat kimenet]
#define SPI_CS_TRIS		TRIS_B2		//B2 chip v�laszt�s (Mem�ria ChipSelect)
#define SPI_CS			LAT_B2		//B2 chip v�laszt�s (Mem�ria ChipSelect)
#define SPI_SO_TRIS		TRIS_B0		//B0-�s bit
#define SPI_SCK_TRIS	TRIS_B1		//B1-es bit �rajel
#define SPI_SI			TRIS_C7		//C7 adat ki
//--	MAX 6675 --//
/***************************************************************************************
PIC18F4550 SPI l�bkioszt�sa MAX6675-�s chip-re
7. (RE0:RE2)[SS - SPI m�sodlagos input v�laszt�]	->	MAX6675 6.l�b (CS) [Chip v�laszt�s]
33.(RBO)[SDI - SPI adat bemenet]					->	MAX6675 7.l�b (SO) [Adat kimenet]
34.(RB1)[SCK - szinkor �ra be/ki SPI m�dban]		->	MAX6675 5.l�b (SCK)[�ra bemenet]
26.(RC7)[SDO - SPI adat kimenet]
*****************************************************************************************/
#define MAX1CS			LAT_E0		//MAX6675 1. chipselect l�b
#define MAX2CS			LAT_E1		//MAX6675 2. chipselect l�b (Fels� f�t�st m�r� pont)
#define MAX3CS			LAT_E2		//MAX6675 3. chipselect l�b
//----------- F�t�bet�tek vez�rl�l�bai -----------------------//
#define AlsoFutesRele	LAT_C0		//Als� f�t�s rel�t vez�rl� l�b (C0)
#define FelsoFutesRele	LAT_D3		//Fels� f�t�s rel�t vez�rl� l�b (C6)
//-- A 25LC640 EEPROM �ltal elfogadott parancsok
//----------- Parancsok 25LC460 mem�ri�hoz -----------------------//
//Parancs	Bin�ris		Hexa		Ler�s
//READ 		0000 0011 	0x03		A mem�ria olvas�sa adott c�mt�l kezd�d�en
//WRITE 	0000 0010 	0x02		A mem�ria �r�sa adott c�mt�l kezd�d�en
//WREN 		0000 0110 	0x06		Az �r�st enged�lyez� bit t�rl�se (letiltja az �r�st)
//WRDI 		0000 0100 	0x04		Az �r�st enged�lyez� bit be�ll�t�sa (enged�lyezi az �r�st)
//RDSR 		0000 0101 	0x05		STATUS register olvas�sa
//WRSR 		0000 0001 	0x01		STATUS register �r�sa
#define CMD_WRSR  		0x01		//St�tuszregiszter �r�sa
#define CMD_WRITE 		0x02		//�r�s a megadott c�mt�l kezd�d�en
#define CMD_READ  		0x03		//Olvas�s a megadott c�mt�l kezd�d�en
#define CMD_WRDI  		0x04		//Letiltja az �r�st
#define CMD_RDSR  		0x05		//St�tuszregiszter olvas�sa
#define CMD_WREN  		0x06		//Enged�lyezi az �r�st
/*Mem�ria blokk kisz�mol�sa: 	1 b�jt a sor �rv�nyes�t�, (#)
								17 b�jt a n�v,			(min 3 - max 17)
							 	6 b�jt a h�profil,		(1-255)
								6 b�jt az id�profil,	(1-255)
								2 b�jt az als� f�t�s	(1-1023)*/
#define PCHARSIZE	18			//18 b�jt profil unsigned char buffer (8bit)
#define PINTSIZE	13			//14 b�jt profil short int buffer (16bit)
#define PMEMBLOCK	32			//Profil mem�ria block (32b�jt)
#define PPROGSZAM	15			//Megengedett profilok sz�ma (32b�jt x 15 == 480b�jt)
//------------------------------ 4x4 MATRIX -----------------------------------//
#define SOROK 			LATA 		//Sorok 4x4 matrix
#define oszlop1 		PORT_B4 	//Oszlopok bemenetek 4x4 matrix
#define oszlop2 		PORT_B5		//-"-
#define oszlop3 		PORT_B6		//-"-
#define oszlop4 		PORT_B7		//-"-
#define KeyNumeric		0			//Csak numerikus gombok enged�lyez�se
#define KeyAlfanum		1			//Alfanumerikus gombok enged�lyez�se
#define Cursors			20			//Kursor villog�s�nak gyakoris�ga
#define AtlagTMeret		4			//�tlagol� buffer t�mb m�rete

/*********************************************
+		Glob�lis v�ltoz�k deklar�l�sa
**********************************************/
//----------- Profil adatok t�rol�sa -----------------// 
unsigned char 	ERVPROFCHAR[PCHARSIZE+1] = {0};		//8bites adatok (#n�v)
short int 		ERVPROFINT[PINTSIZE+1] = {0};		//16bites adatok
//----------- Program fut�sa k�zbeni adatok ------------//
unsigned char Tido;									//timer0 id� m�r�s�hez
unsigned char Hofoklepes;							//H�fokot l�ptetj�k glob�lisan a timer0 miatt
short int	  HoBufferTomb[AtlagTMeret];			//H�m�rs�kleteket fogja t�rolni az �tlagol�shoz
unsigned char AtlagBTCount = 0;						//�tlagol� t�mb sz�mol�
//----------- 4x4 MATRIX v�ltoz�k -----------------//
unsigned char Navigacio;							//Navigacios v�ltoz� (0-9)

/*********************************************
+		K�ls� f�jlok beemel�se 2 (ide ker�lnek azon f�jlok melyek definici�kat vagy glob�lis v�ltoz�kat haszn�lnak)
**********************************************/
#include "init.c"				//Fontosabb f�ggv�nyek egy csoportba
#include "fuggvenyek.c"			//F�ggv�ny k�nyvt�r
#include "interrupt.c"			//Megszakt�sok f�ggv�nyei

void main(void){
	
	/*********************************************
	+		V�ltoz�k deklar�l�sa
	**********************************************/	
	union16 MemCim, ReadCim, AlsoFutes;			//16bit-es struct v�ltoz�k
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
	
	lcd_clear();	//T�r�lj�k az LCD kijelz�t
		
	//Beolvassuk az Eeprom els� 1 b�jt-j�t ami t�rolja az utols� elmentett profil sz�m�t (1-15)
	if((int)EepromRead()>0 && (int)EepromRead()<=PPROGSZAM){
		if(profilok_szama(EepromRead())){
			profilt_betolt(EepromRead());	//Bet�ltj�k az elmentett profilt
			
			printf("Utolso elmentett    ");
			lcd_write(0xC0,0,1,1);   
			printf("profil betoltese... ");
			lcd_write(0x94,0,1,1);
			printf("Profil: [%i]        ", EepromRead());
			lcd_write(0xD4,0,1,1);
			printf("Ossz profil: [%i]   ", profilok_szama(0));
			lcd_write(0x80,0,1,1);
			Delay_ms(600*2);	//V�runk ~2mp-et
		
			Navigacio=1;
		}else{
			//Ha nincs �rv�nyes adat a mem�ria c�men hib�val t�r�nk vissza
			printf("Hiba: Memoria, meg  ");
			lcd_write(0xC0,0,1,1);   
			printf("nincs v. hibas adat!");
			lcd_write(0x94,0,1,1);
			printf("                    ");
			lcd_write(0xD4,0,1,1);
			printf("                    ");
			lcd_write(0x80,0,1,1);
			Delay_ms(600*2);	//V�runk ~2mp-et
			
			Navigacio=0;
		}
	}else{
		//Ha m�g nem t�rt�n ment�s az Eepromba (els� ind�t�s vagy hiba eset�n f�rdul el�)
		printf("Hiba: Eeprom, nincs ");
		lcd_write(0xC0,0,1,1);   
		printf("ertekelheto adat!   ");
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("                    ");
		lcd_write(0x80,0,1,1);
		Delay_ms(600*2);	//V�runk ~2mp-et

		Navigacio=0;
	}
	while(1){
		if(Navigacio==1){	//Profil bet�ltve == 1
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
		}else if(Navigacio==0){	//Men� == 0
			menu();
		}else if(Navigacio==2){	//�j profil felv�tele
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
		}else if(Navigacio==3){	//Profil szerkeszt�se
			profil_szerk();
		}else if(Navigacio==4){	//Profilok list�ja
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
+			H�fok teszt			   +
***********************************/
/*
V�z:			Forraszt� p�ka		SMD Forraszt�		Forraszt� p�ka	-	KID-R480A
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
