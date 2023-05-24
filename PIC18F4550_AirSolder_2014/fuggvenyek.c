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

	void waitFor25LC640(void) {
	  unsigned int flag;
	  do {
		SPI_CS = 0;                      	    //kiadjuk a Chip Enable jelet
		spi_io(CMD_RDSR);                       //St�tuszregiszter olvas�sa parancs
		flag = spi_io(0x00);                    //Olvasunk, de valamit akkor is k�ldeni kell!
		SPI_CS = 1;                   		    //megsz�ntetj�k a Chip Enable jelet
	  } while (flag & 0x01); 
	}

	void memWrite25LC640(union16 MemCim, unsigned char *pbuf) {
		unsigned char i,flag;
		waitFor25LC640();                         //V�runk, ha az EEPROM elfoglalt
		SPI_CS = 0;
		WriteSPI(CMD_WREN);                       //�r�s �jraenged�lyez�se
		SPI_CS = 1;
		Delay10KTCYx(2);
		SPI_CS = 0;
		spi_io(CMD_WRITE);                        //Adatblokk �r�sa
		spi_io(MemCim.low);
		spi_io(MemCim.high);
		for (i=0; i < PMEMBLOCK; i++) {
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
	  for (i=0; i < PMEMBLOCK; i++) {
		*pbuf++=spi_io(0x00);                   //Olvasunk, de valamit akkor is k�ldeni kell!
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
		Delay_ms(2);		//k�sleltet�nk kicsit
		if(oszlop2){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		k++;
		Delay_ms(2);		//k�sleltet�nk kicsit
		if(oszlop3){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		k++;
		Delay_ms(2);		//k�sleltet�nk kicsit
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
					printf("~[%c]               ", alfanum[key][x]);
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
					printf("~[%c]               ", alfanum[key][x]);
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
					printf("~[%c]               ", alfanum[key][x]);
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
void profiltext(unsigned char x){
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

/****************************************************************************
+						"profilt_mutat()" f�ggv�ny
+					A m�r bet�lt�tt profilt mutatja
*****************************************************************************/
void profilt_mutat(void){
	unsigned int ciklus=0, i=0, x=1;
	unsigned char keyval;
	lcd_clear();
	while(1){
		//Ha van �rv�nyes profil bet�ltve akkor ki�rjuk az LCD-re �s ind�that� egyb�l
		keyval = get_key(KeyNumeric);	//numerikus gombok enged�lyez�se
		if(keyval=='a'){
			//Start
			Navigacio=5;
			break;
		}else if(keyval=='c'){
			//goto menu; 	//Ugr�s a men�re
			Navigacio=0;
			break;
		}
		//ERVPROFCHAR n�v t�rol� adat t�mb
		printf("%s", ERVPROFCHAR);
		lcd_write(0xC0,0,1,1);
	   	
		//ERVPROFINT h�profil t�rol� ki�r�sa
		if(ciklus%40==0 && i<PINTSIZE){
			if(x==7){
				printf("%i. Also futes: %03i%c ", x, ERVPROFINT[i+1], 0xdf);
			}else{
				printf("%i. %03i%c %03imp      ", x, ERVPROFINT[i], 0xdf, ERVPROFINT[i+1]);
			}
			if(x<7){
				i++; i++; x++;
			}else{
				i=0; x=1;
			}
		}else{
			if(x==7){
				printf("%i. Also futes: %03i%c ", x, ERVPROFINT[i+1], 0xdf);
			}else{
				printf("%i. %03i%c %03imp       ", x, ERVPROFINT[i], 0xdf, ERVPROFINT[i+1]);
			}
		}
	
		lcd_write(0x94,0,1,1);
	
		printf("Prog inditasa: START");
		lcd_write(0xD4,0,1,1);
		printf("Tovabbi opcio: MENU ");
		lcd_write(0x80,0,1,1);
		Delay_ms(20);
	ciklus++;
	}//while
}

/****************************************************************************
+						"menu()" f�ggv�ny
+					A men� kezel�s�t teszi lehet�v�
*****************************************************************************/
void menu(void){
	unsigned char keyval;
	lcd_clear();
	while(1){
		keyval = get_key(KeyNumeric);	//numerikus gombok enged�lyez�se
		if(keyval=='1'){
			//�j profil hozz�ad�sa
			Navigacio=2;
			break;
		}else if(keyval=='2'){
			//Aktu�lis profil szerkeszt�se
			Navigacio=3;
			break;
		}else if(keyval=='3'){
			//Profilok list�z�sa
			Navigacio=4;
			break;
		}else if(keyval=='4'){
			//Ugr�s a m�r bet�lt�tt profilra
			//goto profil_betoltve;
			Navigacio=1;
			break;
		}
		printf("1. Uj profil hozzaad");
		lcd_write(0xC0,0,1,1);   
		printf("2. Akt. prof. szerk.");
		lcd_write(0x94,0,1,1);
		printf("3. Profilok listaja ");
		lcd_write(0xD4,0,1,1);
		printf("4. Program          ");
		lcd_write(0x80,0,1,1);
		Delay_ms(2);
	}
}

/****************************************************************************
+						"uj_profil()" f�ggv�ny
+						�j profil l�trehoz�s�hoz
*****************************************************************************/
void uj_profil(void){
	unsigned char buffer[PMEMBLOCK+1] = {0};		//Buffer mem�ria
	unsigned char text[PCHARSIZE+1] = {0};			//Ideiglenes karaktereket t�rolunk benne (4x4 matrix)
	unsigned char i=0, count=0, cursor=0xFF, kitoltes, ptleptet=0;
	unsigned char adatcount=PCHARSIZE;				//Adatok ment�s�nek kezdete a PCHARSIZE. b�jtt�l
	unsigned char keyval;							//gomb �rt�k�t t�rolja
	union16 bytes, MemCim;							//16bites adat kett�v�laszt�s�hoz
	bytes.value = 0;
	MemCim.value = 0;
	lcd_clear();
	while(1){
		
		keyval = ptleptet > 0 ? get_key(KeyNumeric) : get_key(KeyAlfanum);	//Alfanumerikus gombok enged�lyez�se
		
		if(keyval != '#'){		//Ha a felt�tel igaz akkor t�rt�nik gombnyom�s
			if(keyval == 'e'){	//DELL gomb megnyom�s�n�l k�vetkezik be
				if(i!=0 && strlen(text)>0){	//Ha van mit t�r�lni
					i--;
					text[i] = 0;	//"\0"-val jelezz�k a t�mb v�g�t (gyakorlatilag t�rl�nk a t�mb v�g�r�l visszafel� egyet)
				}
			}else if(keyval == 'd'){	//OK gomb megnyom�s�n�l k�vetkezik be
				//Ha a felt�tel igaz, ment�nk... (itt majd a mem�ri�ba fogunk �rni)
				if(ptleptet > 0){	//Ha igaz akkor a nevet elmentett�k
					if(szamvagykarakter(text)){	//Ellen�riz�k, hogy biztos csak sz�mot k�ldtek
						if(atoi(text)<256 && atoi(text)>0 && ptleptet<13){	//Ha "ptleptet" l�p�s sz�ml�l� ejut 13-ig akkor az az als� f�t�s �rt�k�t v�rja ami nagyobb mint 255 �s nagyobb mint nulla
							buffer[adatcount] = (char)atoi(text);
							printf("Adat eltarolva!     ");
							adatcount++;
							ptleptet++;
						}else if(ptleptet==13 && atoi(text)>0){
							bytes.value = atoi(text);
							buffer[adatcount] = bytes.low;
							adatcount++;
							buffer[adatcount] = bytes.high;
							adatcount++;
							buffer[adatcount] = '\0';	//Lez�rjuk a buffert
							ptleptet++;
						}else{
							printf("Hiba: 1 <-> 255     ");
						}
					}else{
						printf("Hiba: Nem szam!     ");
					}
				}else{
					if(strlen(text)>2){ 
						buffer[0] = '#';
						kitoltes = 1;	//1, ha nincs r� sz�ks�g
						for(i=1; i < PCHARSIZE; i++){
							if(text[i-1]!= '\0'){
								if(kitoltes){
									buffer[i] = text[i-1];
								}else{
									buffer[i] = ' ';
								}
							}else{
								kitoltes = 0;
								buffer[i] = ' ';
							} 
							//if(i==PCHARSIZE-1) buffer[i] = '\0';
						}
						printf("Profilnev tarolva!  ");
						ptleptet++;
					}else{
						printf("Hiba:Min 3 karakter!");
					}
				}
				memset(text, 0, sizeof(text));	//T�r�lj�k a t�mb tartalm�t
				lcd_write(0xC0,0,1,1); 
				
				Delay_ms(600);	//kb 1mp
				
				i=0;	//Null�zuk a t�mbn�vel� i v�ltoz�t is
			}else if(keyval == 'c'){	//Kis c (c == Men�) nyom�s�n�l k�vetkezik be
				Navigacio=0;
				break;
			}else{
				if(strlen(text)<PCHARSIZE && ptleptet==0){	//Addig, amddig el�rj�k a maximum megengedett be�rhat� karaktert (PROF_NAME_MAX == 18)
					text[i] = keyval;
					i++;
				}else if(ptleptet>0 && strlen(text)<3){	//Ha m�r a profiln�v ment�sre ker�lt (j�hetnek a sz�madatok)
					text[i] = keyval;
					i++;
				}
			}
		}
		if(count%Cursors==0) cursor = cursor==' ' ? ptleptet>0 ? ptleptet%2==0 ? 's' : 0xdf : 0xff : ' ';	//Kurz�rt villogtatjuk
		if(ptleptet==14){
			lcd_clear();
			
			//Ment�s mem�ri�ba
			MemCim.value = profilok_szama(0) == 0 ? 0 : PMEMBLOCK*profilok_szama(0);
			memWrite25LC640(MemCim, buffer);         //Mem�ria lap ki�r�sa
			EepromWrite(profilok_szama(0));
			
			profiltext(ptleptet);
			lcd_write(0xC0,0,1,1);  
			printf("Adatok elmentve!    ");
			lcd_write(0x94,0,1,1);
			printf("Profil betoltese    ");
			lcd_write(0xD4,0,1,1);
			printf("folyamatban....     ");
			lcd_write(0x80,0,1,1);
			Delay_ms(600*3);	//kb ~3mp
			
			//Bet�ltj�k az elmentett profilt
			//N�v let�rol�sa
			kitoltes=1;	//Ha nincs r� sz�ks�g
			for(i=0; i<=PCHARSIZE; i++){
				if(buffer[i]!= ' '){
					if(kitoltes){
						ERVPROFCHAR[i] = buffer[i+1];
					}else{
						ERVPROFCHAR[i] = ' ';
					}
				}else{
					kitoltes = 0;
					ERVPROFCHAR[i] = ' ';
				}
				if(i==PCHARSIZE) ERVPROFCHAR[i] = '\0';
			}
			kitoltes=1;
			//H�profil �s id�k let�rol�sa
			for(i=0; i<PINTSIZE-1; i++){
				ERVPROFINT[i] = buffer[PCHARSIZE+i];
			}
			//Als� f�t�s 2b�jtj�nak �sszef�z�se �s let�rol�sa
			ERVPROFINT[PINTSIZE] = bytes.value;	//Az �sszevont 16bites adatot belet�ltjuk a v�ltoz�nkba
			
			//Null�zuk a v�ltoz�kat az �jra kezd�s miatt
			ptleptet=0;
			bytes.value = 0;
			MemCim.value = 0;
			memset(text, 0, sizeof(text));			//T�r�lj�k a t�mb tartalm�t
			memset(buffer, 0, sizeof(buffer));		//T�r�lj�k a t�mb tartalm�t
			Navigacio=1;
			break;
		}else{
			profiltext(ptleptet);
			lcd_write(0xC0,0,1,1); 
		}
		printf("%s%c                ", text, cursor);
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("Tovabbi opcio: MENU ");
		lcd_write(0x80,0,1,1);
	Delay_ms(10);
	count++;
	}//while()
}

/****************************************************************************
+						"profilok_szama()" f�ggv�ny
+			Profilok sz�m�t adja vissza, ha 0-val param�terezz�k
+			Ha 0-n�l nagyobb sz�m a param�ter akkor vizsg�lodik, �s ha 
+			van a c�men �rv�nyes adat 1-el t�r vissza		
*****************************************************************************/
unsigned char profilok_szama(unsigned char pszam){
	unsigned char i=0, x=1;
	unsigned char MemBuffer[PMEMBLOCK+1] = {0};
	unsigned char result = 0;
	union16 ReadCim;
	ReadCim.value = 0;
	if(pszam){
		//ReadCim.value = PMEMBLOCK;
		ReadCim.value = pszam > 1 ? (PMEMBLOCK*pszam)-PMEMBLOCK : 0;
		memRead25LC640(ReadCim, MemBuffer);
		if(MemBuffer[0]=='#'){
			result++;
		}
	}else{
		do{ 
			ReadCim.value += i==0 ? 0 : PMEMBLOCK;
			memRead25LC640(ReadCim, MemBuffer);
			if(MemBuffer[0]=='#'){
				result++;
			}else{
				x=0;
			}
			i++;
		}while(i<PPROGSZAM && x!=0);
	}	
	return result;
}

/****************************************************************************
+						"profilt_beletolt()" f�ggv�ny
+			Bet�lti az utolj�ra elmentett vagy kiv�lasztott profilt
*****************************************************************************/
void profilt_betolt(unsigned char profilszam){
	union16 ReadCim, AlsoFutes;
	unsigned char Membuffer[PMEMBLOCK+1] = {0}; //Profil blokk buffer
	unsigned char i, kitoltes;					//i n�vekszik, kitoltos : profilnevet �res hellyel t�lti ki ha az kevesebb mint 18karakter
	ReadCim.value = 0;
	AlsoFutes.value = 0;
	//Az Eepromban elmentett c�mz�st be�ll�tjuk
	ReadCim.value = ((PMEMBLOCK*profilszam)-PMEMBLOCK) == 0 ? 0 : (PMEMBLOCK*profilszam)-PMEMBLOCK;
	memRead25LC640(ReadCim, Membuffer);
	
	if(Membuffer[0]=='#'){
		//N�v let�rol�sa
		kitoltes=1;	//Ha nincs r� sz�ks�g
		for(i=0; i<=PCHARSIZE; i++){
			if(Membuffer[i]!= ' '){
				if(kitoltes){
					ERVPROFCHAR[i] = Membuffer[i+1];
				}else{
					ERVPROFCHAR[i] = ' ';
				}
			}else{
				kitoltes = 0;
				ERVPROFCHAR[i] = ' ';
			}
			if(i==PCHARSIZE) ERVPROFCHAR[i] = '\0';
		}
		kitoltes=1;
		//H�profil �s id�k let�rol�sa
		for(i=0; i<PINTSIZE-1; i++){
			ERVPROFINT[i] = Membuffer[PCHARSIZE+i];
		}
		//Als� f�t�s 2b�jtj�nak �sszef�z�se �s let�rol�sa
		AlsoFutes.value = 0;	//Null�zzuk az adatstrukt�r�t
		i = PCHARSIZE+i;
		AlsoFutes.low = Membuffer[i++];
		AlsoFutes.high = Membuffer[i++];
		ERVPROFINT[PINTSIZE] = AlsoFutes.value;	//Az �sszevont 16bites adatot belet�ltjuk a v�ltoz�nkba
	}else{
		printf("Hiba: Ervenytelen   ");
		lcd_write(0xC0,0,1,1);  
		printf("adatblock!          ");
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("                    ");
		lcd_write(0x80,0,1,1);
		Delay_ms(600);
		Navigacio=0;
	}
}

/****************************************************************************
+						"profil_lista()" f�ggv�ny
+					Bet�lti az osszes �rv�nyes profil nev�t
*****************************************************************************/
void profil_lista(void){
	unsigned char MemBlockBuffer[PMEMBLOCK+1] = {0};
	unsigned char listabuffer[3][PCHARSIZE+1] = {{0}};
	unsigned char keyval, i, lista=1, sorszam=0, kitoltes;
	unsigned char profilszam;
	union16 ReadCim, AlsoFutes;
	ReadCim.value = 0;
	AlsoFutes.value = 0;
	lcd_clear();

	//Els� lap lek�r�se
	ReadCim.value = 0;
	while(lista<=3){
		if(sorszam>0){
			ReadCim.value += PMEMBLOCK;
		}
		memRead25LC640(ReadCim, MemBlockBuffer);
		for(i=0; i<PCHARSIZE; i++){
			if(MemBlockBuffer[0]=='#'){
				listabuffer[lista-1][i] = MemBlockBuffer[i+1];	//+1 mert nem kell az azonos�t�
			}else{
				listabuffer[lista-1][i] = ' ';
			}
			if(i==PCHARSIZE-1) listabuffer[lista-1][PCHARSIZE] = '\0';
		}
		lista++;
		sorszam++;
	}
	lista = 0;
	sorszam++;
	while(1){
		keyval = get_key(KeyNumeric);
		
		if(keyval != '#'){
			
			if(keyval == '6'){ //-> Lapozunk jobbra.
				//T�mbb�k t�rl�se
				lista = 1;
				memset(MemBlockBuffer, 0, sizeof(MemBlockBuffer));
				memset(listabuffer, 0, sizeof(listabuffer));
				//Lap lek�r�se
				if(sorszam>=PPROGSZAM){	//A sorsz�mot resetelj�k, ha el�rt�k a list�zhat� maximum PPROGSZAM-ot. (15)
					sorszam = 1;
					ReadCim.value = 0;
				}
				while(lista<=3){
					if(sorszam!=1){
						ReadCim.value += PMEMBLOCK;
					}
					memRead25LC640(ReadCim, MemBlockBuffer);
					for(i=0; i<PCHARSIZE; i++){
						if(MemBlockBuffer[0]=='#'){
							listabuffer[lista-1][i] = MemBlockBuffer[i+1];	//+1 mert nem kell az azonos�t�
						}else{
							listabuffer[lista-1][i] = ' ';
						}
						if(i==PCHARSIZE-1) listabuffer[lista-1][PCHARSIZE] = '\0';
					}
					lista++;
					sorszam++;
				}
			}else if(keyval == '1' || keyval == '2' || keyval == '3'){
				//Profil sz�m meg�llap�t�sa
				if(keyval=='1'){
					profilszam = sorszam-3;
				}else if(keyval=='2'){
					profilszam = sorszam-2;
				}else if(keyval=='3'){
					profilszam = sorszam-1;
				}
				EepromWrite(profilszam);
				profilt_betolt(profilszam);
				Navigacio=1;
				break;
			}else if(keyval == 'c'){	//Kis c (c == Men�) nyom�s�n�l k�vetkezik be
				Navigacio=0;
				break;
			}	
		}
	
		//Ki�rjuk a k�perny�re a list�t
		if(listabuffer[0][0]!=' '){
			printf("%02i.%s", sorszam-3, listabuffer[0]);
		}else{
			printf("%02i. Ures", sorszam-3);
		}
		lcd_write(0xC0,0,1,1);  
		if(listabuffer[1][0]!=' '){
			printf("%02i.%s", sorszam-2, listabuffer[1]);
		}else{
			printf("%02i. Ures", sorszam-2);
		}
		lcd_write(0x94,0,1,1);
		if(listabuffer[2][0]!=' '){
			printf("%02i.%s", sorszam-1, listabuffer[2]);
		}else{
			printf("%02i. Ures", sorszam-1);
		}
		lcd_write(0xD4,0,1,1);
		printf("[Menu]         [6->]"); 
		lcd_write(0x80,0,1,1);
		Delay_ms(5);
		
	}
	
}

/****************************************************************************
+						"profil_szerk()" f�ggv�ny
+					Bet�lti a szerkesztend� profil adatait
*****************************************************************************/
void profil_szerk(void){
	unsigned char buffer[PMEMBLOCK+1] = {0};		//Buffer mem�ria
	unsigned char text[PCHARSIZE+1] = {0};			//Ideiglenes karaktereket t�rolunk benne (4x4 matrix)
	unsigned char i=0, count=0, cursor=0xFF, kitoltes, ptleptet=0;
	unsigned char adatcount=PCHARSIZE;				//Adatok ment�s�nek kezdete a PCHARSIZE. b�jtt�l
	unsigned char keyval;							//gomb �rt�k�t t�rolja
	union16 bytes, MemCim;							//16bites adat kett�v�laszt�s�hoz
	unsigned char fljelzo=0, stop=1;				//Felt�lt�st jelz� v�ltoz�
	
	bytes.value = 0;
	MemCim.value = 0;
	lcd_clear();
	while(1){
		
		//Szerkeszt�sn�l felel�s az adatok felt�lt�s��rt
		if(fljelzo==0 && stop){
			for(i=0; i<=PCHARSIZE; i++){
				text[i] = ERVPROFCHAR[i];
			}
			stop=0;
		}else if(fljelzo>0 && fljelzo<13 && stop){
			itoa(ERVPROFINT[fljelzo-1], text);
			stop=0;
		}else if(fljelzo==13 && stop){	
			itoa(ERVPROFINT[fljelzo], text);
			stop=0;
		}
			
		keyval = ptleptet > 0 ? get_key(KeyNumeric) : get_key(KeyAlfanum);	//Alfanumerikus gombok enged�lyez�se
		
		if(keyval != '#'){		//Ha a felt�tel igaz akkor t�rt�nik gombnyom�s
			if(keyval == 'e'){	//DELL gomb megnyom�s�n�l k�vetkezik be
				i= i==0 ? i=strlen(text) : i;
				if(i!=0 && strlen(text)>0){	//Ha van mit t�r�lni
					i--;
					text[i] = 0;	//"\0"-val jelezz�k a t�mb v�g�t (gyakorlatilag t�rl�nk a t�mb v�g�r�l visszafel� egyet)
				}
			}else if(keyval == 'd'){	//OK gomb megnyom�s�n�l k�vetkezik be
				//Ha a felt�tel igaz, ment�nk... (itt majd a mem�ri�ba fogunk �rni)
				if(ptleptet > 0){	//Ha igaz akkor a nevet elmentett�k
					if(szamvagykarakter(text)){	//Ellen�riz�k, hogy biztos csak sz�mot k�ldtek
						if(atoi(text)<256 && atoi(text)>0 && ptleptet<13){	//Ha "ptleptet" l�p�s sz�ml�l� ejut 13-ig akkor az az als� f�t�s �rt�k�t v�rja ami nagyobb mint 255 �s nagyobb mint nulla
							buffer[adatcount] = (char)atoi(text);
							printf("Adat eltarolva!     ");
							adatcount++;
							ptleptet++;
							fljelzo++; stop=1;	//Szerkeszt�sn�l emelj�k felt�lt�stjelz�-t �s enged�lyezz�k a lefut�st
						}else if(ptleptet==13 && atoi(text)>0){
							bytes.value = atoi(text);
							buffer[adatcount] = bytes.low;
							adatcount++;
							buffer[adatcount] = bytes.high;
							adatcount++;
							buffer[adatcount] = '\0';	//Lez�rjuk a buffert
							ptleptet++;
						}else{
							printf("Hiba: 1 <-> 255     ");
						}
					}else{
						printf("Hiba: Nem szam!     ");
					}
				}else{
					if(strlen(text)>2){ 
						buffer[0] = '#';
						kitoltes = 1;	//1, ha nincs r� sz�ks�g
						for(i=1; i < PCHARSIZE; i++){
							if(text[i-1]!= '\0'){
								if(kitoltes){
									buffer[i] = text[i-1];
								}else{
									buffer[i] = ' ';
								}
							}else{
								kitoltes = 0;
								buffer[i] = ' ';
							} 
							//if(i==PCHARSIZE-1) buffer[i] = '\0';
						}
						printf("Profilnev tarolva!  ");
						ptleptet++;
						fljelzo++; stop=1;	//Szerkeszt�sn�l emelj�k felt�lt�stjelz�-t �s enged�lyezz�k a lefut�st
					}else{
						printf("Hiba:Min 3 karakter!");
					}
				}
				memset(text, 0, sizeof(text));	//T�r�lj�k a t�mb tartalm�t
				lcd_write(0xC0,0,1,1); 
				
				Delay_ms(600);	//kb 1mp
				
				i=0;	//Null�zuk a t�mbn�vel� i v�ltoz�t is
			}else if(keyval == 'c'){	//Kis c (c == Men�) nyom�s�n�l k�vetkezik be
				Navigacio=0;
				break;
			}else{
				if(strlen(text)<PCHARSIZE && ptleptet==0){	//Addig, amddig el�rj�k a maximum megengedett be�rhat� karaktert (PROF_NAME_MAX == 18)
					text[i] = keyval;
					i++;
				}else if(ptleptet>0 && strlen(text)<3){	//Ha m�r a profiln�v ment�sre ker�lt (j�hetnek a sz�madatok)
					text[i] = keyval;
					i++;
				}
			}
		}
		if(count%Cursors==0) cursor = cursor==' ' ? ptleptet>0 ? ptleptet%2==0 ? 's' : 0xdf : 0xff : ' ';	//Kurz�rt villogtatjuk
		if(ptleptet==14){
			lcd_clear();
			
			//Ment�s mem�ri�ba
			MemCim.value = (PMEMBLOCK*EepromRead())-PMEMBLOCK;
			memWrite25LC640(MemCim, buffer);         //Mem�ria lap ki�r�sa
			
			profiltext(ptleptet);
			lcd_write(0xC0,0,1,1);  
			printf("Adatok elmentve!    ");
			lcd_write(0x94,0,1,1);
			printf("Profil betoltese    ");
			lcd_write(0xD4,0,1,1);
			printf("folyamatban....     ");
			lcd_write(0x80,0,1,1);
			Delay_ms(600*3);	//kb ~3mp
			
			//Bet�ltj�k az elmentett profilt
			//N�v let�rol�sa
			kitoltes=1;	//Ha nincs r� sz�ks�g
			for(i=0; i<=PCHARSIZE; i++){
				if(buffer[i]!= ' '){
					if(kitoltes){
						ERVPROFCHAR[i] = buffer[i+1];
					}else{
						ERVPROFCHAR[i] = ' ';
					}
				}else{
					kitoltes = 0;
					ERVPROFCHAR[i] = ' ';
				}
				if(i==PCHARSIZE) ERVPROFCHAR[i] = '\0';
			}
			kitoltes=1;
			//H�profil �s id�k let�rol�sa
			for(i=0; i<PINTSIZE-1; i++){
				ERVPROFINT[i] = buffer[PCHARSIZE+i];
			}
			//Als� f�t�s 2b�jtj�nak �sszef�z�se �s let�rol�sa
			ERVPROFINT[PINTSIZE] = bytes.value;	//Az �sszevont 16bites adatot belet�ltjuk a v�ltoz�nkba
			
			//Null�zuk a v�ltoz�kat az �jra kezd�s miatt
			ptleptet=0;
			bytes.value = 0;
			MemCim.value = 0;
			memset(text, 0, sizeof(text));			//T�r�lj�k a t�mb tartalm�t
			memset(buffer, 0, sizeof(buffer));		//T�r�lj�k a t�mb tartalm�t
			Navigacio=1;
			break;
		}else{
			profiltext(ptleptet);
			lcd_write(0xC0,0,1,1); 
		}
		printf("%s%c                ", text, cursor);
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("Tovabbi opcio: MENU ");
		lcd_write(0x80,0,1,1);
		Delay_ms(10);
	count++;
	}//while()
}

/****************************************************************************
+							"Max6675()" f�ggv�ny
+					Max6675 olvas� f�ggv�ny, param�terei 1-3-ig
*****************************************************************************/
short int Max6675(unsigned char MaxICcs){
	short int temp = 0;		// 8bites v�ltoz�
	short int result = 0;		// 16bites v�ltoz�
	float fokra_valt = 0;		// Lebeg�pontos v�ltoz�
	//-- Enged�lyezz�k a kiv�llasztott chip-et
	if(MaxICcs==1){
		MAX1CS = 0;       		// 1. MAX6675-�t enged�lyezz�k
	}else if(MaxICcs==2){
		MAX2CS = 0;       		// 2. MAX6675-�t enged�lyezz�k
	}else if(MaxICcs==3){
		MAX3CS = 0;       		// 3. MAX6675-�t enged�lyezz�k
	}
	Delay10KTCYx(2); 			// Id�z�t�s
	// --------- Beolvassuk  az 1-es MAX6675 adatait ---------------//
	temp = spi_io(0x00);		// Olvasunk, de valamit akkor is k�ldeni kell
	result = spi_io(0x00);		// Olvasunk, de valamit akkor is k�ldeni kell
	temp = temp << 8;    		// L�ptetjuk ballra 8 bitet �s let�roljuk az els� 8bitet
	result = result + temp; 	// Vissza alak�tjuk 16bites-re az adatot
	result = result >> 3;  		// L�ptet�nk jobbra 3bitet �gy megkapjuk a 12bitet
	//-- Enged�lyezz�k a kiv�llasztott chip-et
	if(MaxICcs==1){
		MAX1CS = 1;       		// 1. MAX6675-�t tiltjuk
	}else if(MaxICcs==2){
		MAX2CS = 1;       		// 2. MAX6675-�t tiltjuk
	}else if(MaxICcs==3){
		MAX3CS = 1;       		// 3. MAX6675-�t tiltjuk
	}
	fokra_valt = result*0.25;	// �tv�lt�s fokra
	return (int)fokra_valt;		// Visszat�r�nk a fokkal
}

/****************************************************************************
+							"hofokatlagolo()" f�ggv�ny
+					Elindd�l a program �s a forraszt�s
*****************************************************************************/
//H�fok �tlagol�
short int hofokatlagolo(short int foktomb[4], unsigned char tsize){
    unsigned char i;
    short int osszegzes = 0;
    float atlag;
    
    for(i=0; i<tsize; i++){
        osszegzes += foktomb[i];
    }
    atlag = (float)osszegzes / tsize;
    return (short)atlag;
}

/****************************************************************************
+							"start()" f�ggv�ny
+					Elindd�l a program �s a forraszt�s
*****************************************************************************/
void start(void){
	unsigned char i, x, visszaszamlalo;		//Bels� l�ptet�shez
	unsigned char keyval;					//Gombsor figyel�se
	short int homerseklet = 0;				//H�m�rs�kletet fogja t�rolni
	unsigned char Fej=0;					//Fej kiv�laszt�sa
	
	//Glob�lis v�ltoz�k
	//ERVPROFCHAR[18] n�v t�rol� 
	//ERVPROFINT[13] Profil adat t�rol�
	Tido = 0;								//timer0 id� m�r�s�hez
	Hofoklepes = 0;							//H�fokot l�ptetj�k glob�lisan a timer0 miatt
	
	lcd_clear();
	
	//Fejm�ret kiv�laszt�sa
	while(Fej == 0){							//Ha fej m�g nincs kiv�lasztva
		keyval = get_key(KeyNumeric);			// Gombsor figyel�se
		if(keyval!='#'){
			if(keyval=='1'){
				//Ventill�tor ind�t�sa (1-5v) (PWM 1)
				ventillator1(1);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='2'){
				ventillator1(2);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='3'){
				ventillator1(3);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='4'){
				ventillator1(4);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='5'){
				ventillator1(5);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}
		}
		printf("  1      2      3  ");
		lcd_write(0xC0,0,1,1); 
		printf("10x10  20x20  30x30");
		lcd_write(0x94,0,1,1);
		printf("     4       5     ");
		lcd_write(0xD4,0,1,1);
		printf("   40x40   50x50   ");
		lcd_write(0x80,0,1,1);
		Delay_ms(2);
	}
	
	INTCONbits.GIE = 1;						// Megszak�t�sok enged�lyez�se

	//timer ind�t�sa
	/*A timernek mindig 65535-46875=18660-r�l kell indulnia, (~1mp) hogy j�kor csorduljon t�l �s k�rje a megszak�t�st.
	Ezt az �rt�ket minden megszak�t�skor(r�gt�n az elej�n) bele kell �rni a TMR0L �s TMR0H regiszterekbe.
	Mivel ezek a regiszterek egyenk�nt 8 bitesek, sz�t kell bontani.
	18660 bin�risan: 01001000 11100100, vagyis
	18660 hex�ban: 	 0x48	  0xE4
	F�l�sodperces mintav�telez�s 18660*2: 10010001 11001000
	Hex�ban: 0x91	0xC8*/
	TMR0H = 0x91;
	TMR0L = 0xC8;
	INTCONbits.T0IF = 0;    // Timer0 t�lcsordul�sjelz�j�t t�r�lj�k
	INTCONbits.T0IE = 1;    // Timer0 megszak�t�sainak enged�lyez�se
	T0CONbits.TMR0ON = 1;	// Timer0 ind�t�sa
	
	lcd_clear();
	
	//Program elindul
	//Addig fut ameddig Hofoklepes kisebb mint maximum 6 l�p�s (0-5)
	for(i=0; i<6; i++){
		x = i==0 ? 0 : i*2;
		visszaszamlalo = ERVPROFINT[x+1];		// Vissza sz�ml�l�ba bet�ltj�k az id�t
		while(ERVPROFINT[x+1]>Tido/2){			// Addig fut ameddig Tido le nem j�r
			keyval = get_key(KeyNumeric);		// Gombsor figyel�se
			if(keyval != '#'){
				if(keyval == 'b'){				// Program le�ll�t�sa
					T0CONbits.TMR0ON = 0; 		// Timer0 t�lt�sa
					AlsoFutesRele = 0;			// Als�f�t�s let�lt�sa
					FelsoFutesRele = 0;			// Fels�f�t�s let�lt�sa
					Navigacio=6;
					return;
				}
			}
			//�tlagol� ind�t�sa
			if(AtlagBTCount<4){											// Ha �tlag sz�ml�l� kissebb mint 4
				HoBufferTomb[AtlagBTCount] = Max6675(2);				// Elt�roljuk a h�mint�t
				if(homerseklet==0){ homerseklet=Max6675(2); }			// Ha m�g nincs kijelezhet� h�minta
			}else if(AtlagBTCount==4){									// Ha �tlag sz�ml�l� nagyobb mint 4
				homerseklet = hofokatlagolo(HoBufferTomb, AtlagTMeret);	// �tlagoljuk a be�rkezett h�adatokat
			}
			printf("%s", ERVPROFCHAR);	//Ki�rjuk a nevet
			lcd_write(0xC0,0,1,1); 
			printf("%i. [%03i%c]  [%03is]   ", i+1, ERVPROFINT[x], 0xdf, visszaszamlalo);
			lcd_write(0x94,0,1,1);
			printf("%i. [%03i%c]  [%03is]   ", i+1, homerseklet, 0xdf, visszaszamlalo-(Tido/2));
			lcd_write(0xD4,0,1,1);
			printf("Tovabbi opcio: STOP ");
			lcd_write(0x80,0,1,1);
			Delay_ms(2);
		}
		Hofoklepes = (i+1)*2;
		Tido=0;			//Null�zzuk Tido-t az �jra sz�ml�l�shoz
	}
	T0CONbits.TMR0ON = 0; 	// Timer0 t�lt�sa
	Navigacio=6;
	return;
}

/****************************************************************************
+							"stop()" f�ggv�ny
+				Forraszt�si program k�nyszer�tett le�ll�t�sa
*****************************************************************************/
void stop(void){
	unsigned char keyval;					//Gombsor figyel�se
	
	//H�t�s vez�rl�se
	ventillator1(5);						//Maximum ventill�tor f�rdulat
	AlsoFutesRele = 0;						//Als�f�t�s t�lt�sa
	FelsoFutesRele = 0;						//Fels� f�t�s t�lt�sa

	while(Max6675(3)>80){
		keyval = get_key(KeyNumeric);		//Gombsor figyel�se
		if(keyval != '#'){
			if(keyval == 'c'){				//Vissza t�r a men�be
				ventillator1(0);			//Ventill�tor le�ll�t�sa
				Navigacio=1;
				break;
			}
		}
		printf("%s                  ", ERVPROFCHAR);	//Ki�rjuk a nevet
		lcd_write(0xC0,0,1,1); 
		printf("Hutes: %03i%c       ", Max6675(3), 0xdf);
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("Tovabbi opcio: MENU ");
		lcd_write(0x80,0,1,1);
		Delay_ms(20);
	}
	ventillator1(0);	//Ventill�tor le�ll�t�sa
	Navigacio=1;
}
