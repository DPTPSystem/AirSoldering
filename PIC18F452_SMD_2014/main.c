/************************
+ SMD DPTP SOLDERING
+ don_peter@freemail.hu
+ 2014-11-06 - Budapest
************************/

/*********************************************
+		K�ls� f�jlok beemel�se
**********************************************/
#include <p18F452.h>			//PIC be�llt�sa
#include <stdio.h>
#include <delays.h>				//K�sleltet�sek
#include <stdlib.h>
#include <timers.h>				//Timer-ek
#include <pwm.h>				//PWM f�gv�nyk�nyvt�r
#include <spi.h>				//SPI f�gv�nyk�nyvt�r
#include "header.h"				//�lltal�nos header be�ll�t�sok
#include "lcd.c"				//LDC meghajt�s f�ggv�nyei

/*********************************************
+		Defincici�k deklar�l�sa
**********************************************/

#define SPI_CS_TRIS		TRISCbits.TRISC1	//C1 chip v�laszt�s (Mem�ria ChipSelect)
#define SPI_CS			LATCbits.LATC1		//C1 chip v�laszt�s (Mem�ria ChipSelect)
#define SPI_SO_TRIS		TRISCbits.TRISC5	//C5-�s bit
#define SPI_SCK_TRIS	TRISCbits.TRISC3	//C3-es bit �rajel
#define SPI_SI			TRISCbits.TRISC4	//C4 adat ki
//----------- Max6675 vez�rl�l�bai -----------------------//
#define MAX1CS			LATCbits.LATC0		// MAX6675 ChipSelect l�b
//----------- ShifRegiszter vez�rl�l�bai -----------------------//
#define SRCLK			LATBbits.LATB6
#define	SRCS			LATBbits.LATB5
#define	SRADAT			LATBbits.LATB4
#define SBIT(x){SRCS=0; SRADAT=x; SRCLK=1; SRCLK=0; SRCS=1;}
//----------- Power led vez�rl� l�b -----------------------//
#define LED_P			LATDbits.LATD3		// Piros LED
//----------- BUZZER vez�rl� l�b -----------------------//
#define BUZZER			LATBbits.LATB7		//Buzzer (csipog�)
//----------- F�t�bet�tek vez�rl�l�bai -----------------------//
#define FelsoFutesRele	LATAbits.LATA0		//Fels� f�t�s rel�t vez�rl� l�b (A0)
//-- A 25LC640 EEPROM �ltal elfogadott parancsok
//----------- Parancsok 25LC460 mem�ri�hoz -----------------------//
#define CMD_WRSR  		0x01		//St�tuszregiszter �r�sa
#define CMD_WRITE 		0x02		//�r�s a megadott c�mt�l kezd�d�en
#define CMD_READ  		0x03		//Olvas�s a megadott c�mt�l kezd�d�en
#define CMD_WRDI  		0x04		//Letiltja az �r�st
#define CMD_RDSR  		0x05		//St�tuszregiszter olvas�sa
#define CMD_WREN  		0x06		//Enged�lyezi az �r�st
/*Mem�ria blokk kisz�mol�sa: 		1 b�jt a sor �rv�nyes�t�, (#)
									14 b�jt a n�v,			(min 3 - max 14)
							 		12 b�jt a h�profil,		(1-500)
									6 b�jt az id�profil,	(1-255)*/
#define PCHARSIZE	14				//18 b�jt profil unsigned char buffer (8bit)
#define PINTSIZE	12				//18 b�jt profil short int buffer (16bit)
#define PMEMBLOCK	32				//Profil mem�ria block (32b�jt)
#define PPROGSZAM	15				//Megengedett profilok sz�ma (32b�jt x 15 == 480b�jt)
//------------------------------ 4x4 MATRIX -----------------------------------//
//#define SOROK 			LATA 		//Sorok 4x4 matrix
#define oszlop1 		PORTBbits.RB0 	//Oszlopok bemenetek 4x4 matrix
#define oszlop2 		PORTBbits.RB1	//-"-
#define oszlop3 		PORTBbits.RB2	//-"-
#define oszlop4 		PORTBbits.RB3	//-"-
#define KeyNumeric		0			//Csak numerikus gombok enged�lyez�se
#define KeyAlfanum		1			//Alfanumerikus gombok enged�lyez�se
#define Cursors			10			//Kursor villog�s�nak gyakoris�ga
#define WhileTime		20			//While ciklus azonos id�z�t�se (miliszekondumban)
#define AtlagTMeret		4			//�tlagol� buffer t�mb m�rete
/*********************************************
+		Glob�lis v�ltoz�k deklar�l�sa
**********************************************/
//----------- Profil adatok t�rol�sa -----------------// 
unsigned char 	ERVPROFCHAR[PCHARSIZE+1] = {0};				//8bites adatok (#n�v)
short int 		ERVPROFINT[PINTSIZE+1] = {0};				//16bites adatok
//----------- Program fut�sa k�zbeni adatok ------------//
unsigned char Tido;											//timer0 id� m�r�s�hez
unsigned char Timer0Count;									//timer0 sz�mol�sa
unsigned char Hofoklepes;									//H�fokot l�ptetj�k glob�lisan a timer0 miatt
unsigned short int HoBufferTomb[AtlagTMeret+1] = {0};	//H�m�rs�kleteket fogja t�rolni az �tlagol�shoz
unsigned char AtlagBTCount = 0;								//�tlagol� t�mb sz�mol�
//----------- 4x4 MATRIX v�ltoz�k -----------------//
unsigned char Navigacio;									//Navigacios v�ltoz� (0-9)

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
	union16 MemCim, ReadCim;					//16bit-es struct v�ltoz�k
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
	
	lcd_clear();	//T�r�lj�k az LCD kijelz�t
		
	//Beolvassuk az Eeprom els� 1 b�jt-j�t ami t�rolja az utols� elmentett profil sz�m�t (1-15)
	if((int)EepromRead()>0 && (int)EepromRead()<=PPROGSZAM){
		if(profilok_szama(EepromRead())){
			profilt_betolt(EepromRead());	//Bet�ltj�k az elmentett profilt
			
			RomStringWrite(szovegek[18]);
			lcd_write(0xC0,0,1,1);   
			RomStringWrite(szovegek[19]);
			lcd_write(0x94,0,1,1);
			RomStringWrite(szovegek[20]);
			lcd_write(0xD4,0,1,1);
			betoltes();
			lcd_write(0x80,0,1,1);
			Delay_ms(500);	//V�runk ~2mp-et
		
			Navigacio=1;
		}else{
			//Ha nincs �rv�nyes adat a mem�ria c�men hib�val t�r�nk vissza
			RomStringWrite(szovegek[21]);
			lcd_write(0xC0,0,1,1);   
			RomStringWrite(szovegek[22]);
			lcd_write(0x94,0,1,1);
			RomStringWrite(szovegek[20]);
			lcd_write(0xD4,0,1,1);
			RomStringWrite(szovegek[20]);
			lcd_write(0x80,0,1,1);
			Delay_ms(1000*2);	//V�runk ~2mp-et
			
			Navigacio=0;
		}
	}else{
		//Ha m�g nem t�rt�n ment�s az Eepromba (els� ind�t�s vagy hiba eset�n f�rdul el�)
		RomStringWrite(szovegek[23]);
		lcd_write(0xC0,0,1,1);   
		RomStringWrite(szovegek[24]);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0x80,0,1,1);
		Delay_ms(1000*2);	//V�runk ~2mp-et

		Navigacio=0;
	}
	while(1){
		if(Navigacio==1){	//Profil bet�ltve == 1
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
		}else if(Navigacio==0){	//Men� == 0
			LED_P = 0;	//Piros LED tilt�sa
			HighInterruptIni();	// Magas pior�t�s� interrupt enged�lyez�se
			menu();
		}else if(Navigacio==2){	//�j profil felv�tele
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
		}else if(Navigacio==3){	//Profil szerkeszt�se
			uj_profil(1);
		}else if(Navigacio==4){	//Profilok list�ja
			profil_lista();
		}else if(Navigacio==5){	//Start
			start();
		}else if(Navigacio==6){	//Stop
			stop();
		}else if(Navigacio==7){	//SMD forraszt�
			SMD_forraszto();
		}
		
	}//while()

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
