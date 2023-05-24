/*********************************************
+		Statikus v�ltoz�k
**********************************************/
static char alfanum[17][5] = {  {'d'},	//OK
								{'0','Q','Z',' '},
								{'e'},	//DELL
								{'f'},	//SETUP
								
								{'7','P','R','S'},
								{'8','T','U','V'},
								{'9','W','X','Y'},
								{'c'},	//MENU
								
								{'4','G','H','I'},
								{'5','J','K','L'},
								{'6','M','N','O'},
								{'b'},	//STOP
								
								{'1'},
								{'2','A','B','C'},
								{'3','D','E','F'},
								{'a'}	//START
							};

/*********************************************
+		K�sleltet�s
**********************************************/
void Delay_ms(unsigned int ms){ //Bemenet*ms v�rakoz�s
   unsigned int i=0;
   for (i=0;i<=ms;i++) Delay10KTCYx(2);
}

/*********************************************
+		PWM PS3 ventill�tor be�ll�t�sa
+		Egys�g: (volt*1000)/1023 (10bit-es felbont�s) = 1 egys�g == 4,89
+		Volt: 1 egys�g / 1000 = volt == 0,00489v
+		Fordulat: 2450/1023 = 2.4 (1 egys�gnyi id� alatt 2.4 fordulat, Maximum 2450RPM (12v))
+		Minimum 0.7335v == 150 (10bit-es felbont�sban) == 360rpm
+		PWM1 = RC2 l�b
+		PWM2 = RC1 l�b
**********************************************/
void ventillator1(unsigned int szint){
	OpenTimer2( TIMER_INT_OFF & T2_PS_1_4 & T2_POST_1_8 );
	OpenPWM1(0xff);
	switch(szint){
		case 0:			//Nincs forg�s 0rpm (0v)
		SetDCPWM1(0);
		break;
		case 1:			//f�rdulat 492rpm (1v)
		SetDCPWM1(205);
		break;
		case 2:			//f�rdulat 984rpm (2v)
		SetDCPWM1(410);
		break;
		case 3:			//f�rdulat 1474rpm (3v)
		SetDCPWM1(614);
		break;
		case 4:			//f�rdulat 1963rpm (4v)
		SetDCPWM1(818);
		break;
		case 5: 		//Max f�rdulat 2450rpm (5v)
		SetDCPWM1(1023);
		break;
		default:		//Ha az �rt�k hib�s akkor a ventill�tor nem indul
		SetDCPWM1(0);
	}
	
}

/*********************************************
+		PWM Veltill�tor 2
**********************************************/
void ventillator2(unsigned int speed){
	OpenPWM2(0xff);
	SetDCPWM2(speed);
}

/*********************************************
+		LCD T�rl�s
**********************************************/
void lcd_clear(void){
	printf("                    ");
	lcd_write(0xC0,0,1,1);   
	printf("                    ");
	lcd_write(0x94,0,1,1);
	printf("                    ");
	lcd_write(0xD4,0,1,1);
	printf("                    ");
	lcd_write(0x80,0,1,1); 
}

/*********************************************
+		Eeprom olvas�sa
**********************************************/
unsigned char EepromRead (void){
	unsigned char buffer;
	EEADR = 0;				// Mem�ria c�mt�l kezdi az olvas�st
	EECON1bits.EEPGD = 0;	// Flash Program vagy adat EEPROM mem�ria v�laszt� bit
							// 1 = Hozz�f�r�s a flash program mem�ri�hoz
							// 0 = Hozz�f�r�s az EEPROM mem�ri�hoz
	EECON1bits.CFGS = 0;	// Flash Program / adat EEPROM vagy konfigur�ci� v�laszt� bit
							// 1 = Hozz�f�r�s a konfigur�ci�s nyilv�ntart�sokhoz
							// 0 = Hozz�f�r�s a Flash program vagy adat EEPROM mem�ri�hoz	
	EECON1bits.RD = 1;		// Olvas�st vez�rl� bit
							// 1 = kezdem�nyezi az EEPROM olvas�s�t (RD bitet nem lehet be�ll�tani , ha EEPGD = 1 vagy CFGS = 1 )
							// 0 = Nem kezdem�nyez EEPROM olvas�s
	buffer = EEDATA;		// Kiolvassuk az adatot
	return buffer;
}

/*********************************************
+		Eeprom �r�sa
**********************************************/
void EepromWrite (unsigned char adat){
	EEADR = 0;				// Mem�ria c�mt�l kezdj�k az �r�st
	EEDATA = adat;
	EECON1bits.EEPGD = 0;	// Flash Program vagy adat EEPROM mem�ria v�laszt� bit
							// 1 = Hozz�f�r�s a flash program mem�ri�hoz
							// 0 = Hozz�f�r�s az EEPROM mem�ri�hoz
	EECON1bits.CFGS = 0;	// Flash Program / adat EEPROM vagy konfigur�ci� v�laszt� bit
							// 1 = Hozz�f�r�s a konfigur�ci�s nyilv�ntart�sokhoz
							// 0 = Hozz�f�r�s a Flash program vagy adat EEPROM mem�ri�hoz
	EECON1bits.WREN = 1;	// Flash Program / adat EEPROM �r�st enged�lyez� bit
							// 1 = Enged�lyezi a Flash program / adat EEPROM �r�s�t
							// 0 = G�tolja a Flash program / adat EEPROM �r�s�t
	INTCONbits.GIE = 0;		//Tiltjuk a interrupt-ot (megszak�t�st)
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;		// �r�st vez�rl� bit
							// 1 = kezdem�nyez adat EEPROM t�rl�s / �r�s ciklust vagy a program mem�ria t�rl�si ciklust vagy �r�si ciklus
							// 0 = Write ciklus az EEPROM teljes
	while (EECON1bits.WR == 1);	//V�rakozunk az �r�s befejez�s�ig
	INTCONbits.GIE = 1;		// enged�lyezz�k az interrupt-ot (megszak�t�st)
	EECON1bits.WREN = 0;	// Flash Program / adat EEPROM �r�st enged�lyez� bit
							// 1 = Enged�lyezi a Flash program / adat EEPROM �r�s�t
							// 0 = G�tolja a Flash program / adat EEPROM �r�s�t
}

/*
EECON1bits.EEPGD = 0;	// Flash Program vagy adat EEPROM mem�ria v�laszt� bit
						// 1 = Hozz�f�r�s a flash program mem�ri�hoz
						// 0 = Hozz�f�r�s az EEPROM mem�ri�hoz
EECON1bits.CFGS = 0;	// Flash Program / adat EEPROM vagy konfigur�ci� v�laszt� bit
						// 1 = Hozz�f�r�s a konfigur�ci�s nyilv�ntart�sokhoz
						// 0 = Hozz�f�r�s a Flash program vagy adat EEPROM mem�ri�hoz					
//bit 5 Unimplemented: Read as '0'
EECON1bits.FREE = 0;	// flash t�rl�s�nek enged�lyez�se
						// 1 = T�rl�s enged�lyez�se
						// 0 = Csak olvas�s
EECON1bits.WRERR = 0;	// Flash Program / adat EEPROM hiba jelz� bit
						// 1 = �r�si m�velet id� el�tt megsz�nik ( reset sor�n �n�ll� id�z�tett programoz�s norm�l m�k�d�s , vagy nem megfelel� �r�si k�s�rlet )
						// 0 = Az �r�si m�velet befejez�d�tt
EECON1bits.WREN = 0;	// Flash Program / adat EEPROM �r�st enged�lyez� bit
						// 1 = Enged�lyezi a Flash program / adat EEPROM �r�s�t
						// 0 = G�tolja a Flash program / adat EEPROM �r�s�t
EECON1bits.WR = 0;		// �r�st vez�rl� bit
						// 1 = kezdem�nyez adat EEPROM t�rl�s / �r�s ciklust vagy a program mem�ria t�rl�si ciklust vagy �r�si ciklus
						// 0 = Write ciklus az EEPROM teljes
EECON1bits.RD = 0;		// Olvas�st vez�rl� bit
						// 1 = kezdem�nyezi az EEPROM olvas�s�t (RD bitet nem lehet be�ll�tani , ha EEPGD = 1 vagy CFGS = 1 )
						// 0 = Nem kezdem�nyez EEPROM olvas�s
//megjegyz�s : Ha hiba ( WRERR ) t�rt�nik, a EEPGD �s CFGS bit nem t�rl�dik ez �lltal lehet�v� teszi a hiba nyomon k�vet�s�t
*/


/*********************************************
+		SPI 25LC640 mem�ria
**********************************************/
unsigned char spi_io(unsigned char adat){
	unsigned char tmp;  
	SSPSTATbits.BF = 0;			// t�rli foglalts�g jelz� �lapot�t a st�tusz regiszterben
	SSPCON1bits.WCOL = 0;		// t�rli az esetleges �r�s �tk�z�s hibajelz�t
	tmp = SSPBUF;				// t�rli a BF jelz�bitet
	SSPBUF = adat;				// kirakja a kimen� adatot az SSPBUF regiszterbe
	while(!SSPSTATbits.BF);		// megv�rjuk a busz ciklus v�g�t 
	return (SSPBUF);			// a vett b�jttal t�r�nk vissza
}
	/** V�rakoz�s arra, hogy az EEPROM befejezze az �r�st. Am�g az �r�s tart,
	 *  addig az EEPROM STATUS regiszter�nek legals� bitje (WIP) '1'-ben �ll.
	 *  A f�ggv�ny blokkol� t�pus�, addig nem t�r vissza, am�g az EEPROM foglalt.
	 */
void waitFor25LC640(void) {
	  unsigned int flag;
	  do {
		SPI_CS = 0;                      	    //kiadjuk a Chip Enable jelet
		spi_io(CMD_RDSR);                       //St�tuszregiszter olvas�sa parancs
		flag = spi_io(0x00);                    //Olvasunk, de valamit akkor is k�ldeni kell!
		SPI_CS = 1;                   		    //megsz�ntetj�k a Chip Enable jelet
	  } while (flag & 0x01); 
}
	/** Egy mem�rialap (? b�jt) �r�sa a bemen� adatbufferb�l, az EEPROM egy
	 *  megadott c�m�t�l kezd�d�en.
	 *  \param MemAddr a mem�rialap kezd�c�me, ahov� �runk
	 *  \param buf mutat� az adatbuffer kezdet�hez
	 */
	//---------------- char t�pus� -----------------------------// 
void memWrite25LC640(union16 MemCim, unsigned char *pbuf) {
		unsigned char i;
		waitFor25LC640();                         //V�runk, ha az EEPROM elfoglalt
		SPI_CS = 0;
		WriteSPI(CMD_WREN);                       //�r�s �jraenged�lyez�se
		SPI_CS = 1;
		Delay10KTCYx(1);
		SPI_CS = 0;
		spi_io(CMD_WRITE);                        //Adatblokk �r�sa
		spi_io(MemCim.low);
		spi_io(MemCim.high);
		for (i=0; i < PCHARSIZE; i++) {
			spi_io(*pbuf++);                      //Az adatbuffer ki�r�sa
		}
		SPI_CS = 1;
}
void memRead25LC640(union16 MemCim, unsigned char *pbuf) {
	  unsigned char i;
	  waitFor25LC640();                         //V�runk, ha az EEPROM elfoglalt
	  SPI_CS = 0;
	  spi_io(CMD_READ);                         //Adatblokk olvas�sa
	  spi_io(MemCim.low);
	  spi_io(MemCim.high);
	  for (i=0; i < PCHARSIZE; i++) {
		*pbuf+=spi_io(0x00);                   //Olvasunk, de valamit akkor is k�ldeni kell!
	  }
	  SPI_CS = 1;
}

/*********************************************
+		4x4 MATRIX f�ggv�nyek
**********************************************/
/*Elen�riz�k a melyik gombt nyomtuk le*/
unsigned char get_key(unsigned char alfavagynum){
	unsigned char i, k=0, result;
	for(i=0; i<4; i++){
		SOROK = (0x08>>i); 	//L�ptetj�k a sorok szintj�t
		if(k!=0) k++;
		if(oszlop1){
			if(alfavagynum){	//ha 1 akkor alfanumerikus gombosort v�runk
				return get_alfa_value(k);
			}else{	//ha nulla akkor numerikus gomsort v�runk
				return get_num_value(k);
			}
		}
		k++;
		if(oszlop2){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		k++;
		if(oszlop3){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		k++;
		if(oszlop4){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
	}
	return '#';
}

/*Ellen�riz�k gombon bel�l melyik karaktert v�lasztjuk (alfanumerikus)*/
unsigned char get_num_value(unsigned char key){
	unsigned char i = 50, x = 0, result;
	if(oszlop1){
		while(oszlop1); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}
	if(oszlop2){	
		while(oszlop2); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}
	if(oszlop3){
		while(oszlop3); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}
	if(oszlop4){
		while(oszlop4); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}
	return result;
}

/*Ellen�riz�k gombon bel�l melyik sz�mot v�lasztjuk (numerikus)*/
unsigned char get_alfa_value(unsigned char key){
	unsigned char i = 50, x = 0, result;
	
	if(oszlop1){
		if(key!=12 & key!=0){	//Ha 1-est vagy ok-ot nyomtunk �tugrik az else �gra �s azonna vissza t�r
			do{
				if(oszlop2 | oszlop3 | oszlop4) break;
				if(oszlop1){
					while(oszlop1); //V�rakozunk a gomb felenged�s�ig
					
					printf("                    ");
					lcd_write(0xC0,0,1,1);	
					printf("~[%c]              ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("                    ");
					lcd_write(0xD4,0,1,1);
					printf("                    ");
					lcd_write(0x80,0,1,1);
					
					result = alfanum[key][x];
					
					if(x==3){	//H�romig emelj�k x �rt�k�t majd null�zuk (K�rbeforgatjuk a tomb tartam�t)
						x=0;
					}else{
						x++;
					}
				}
				Delay10KTCYx(50);
				//Delay10KTCYx(2);	//proteus
			}while(i--);	//V�rakozik h�tha �rkezik ebben a ciklusid�ben tov�bbi k�r�s
		}else{
			while(oszlop1); //V�rakozunk a gomb felenged�s�ig
			result = alfanum[key][0];
		}
	}
	if(oszlop2){	
			do{
				if(oszlop1 | oszlop3 | oszlop4) break;
				if(oszlop2){
					while(oszlop2); //V�rakozunk a gomb felenged�s�ig
						
					printf("                    ");
					lcd_write(0xC0,0,1,1);	
					printf("~[%c]              ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("                    ");
					lcd_write(0xD4,0,1,1);
					printf("                    ");
					lcd_write(0x80,0,1,1);
						
					result = alfanum[key][x];
					
					if(x==3){	//H�romig emelj�k x �rt�k�t majd null�zuk (K�rbeforgatjuk a tomb tartam�t)
						x=0;
					}else{
						x++;
					}
				}
				Delay10KTCYx(50);
				//Delay10KTCYx(2);	//proteus
			}while(i--);	//V�rakozik h�tha �rkezik ebben a ciklusid�ben tov�bbi k�r�s
	}
	if(oszlop3){
		if(key!=2){	//Ha DELL-t nyomtunk azonnal az else �gra ugrik �s vissza t�r
			do{
				if(oszlop1 | oszlop2 | oszlop4) break;
				if(oszlop3){
					while(oszlop3); //V�rakozunk a gomb felenged�s�ig
							
					printf("                    ");
					lcd_write(0xC0,0,1,1);	
					printf("~[%c]              ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("                    ");
					lcd_write(0xD4,0,1,1);
					printf("                    ");
					lcd_write(0x80,0,1,1); 
						
					result = alfanum[key][x];
					
					if(x==3){	//H�romig emelj�k x �rt�k�t majd null�zuk (K�rbeforgatjuk a tomb tartam�t)
						x=0;
					}else{
						x++;
					}
				}
				Delay10KTCYx(50);
				//Delay10KTCYx(2);	//proteus
			}while(i--);	//V�rakozik h�tha �rkezik ebben a ciklusid�ben tov�bbi k�r�s
		}else{
			while(oszlop3); //V�rakozunk a gomb felenged�s�ig
			result = alfanum[key][0];
		}
	}
	if(oszlop4){
		while(oszlop4); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}
	return result;
}
			
/*********************************************
+		Profil adatok bek�r� �zenetei
**********************************************/
void profiltext(unsigned int x){
		switch(x){
			case 0:
				printf("Profil nev:         ");
			break;
			case 1:
				printf("Kezdo hofok:        ");
			break;
			case 2:
				printf("Kezdo ido:          ");
			break;
			case 3:
				printf("Hofok 2:            ");
			break;
			case 4:
				printf("Program ido 2:      ");
			break;
			case 5:
				printf("Hofok 3:            ");
			break;
			case 6:
				printf("Program ido 3:      ");
			break;
			case 7:
				printf("Hofok 4:            ");
			break;
			case 8:
				printf("Program ido 4:      ");
			break;
			case 9:
				printf("Hofok 5:            ");
			break;
			case 10:
				printf("Program ido 5:      ");
			break;
			case 11:
				printf("Kifuto hofok:       ");
			break;
			case 12:
				printf("Kifuto ido:         ");
			break;
			case 13:
				printf("Also futes:         ");
			break;
			case 14:
				printf("Profil letrehozva!!!");
			break;
			default:
				printf("Hiba: profiltext()  ");
			break;
		}	
}

/*********************************************
+		Karakter vagy sz�m ellen�rz�
+		1=sz�m (TRUE), 0=nem sz�m (FALSE)
**********************************************/
unsigned int szamvagykarakter(unsigned char tomb[]){
    unsigned int i=0, result=0;
    while(tomb[i]){ //Amig nem lesz nulla
        if ( (tomb[i] >= '0' && tomb[i] <= '9') ){
            result = 1;
        }else{
            result = 0;
            break;
        }
        i++;
    }
    return result;
}
