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
#define SPI_CS_TRIS		TRIS_A5		//A5 chip v�laszt�s
#define SPI_CS			LAT_A5		//A5 chip v�laszt�s
#define SPI_SO_TRIS		TRIS_B0		//B0-�s bit
#define SPI_SCK_TRIS	TRIS_B1		//B1-es bit �rajel
#define SPI_SI			TRIS_C7		//C7 adat ki
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
#define SOROK 			LATB 		//Sorok 4x4 matrix
#define oszlop1 		PORT_B4 	//Oszlopok bemenetek 4x4 matrix
#define oszlop2 		PORT_B5		//-"-
#define oszlop3 		PORT_B6		//-"-
#define oszlop4 		PORT_B7		//-"-

/*********************************************
+		K�ls� f�jlok beemel�se 2 (ide ker�lnek azon f�jlok melyek definici�kat haszn�lnak)
**********************************************/
#include "init.c"				//Fontosabb f�ggv�nyek egy csoportba
#include "fuggvenyek.c"			//F�ggv�ny k�nyvt�r
#include "interrupt.c"			//Megszakt�sok f�ggv�nyei

/*********************************************
+		Glob�lis v�ltoz�k deklar�l�sa
**********************************************/
//----------- Profil adatk t�rol�sa -----------------// 
unsigned char 	ERVPROFCHAR[PCHARSIZE] = {0};	//8bites adatok (#n�v)
short int 		ERVPROFINT[PINTSIZE] = {0};		//16bites adatok
//----------- 4x4 MATRIX v�ltoz�k -----------------//
unsigned char keyval;	

void main(void){
	
	/*********************************************
	+		V�ltoz�k deklar�l�sa
	**********************************************/	
	union16 MemCim, ReadCim, AlsoFutes;					//16bit-es struct v�ltoz�k
	unsigned char pmemblockbuffer[PMEMBLOCK] = {0}; 	//Profil blokk buffer
	unsigned char EepromTrue = 0;						//Eeprom vizsg�lat�hoz
	unsigned char i, kitoltes;							//i n�vekszik, kitoltos : profilnevet �res hellyel t�lti ki ha az kevesebb mint 18karakter
	unsigned char KeyAlfOrNum = 0;						//Alfanumerikus vagy Numerikus gombok
	unsigned int ciklus = 0, x;							//cilus perges, sz�mol�

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
	
	lcd_clear();	//T�r�lj�k az LCD kijelz�t

	//Beolvassuk az Eeprom els� 1 b�jt-j�t ami t�rolja az utols� elmentett profil sz�m�t (1-15)
	if((int)EepromRead()>0 && (int)EepromRead()<=PPROGSZAM){
		//Ha �rv�nyes adatot kaptunk tov�bb l�phet�nk
		ReadCim.value = PMEMBLOCK*(int)EepromRead()-PMEMBLOCK;	//Az Eepromban elmentett c�mz�st be�ll�tjuk (32b�jt*0-14)
		memRead25LC640(ReadCim, pmemblockbuffer);				//kiolvassuk a mem�ri�b�l az eeproba t�rolt profil azonos�t�t
		//Megvizsg�ljuk, hogy van e �rv�nyes�tett adat a c�men
		if(pmemblockbuffer[0]=='#'){
			//Ha van �rv�nyes adat
			printf("Utolso elmentett    ");
			lcd_write(0xC0,0,1,1);   
			printf("profil betoltese... ");
			lcd_write(0x94,0,1,1);
			//N�v let�rol�sa
			kitoltes=1;	//Ha nincs r� sz�ks�g
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
			//H�profil �s id�k let�rol�sa
			for(i=1; i<PINTSIZE; i++){
				ERVPROFINT[i-1] = pmemblockbuffer[PCHARSIZE+i];
			}
			//Als� f�t�s 2b�jtj�nak �sszef�z�se �s let�rol�sa
			AlsoFutes.value = 0;	//Null�zzuk az adatstrukt�r�t
			i = PCHARSIZE+i;
			AlsoFutes.low = pmemblockbuffer[i+1];
			AlsoFutes.high = pmemblockbuffer[i+2];
			ERVPROFINT[PINTSIZE] = AlsoFutes.value;	//Az �sszevont 16bites adatot belet�ltjuk a v�ltoz�nkba
			memset(pmemblockbuffer, 0, sizeof(pmemblockbuffer));
			AlsoFutes.value = 0;
			EepromTrue=1;	//Van �rv�nyes profil elmentve �s bet�ltve a bufferbe
		}else{
			//Ha nincs �rv�nyes adat a mem�ria c�men hib�val t�r�nk vissza
			printf("Hiba: Eeprom, meg   ");
			lcd_write(0xC0,0,1,1);   
			printf("nincs v. hibas adat!");
			lcd_write(0x94,0,1,1);
			memset(pmemblockbuffer, 0, sizeof(pmemblockbuffer));	//T�r�lj�k a hib�s profil buffert
			ReadCim.value = 0;	//Null�zzuk a mem�ria c�mz�st
			EepromTrue=0;		//Nincs �rv�nyes Eeprom elment�s
		}
	}else{
		//Ha m�g nem t�rt�n ment�s az Eepromba (els� ind�t�s vagy hiba eset�n f�rdul el�)
		printf("Hiba: Eeprom, nincs ");
		lcd_write(0xC0,0,1,1);   
		printf("ertekelheto adat!   ");
		lcd_write(0x94,0,1,1);
		EepromTrue=0;	//Nincs �rv�nyes Eeprom ment�s
	}
	printf("                    ");
	lcd_write(0xD4,0,1,1);
	printf("                    ");
	lcd_write(0x80,0,1,1);
	Delay_ms(600*3);	//V�runk ~3mp-et
	
	profil_betoltese:
	i=0, x=0;
	while(1){

		if(EepromTrue==1){
			//Ha van �rv�nyes profil bet�ltve akkor ki�rjuk az LCD-re �s ind�that� egyb�l
			keyval = get_key(KeyAlfOrNum);	//0 vagy is numerikus gombok enged�lyez�se
			lcd_clear();
			//ERVPROFCHAR n�v t�rol� adat t�mb
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
			//Nincs �rv�nyes profil bet�ltve, ugrunk a men�re (els� ind�l�s �s hiba eset�n)
		}
		Delay10KTCYx(100);
	
	ciklus++;
	}//while
	

}//main
