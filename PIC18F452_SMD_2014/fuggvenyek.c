/*********************************************
+		Statikus v�ltoz�k
**********************************************/
static unsigned char alfanum[21][5] = { {'x'},	//F1
										{'y'},	//F2
										{'#'},	//?
										{'*'},	//?
										
										{'1'},
										{'2','A','B','C'},
										{'3','D','E','F'},
										{'f'},	//Fel
										
										{'4','G','H','I'},
										{'5','J','K','L'},
										{'6','M','N','O'},
										{'l'},	//Le
										
										{'7','P','R','S'},
										{'8','T','U','V'},
										{'9','W','X','Y'},
							 			{'s'},	//ESC
										
										{'b'},	//Ballra - DEL
										{'0','Q','Z',' '},
										{'j'},	//Jobbra
										{'e'}	//Enter - OK
										
									};

const rom static unsigned char szovegek[59][21] = {     {"Profil nev:         "},
														{"Kezdo hofok:        "},
														{"Kezdo ido:          "},
														{"Hofok 2:            "},
														{"Program ido 2:      "},
														{"Hofok 3:            "},
														{"Program ido 3:      "},
														{"Hofok 4:            "},
														{"Program ido 4:      "},
														{"Hofok 5:            "},
														{"Program ido 5:      "},
														{"Kifuto hofok:       "},
														{"Kifuto ido:         "},
														{"Profil letrehozva!  "},
			
														{"  DPTP SOLDERING    "},//14
														{"   BGASZERVIZ.HU    "},
														{"  VERZIO SZAM: 3.0  "},
														{" Copyright DPTP 14' "},
		
														{"Utolso elmentett    "},//18
														{"profil betoltese... "},
														
														{"                    "},//20
														
														{"Hiba: Memoria, meg  "},//21
														{"nincs v. hibas adat!"},
														
														{"Hiba: Eeprom, nincs "},//23
														{"ertekelheto adat!   "},
														
														{"Hiba: Nincs profil  "},//25
														{"betoltve!           "},
														
														{"Hiba: Memoria       "},//27
														{"megtelt! Max. 15    "},
		
														{"Karakter valaszto   "},//29
														
														{"Tovabbi opcio: [ESC]"},//30
														{"Prog inditasa: [ENT]"},//31
														
														{"[1] Uj prof. hozzaad"},//32
														{"[2] Akt.prof. szerk."},
														{"[3] Profilok listaja"},
														{"[4] Akt.program [->]"},
														{"[F2] SMD Forrasztas "},
														{"  [<-]        [->]  "},//37
		
														{"Adat eltarolva!     "},//38
														{"Hiba: 1 <-> 255     "},//39
														{"Hiba: 1 <-> 450     "},//40
														{"Hiba: Nem szam!     "},//41
														{"Profilnev tarolva!  "},//42
														{"Hiba:Min 3 karakter!"},//43
		
														{"Adatok elmentve!    "},//44
														{"Profil betoltese    "},//45
		
														{"Hiba: Ervenytelen   "},//46
														{"adatblock!          "},//47
		
														{"  [ESC]       [->]  "},//48
														
														{" [1]    [2]    [3]  "},//49
														{" 5x5   10x10  15x15 "},
														{"    [4]     [5]     "},
														{"   20x20   30x30    "},
		
														{"   Kerem varjon!    "},//53
														{"  Stop:      [ESC]  "},
		
														{"   SMD Forraszto    "},//55
														{"        DEMO        "},
														{"  Fejlesztes alatt  "},
														{"BGA forrasztas: [F1]"}
};

/*********************************************
+		K�sleltet�s
**********************************************/
void Delay_ms(unsigned int ms){ //Bemenet*ms v�rakoz�s
   unsigned int i=0;
   for (i=0;i<=ms;i++) Delay1KTCYx(10);
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
		case 1:			//f�rdulat 492rpm (1v) (205)
		SetDCPWM1(205);
		break;
		case 2:			//f�rdulat 492rpm (1v) (205)
		SetDCPWM1(350);
		break;
		case 3:			//f�rdulat 492rpm (1v) (205)
		SetDCPWM1(550);
		break;
		case 4:			//f�rdulat 984rpm (2v) (410)
		SetDCPWM1(650);
		break;
		case 5:			//f�rdulat 1474rpm (3v) (614)
		SetDCPWM1(750);
		break;
		case 6:			//f�rdulat 1963rpm (4v) (818)
		SetDCPWM1(850);
		break;
		case 7: 		//Max f�rdulat 2450rpm (5v) (1023)
		SetDCPWM1(950);
		break;
		case 8: 		//Max f�rdulat 2450rpm (5v) (1023)
		SetDCPWM1(1023);
		break;
		default:		//Ha az �rt�k hib�s akkor a ventill�tor nem indul
		SetDCPWM1(0);
	}
}

/*********************************************
+		PWM Veltill�tor 2
**********************************************/
/*void ventillator2(unsigned int speed){
	OpenPWM2(0xff);
	SetDCPWM2(speed);
}*/

/*********************************************
+		LCD T�rl�s
**********************************************/
void lcd_clear(void){
	lcd_write(0x01,0,1,1);
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
	Delay_ms(1);
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
	unsigned char flag;
	for(i=0; i<5; i++){
		
		ShiftBitStep(i);	//L�ptetj�k a sorokat
		//SOROK = 0x10>>i; 	//L�ptetj�k a sorok szintj�t

		if(k!=0) k++;
		if(oszlop1){
			if(alfavagynum){				//ha 1 akkor alfanumerikus gombosort v�runk
				return get_alfa_value(k);
			}else{							//ha nulla akkor numerikus gomsort v�runk
				return get_num_value(k);
			}
		}
		Delay_ms(3);
		k++;
		if(oszlop2){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		Delay_ms(3);
		k++;
		if(oszlop3){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		Delay_ms(3);
		k++;
		if(oszlop4){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
	}
	return 0xFF;
}

/*Ellen�riz�k gombon bel�l melyik sz�mot v�lasztjuk (numerikus)*/
unsigned char get_num_value(unsigned char key){
	unsigned char result;
	if(oszlop1){
		while(oszlop1); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}else if(oszlop2){
		while(oszlop2); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}else if(oszlop3){
		while(oszlop3); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}else if(oszlop4){
		while(oszlop4); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}
	Buzzer(1, 10);
	return result;
}

/*Ellen�riz�k gombon bel�l melyik karaktert v�lasztjuk (alfanumerikus)*/
unsigned char get_alfa_value(unsigned char key){
	unsigned char i = 50, x = 0, result;
	
	if(oszlop1){
		if(key!=0 && key!=4 && key!=16){	//Ha F1-est vagy Ball gombot nyomtunk �tugrik az else �gra �s azonna vissza t�r
			do{
				if(oszlop2 | oszlop3 | oszlop4) break;
				if(oszlop1){
					while(oszlop1); //V�rakozunk a gomb felenged�s�ig
					
					RomStringWrite(szovegek[29]);
					lcd_write(0xC0,0,1,1);	
					printf("  ~[%c]             ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("%s                  ", alfanum[key]);
					lcd_write(0xD4,0,1,1);
					RomStringWrite(szovegek[20]);
					lcd_write(0x80,0,1,1);
					
					result = alfanum[key][x];
					
					if(x==3){	//H�romig emelj�k x �rt�k�t majd null�zuk (K�rbeforgatjuk a tomb tartam�t)
						x=0;
					}else{
						x++;
					}
				}
				Delay_ms(60);
			}while(i--);	//V�rakozik h�tha �rkezik ebben a ciklusid�ben tov�bbi k�r�s
		}else{
			while(oszlop1); //V�rakozunk a gomb felenged�s�ig
			result = alfanum[key][0];
		}
	}else if(oszlop2){	
		if(key!=1){	//Ha F2-es gombot nyomtunk �tugrik az else �gra �s azonna vissza t�r
			do{
				if(oszlop1 | oszlop3 | oszlop4) break;
				if(oszlop2){
					while(oszlop2); //V�rakozunk a gomb felenged�s�ig
						
					RomStringWrite(szovegek[29]);
					lcd_write(0xC0,0,1,1);	
					printf("  ~[%c]             ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("%s                  ", alfanum[key]);
					lcd_write(0xD4,0,1,1);
					RomStringWrite(szovegek[20]);
					lcd_write(0x80,0,1,1);
						
					result = alfanum[key][x];
					
					if(x==3){	//H�romig emelj�k x �rt�k�t majd null�zuk (K�rbeforgatjuk a tomb tartam�t)
						x=0;
					}else{
						x++;
					}
				}
				Delay_ms(60);
			}while(i--);	//V�rakozik h�tha �rkezik ebben a ciklusid�ben tov�bbi k�r�s
		}else{
			while(oszlop2); //V�rakozunk a gomb felenged�s�ig
			result = alfanum[key][0];
		}
	}else if(oszlop3){
		if(key!=2 && key!=18){	//Ha # vagy Jobbra gombot nyomtunk azonnal az else �gra ugrik �s vissza t�r
			do{
				if(oszlop1 | oszlop2 | oszlop4) break;
				if(oszlop3){
					while(oszlop3); //V�rakozunk a gomb felenged�s�ig
							
					RomStringWrite(szovegek[29]);
					lcd_write(0xC0,0,1,1);	
					printf("  ~[%c]             ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("%s                  ", alfanum[key]);
					lcd_write(0xD4,0,1,1);
					RomStringWrite(szovegek[20]);
					lcd_write(0x80,0,1,1); 
						
					result = alfanum[key][x];
					
					if(x==3){	//H�romig emelj�k x �rt�k�t majd null�zuk (K�rbeforgatjuk a tomb tartam�t)
						x=0;
					}else{
						x++;
					}
				}
				Delay_ms(60);
			}while(i--);	//V�rakozik h�tha �rkezik ebben a ciklusid�ben tov�bbi k�r�s
		}else{
			while(oszlop3); //V�rakozunk a gomb felenged�s�ig
			result = alfanum[key][0];
		}
	}else if(oszlop4){
		while(oszlop4); //V�rakozunk a gomb felenged�s�ig
		result = alfanum[key][0];
	}
	Buzzer(1, 10);
	return result;
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
		if(keyval=='e'){
			//Start
			Navigacio=5;
			break;
		}else if(keyval=='s'){
			//goto menu; 	//Ugr�s a men�re
			Navigacio=0;
			break;
		}
		//ERVPROFCHAR n�v t�rol� adat t�mb
		printf("%s (%03i)", ERVPROFCHAR, Max6675());	//Ki�rjuk a nevet
		lcd_write(0xC0,0,1,1);
	   	
		//ERVPROFINT h�profil t�rol� ki�r�sa
		if(ciklus%20==0 && i<PINTSIZE){
			printf("%i. %03i%c %03imp      ", x, ERVPROFINT[i], 0xdf, ERVPROFINT[i+1]);
			if(x<6){
				i++; i++; x++;
			}else{
				i=0; x=1;
			}
		}

		lcd_write(0x94,0,1,1);
		
		RomStringWrite(szovegek[30]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[31]);
		lcd_write(0x80,0,1,1);
		
	ciklus++;
	Delay_ms(WhileTime);
	//Biztos�t�k a f�t�test kikapcsol�s�ra �s a h�t�sre
	Utanhutes_relefigyeles();
	}//while
}

/****************************************************************************
+						"menu()" f�ggv�ny
+					A men� kezel�s�t teszi lehet�v�
*****************************************************************************/
void menu(void){
	unsigned char keyval;
	unsigned char lap = 0;	// Men� lap
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
			Navigacio=1;
			break;
		}else if(keyval=='y'){
			//SMD forraszt�
			Navigacio=7;
			break;
		}else if(keyval=='j' || keyval=='b'){		//Lapoz�s jobbra vagy ballra
			lap = lap==0 ? 1 : 0;
		}
		if(lap==0){
			lcd_clear();
			RomStringWrite(szovegek[32]);
			lcd_write(0xC0,0,1,1);   
			RomStringWrite(szovegek[33]);
			lcd_write(0x94,0,1,1);
			RomStringWrite(szovegek[34]);
			lcd_write(0xD4,0,1,1);
			RomStringWrite(szovegek[35]);
			lcd_write(0x80,0,1,1);
		}else if(lap==1){
			lcd_clear();
			RomStringWrite(szovegek[36]);
			lcd_write(0xC0,0,1,1);   
			RomStringWrite(szovegek[20]);
			lcd_write(0x94,0,1,1);
			RomStringWrite(szovegek[20]);
			lcd_write(0xD4,0,1,1);
			RomStringWrite(szovegek[37]);
			lcd_write(0x80,0,1,1);
		}

		Delay_ms(WhileTime);
		//Biztos�t�k a f�t�test kikapcsol�s�ra �s a h�t�sre
		Utanhutes_relefigyeles();
		LED_P = ! LED_P;
	}
}

/****************************************************************************
+						"uj_profil()" f�ggv�ny
+						�j profil l�trehoz�s�hoz
*****************************************************************************/
void uj_profil(unsigned char mod){
	unsigned char buffer[PMEMBLOCK+1] = {0};		//Buffer mem�ria
	unsigned char text[PCHARSIZE+1] = {0};			//Ideiglenes karaktereket t�rolunk benne (4x4 matrix)
	unsigned char i=0, x=0, count=0, cursor=0xFF, kitoltes, ptleptet=0;
	unsigned char adatcount=PCHARSIZE;				//Adatok ment�s�nek kezdete a PCHARSIZE. b�jtt�l
	unsigned char keyval;							//gomb �rt�k�t t�rolja
	union16 bytes, MemCim;							//16bites adat kett�v�laszt�s�hoz
	unsigned char fljelzo=0, stop=1;				//Felt�lt�st jelz� v�ltoz�
	
	bytes.value = 0;
	MemCim.value = 0;
	lcd_clear();
	while(1){
		
		//Szerkeszt�sn�l felel�s az adatok felt�lt�s��rt
		if(mod==1){
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
		}
			
		keyval = ptleptet > 0 ? get_key(KeyNumeric) : get_key(KeyAlfanum);	//Alfanumerikus gombok enged�lyez�se
		
		if(keyval != 0xFF){		//Ha a felt�tel igaz akkor t�rt�nik gombnyom�s
			if(keyval == 'b'){	//DELL gomb megnyom�s�n�l k�vetkezik be
				if(mod==1){	//Ha szerkeszt�s van
					i= i==0 ? i=strlen(text) : i;
				}
				if(i!=0 && strlen(text)>0){	//Ha van mit t�r�lni
					i--;
					text[i] = 0;	//"\0"-val jelezz�k a t�mb v�g�t (gyakorlatilag t�rl�nk a t�mb v�g�r�l visszafel� egyet)
				}
			}else if(keyval == 'e'){	//OK gomb megnyom�s�n�l k�vetkezik be
				//Ha a felt�tel igaz, ment�nk... (itt majd a mem�ri�ba fogunk �rni)
				if(ptleptet > 0){	//Ha igaz akkor a nevet elmentett�k
					if(szamvagykarakter(text)){	//Ellen�riz�k, hogy biztos csak sz�mot k�ldtek
						
						if(ptleptet%2==0){
							// 8bites adat
							if(atoi(text)<256 && atoi(text)>0){
								buffer[adatcount] = (char)atoi(text);
								RomStringWrite(szovegek[38]);
								adatcount++;
								ptleptet++;
								fljelzo++; stop=1;	//Szerkeszt�sn�l emelj�k felt�lt�stjelz�-t �s enged�lyezz�k a lefut�st
							}else{
								RomStringWrite(szovegek[39]);
							}
						}else{
							// 16bites adat
							if(atoi(text)<451 && atoi(text)>0){ 
								bytes.value = atoi(text);
								buffer[adatcount] = bytes.low;
								adatcount++;
								buffer[adatcount] = bytes.high;
								RomStringWrite(szovegek[38]);
								adatcount++;
								ptleptet++;
								fljelzo++; stop=1;	//Szerkeszt�sn�l emelj�k felt�lt�stjelz�-t �s enged�lyezz�k a lefut�st
							}else{
								RomStringWrite(szovegek[40]);
							}
						}
						
						// Lez�rjuk a buffert
						if(ptleptet>=PINTSIZE){
							buffer[adatcount] = '\0';	//Lez�rjuk a buffert
						}
						
					}else{
						RomStringWrite(szovegek[41]);
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
						RomStringWrite(szovegek[42]);
						ptleptet++;
						fljelzo++; stop=1;	//Szerkeszt�sn�l emelj�k felt�lt�stjelz�-t �s enged�lyezz�k a lefut�st
					}else{
						RomStringWrite(szovegek[43]);
					}
				}
				memset(text, 0, sizeof(text));	//T�r�lj�k a t�mb tartalm�t
				lcd_write(0xC0,0,1,1); 
				
				Delay_ms(1000);	//kb 1mp
				
				i=0;	//Null�zuk a t�mbn�vel� i v�ltoz�t is
			}else if(keyval == 's'){	//Kis c (c == Men�) nyom�s�n�l k�vetkezik be
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
		if(ptleptet==13){
			lcd_clear();
			
			if(mod==0){	//Ha �j profilt ment�nk
				//Ment�s mem�ri�ba
				MemCim.value = profilok_szama(0) == 0 ? 0 : PMEMBLOCK*profilok_szama(0);
				memWrite25LC640(MemCim, buffer);         //Mem�ria lap ki�r�sa
				EepromWrite(profilok_szama(0));
			}else if(mod==1){	//Ha szerkesztett profilt ment�nk
				//Ment�s mem�ri�ba
				MemCim.value = (PMEMBLOCK*EepromRead())-PMEMBLOCK;
				memWrite25LC640(MemCim, buffer);         //Mem�ria lap ki�r�sa
			}
			
			RomStringWrite(szovegek[ptleptet]);
			lcd_write(0xC0,0,1,1);  
			RomStringWrite(szovegek[44]);
			lcd_write(0x94,0,1,1);
			RomStringWrite(szovegek[45]);
			lcd_write(0xD4,0,1,1);
			betoltes();
			lcd_write(0x80,0,1,1);
			Delay_ms(500);
			
			//Bet�ltj�k az elmentett profilt
			//N�v let�rol�sa
			kitoltes=1;	//Ha nincs r� sz�ks�g
			for(i=0; i<=PCHARSIZE; i++){
				if(buffer[i]!= '\0'){
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
			x=0;
			for(i=0; i<PINTSIZE; i++){
				bytes.value = 0;
				if(i%2==1){
					ERVPROFINT[i] = buffer[PCHARSIZE+x];
				}else{
					bytes.low = buffer[PCHARSIZE+x];
					x++;
					bytes.high = buffer[PCHARSIZE+x];
					ERVPROFINT[i] = bytes.value;
				}
				x++;
			}
			
			ERVPROFINT[PINTSIZE] = '\0';	// lez�rjuk a buffert
			
			//Null�zuk a v�ltoz�kat az �jra kezd�s miatt
			ptleptet=0;
			bytes.value = 0;
			MemCim.value = 0;
			memset(text, 0, sizeof(text));			//T�r�lj�k a t�mb tartalm�t
			memset(buffer, 0, sizeof(buffer));		//T�r�lj�k a t�mb tartalm�t
			Navigacio=1;
			break;
		}else{
			RomStringWrite(szovegek[ptleptet]);
			lcd_write(0xC0,0,1,1); 
		}
		printf("%s%c                ", text, cursor);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[30]);
		lcd_write(0x80,0,1,1);
	Delay_ms(WhileTime);
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
	union16 ReadCim, Futes;
	unsigned char Membuffer[PMEMBLOCK+1] = {0}; //Profil blokk buffer
	unsigned char i, x=0, kitoltes;					//i n�vekszik, kitoltos : profilnevet �res hellyel t�lti ki ha az kevesebb mint 18karakter
	ReadCim.value = 0;
	Futes.value = 0;
	//Az Eepromban elmentett c�mz�st be�ll�tjuk
	ReadCim.value = ((PMEMBLOCK*profilszam)-PMEMBLOCK) == 0 ? 0 : (PMEMBLOCK*profilszam)-PMEMBLOCK;
	memRead25LC640(ReadCim, Membuffer);
	
	if(Membuffer[0]=='#'){
		//N�v let�rol�sa
		kitoltes=1;	//Ha nincs r� sz�ks�g
		for(i=0; i<=PCHARSIZE; i++){
			if(Membuffer[i] != '\0'){
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
		x=0;
		for(i=0; i<PINTSIZE; i++){
			Futes.value = 0;
			if(i%2==1){
				ERVPROFINT[i] = Membuffer[PCHARSIZE+x];
			}else{
				Futes.low =  Membuffer[PCHARSIZE+x];
				x++;
				Futes.high =  Membuffer[PCHARSIZE+x];
				ERVPROFINT[i] = Futes.value;
			}
			x++;
		}
			
		Futes.value = 0;	//Null�zzuk az adatstrukt�r�t
		ERVPROFINT[PINTSIZE] = '\0';	// lez�rjuk a buffert
		ReadCim.value = 0;
		memset(Membuffer, 0, sizeof(Membuffer));			//T�r�lj�k a t�mb tartalm�t
	}else{
		RomStringWrite(szovegek[46]);
		lcd_write(0xC0,0,1,1);  
		RomStringWrite(szovegek[47]);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0x80,0,1,1);
		Delay_ms(1000*2);
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
		
		if(keyval != 0xFF){
			
			if(keyval == 'j'){ //-> Lapozunk jobbra.
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
				lcd_clear();	//T�r�lj�k az LCD-t
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
			}else if(keyval == 's'){	//Kis c (c == Men�) nyom�s�n�l k�vetkezik be
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
		RomStringWrite(szovegek[48]); 
		lcd_write(0x80,0,1,1);

		Delay_ms(WhileTime);	
	}
	
}

/****************************************************************************
+							"Max6675()" f�ggv�ny
+					Max6675 olvas� f�ggv�ny, param�terei 1-3-ig
*****************************************************************************/
unsigned short int Max6675(void){
	unsigned short int temp = 0;		// 16bites v�ltoz�
	unsigned short int result = 0;		// 16bites v�ltoz�
	float fokra_valt = 0;				// Lebeg�pontos v�ltoz�
	//-- Enged�lyezz�k a chip-et
	MAX1CS = 0;       					// MAX6675-�t enged�lyezz�k
	Delay_ms(1); 						// Id�z�t�s
	// --------- Beolvassuk  a MAX6675 adatait ---------------//
	temp = spi_io(0x00);				// Olvasunk, de valamit akkor is k�ldeni kell
	result = spi_io(0x00);				// Olvasunk, de valamit akkor is k�ldeni kell
	temp = temp << 8;    				// L�ptetjuk ballra 8 bitet �s let�roljuk az els� 8bitet
	result = result + temp; 			// Vissza alak�tjuk 16bites-re az adatot
	result = result >> 3;  				// L�ptet�nk jobbra 3bitet �gy megkapjuk a 12bitet
	//-- Tiltjuk a chip-et
	MAX1CS = 1;       					// MAX6675-�t tiltjuk
	fokra_valt = result*0.25;			// �tv�lt�s fokra
	if(fokra_valt>1023){fokra_valt=0;}	// Ha h�fok nagyobb mint 1023 akkor nyilv�n hib�s az �rt�k �s null�zuk
	return (int)fokra_valt;				// Visszat�r�nk a fokkal
}

/****************************************************************************
+							"hofokatlagolo()" f�ggv�ny
+					Elindd�l a program �s a forraszt�s
*****************************************************************************/
//H�fok �tlagol�
unsigned short int hofokatlagolo(unsigned short int foktomb[AtlagTMeret+1], unsigned char tsize){
    unsigned char i;
    unsigned short int osszegzes = 0;
    float atlag;
    
    for(i=0; i<tsize; i++){
        osszegzes += foktomb[i];
    }
    atlag = (float)osszegzes / tsize;
    return (unsigned short int)atlag;
}

/****************************************************************************
+							"start()" f�ggv�ny
+					Elindd�l a program �s a forraszt�s
*****************************************************************************/
void start(void){
	unsigned char i, x, visszaszamlalo;		//Bels� l�ptet�shez
	unsigned char keyval;					//Gombsor figyel�se
	unsigned short int homerseklet = 0;				//H�m�rs�kletet fogja t�rolni
	unsigned char Fej=0;					//Fej kiv�laszt�sa
	unsigned char figyelmeztet=1;			//Figyelmeztet�s utols� 5. m�sodpercn�l
	unsigned short int WhileCount = 0;
	unsigned short int Max6675Buff = 0;

	//Glob�lis v�ltoz�k
	//ERVPROFCHAR[14] n�v t�rol� 
	//ERVPROFINT[12] Profil adat t�rol�
	
	Tido = 0;								//timer0 id� m�r�s�hez
	Hofoklepes = 0;							//H�fokot l�ptetj�k glob�lisan a timer0 miatt
	
	lcd_clear();
	
	//Fejm�ret kiv�laszt�sa
	while(Fej == 0){							//Ha fej m�g nincs kiv�lasztva
		keyval = get_key(KeyNumeric);			// Gombsor figyel�se
		if(keyval!=0xFF){
			if(keyval=='1'){
				//Ventill�tor ind�t�sa (1-5v) (PWM 1)
				ventillator1(3);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='2'){
				ventillator1(4);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='3'){
				ventillator1(5);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='4'){
				ventillator1(6);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}else if(keyval=='5'){
				ventillator1(7);
				Fej=1;	//Ezzel a ciklusb�l kil�p
			}
		}
		RomStringWrite(szovegek[49]);
		lcd_write(0xC0,0,1,1); 
		RomStringWrite(szovegek[50]);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[51]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[52]);
		lcd_write(0x80,0,1,1);
	Delay_ms(WhileTime);
	}
	
	lcd_clear();
	RomStringWrite(szovegek[53]);
	lcd_write(0xC0,0,1,1); 
	RomStringWrite(szovegek[20]);
	lcd_write(0x94,0,1,1);
	RomStringWrite(szovegek[20]);
	lcd_write(0xD4,0,1,1);
	RomStringWrite(szovegek[20]);
	lcd_write(0x80,0,1,1);
	Delay_ms(1000*4);			// V�rakozunk venntill�tor elidul�sig (~4mp)

	INTCONbits.GIE = 1;			// Megszak�t�sok enged�lyez�se

	//timer ind�t�sa
	/*65536-1/(1*(4/40000000)*256)
	= 26 473 0x6769
	65536-1/(2*(4/40000000)*256)
	= 46 005 0xB3B5
	65536-1/(3*(4/40000000)*256)
	= 52 515 0xCD23
	65536-1/(4*(4/40000000)*256)
	= 55 770 0xD9DA
	65536-1/(5*(4/40000000)*256)
	= 57 723 0xE17B
	65536-1/(6*(4/40000000)*256)
	= 59 026 0xE692
	65536-1/(7*(4/40000000)*256)
	= 59 956 0xEA34
	65536-1/(8*(4/40000000)*256)
	= 60 653 0xECED
	65536-1/(9*(4/40000000)*256)
	= 61 196 0xEF0C
	65536-1/(10*(4/40000000)*256)
	= 61 630 0xF0BE*/

	Timer0Count = 0;		// Null�zuk timer0 sz�ml�l�t
	TMR0H = 0x67;
	TMR0L = 0x69;
	INTCONbits.T0IF = 0;    // Timer0 t�lcsordul�sjelz�j�t t�r�lj�k
	INTCONbits.T0IE = 1;    // Timer0 megszak�t�sainak enged�lyez�se
	T0CONbits.TMR0ON = 1;	// Timer0 ind�t�sa
	
	lcd_clear();
	
	//Program elindul
	//Addig fut ameddig Hofoklepes kisebb mint maximum 6 l�p�s (0-5)
	for(i=0; i<6; i++){
		
		//Hang visszajelz�s profill�p�senk�nt
		Buzzer(2, 10);
		
		x = i==0 ? 0 : i*2;
		visszaszamlalo = ERVPROFINT[x+1];		// Vissza sz�ml�l�ba bet�ltj�k az id�t
		while(ERVPROFINT[x+1]>Tido){			// Addig fut ameddig Tido le nem j�r
			keyval = get_key(KeyNumeric);		// Gombsor figyel�se
			if(keyval != 0xFF){
				if(keyval == 's'){				// Program le�ll�t�sa
					T0CONbits.TMR0ON = 0; 		// Timer0 t�lt�sa
					FelsoFutesRele = 0;			// Fels�f�t�s let�lt�sa
					Navigacio=6;
					return;
				}
			}

			Max6675Buff = Max6675();
			//�tlagol� ind�t�sa
			/****************************************
			+		H�fok adatok �tlagol�s�hoz		+
			****************************************/
			if(AtlagBTCount<AtlagTMeret){								// Ha �tlag sz�ml�l� kissebb mint 4
				HoBufferTomb[AtlagBTCount] = Max6675Buff;					// Elt�roljuk a h�mint�t
				if(homerseklet>1023 && homerseklet<=0){					// Ha m�g nincs kijelezhet� h�minta
					homerseklet=Max6675Buff;
				}
			}else if(AtlagBTCount==AtlagTMeret){						// Ha �tlag sz�ml�l� nagyobb mint 4
				HoBufferTomb[AtlagTMeret]='\0';							// Lez�rjuk a t�mb�t
				homerseklet = hofokatlagolo(HoBufferTomb, AtlagTMeret);	// �tlagoljuk a be�rkezett h�adatokat
			}

			if(AtlagBTCount > AtlagTMeret){
				AtlagBTCount = 0;
			}else{
				AtlagBTCount++;
			}
			
			printf("%s", ERVPROFCHAR);	//Ki�rjuk a nevet
			lcd_write(0xC0,0,1,1); 
			printf("%i. [%03i%c]  [%03is]   ", i+1, ERVPROFINT[x], 0xdf, visszaszamlalo);
			lcd_write(0x94,0,1,1);
			printf("%i. [%03i%c]  [%03is]   ", i+1, homerseklet, 0xdf, visszaszamlalo-Tido);
			lcd_write(0xD4,0,1,1);
			//printf("\"%i\"", WhileCount); 
			RomStringWrite(szovegek[54]);
			lcd_write(0x80,0,1,1);

			//Utols� profill�p�s, utols� 5. m�sodperc�ben figyelmeztet� hangot adunk
			if(i==5 && (visszaszamlalo-Tido)<6 && figyelmeztet){
				Buzzer(5, 50);
				figyelmeztet=0;
			}
		
		Delay_ms(WhileTime);
		WhileCount++;
		}
		Hofoklepes = (i+1)*2;
		Tido=0;					//Null�zzuk Tido-t az �jra sz�ml�l�shoz
	}
	T0CONbits.TMR0ON = 0; 		// Timer0 t�lt�sa
	FelsoFutesRele = 0;			// Fels�f�t�s let�lt�sa
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
	ventillator1(8);						//Maximum ventill�tor f�rdulat
	Buzzer(1, 100);							//Hossz� s�pol�ssal jelezz�k a h�t�st
	while(Max6675()>80){
		Utanhutes_relefigyeles();			//H�t�s
		keyval = get_key(KeyNumeric);		//Gombsor figyel�se
		if(keyval != 0xFF){
			if(keyval == 's'){				//Vissza t�r a men�be
				ventillator1(0);			//Ventill�tor le�ll�t�sa
				Navigacio=1;
				break;
			}
		}
		printf("%s                  ", ERVPROFCHAR);	//Ki�rjuk a nevet
		lcd_write(0xC0,0,1,1); 
		printf("Hutes: %03i%c       ", Max6675(), 0xdf);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[30]);
		lcd_write(0x80,0,1,1);
	Delay_ms(WhileTime);
	}
	ventillator1(1);	//Ventill�tor legals� �llapotba �ll�tva kil�phet�nk a programb�l
	Navigacio=1;
}

// T�lt�s jelz�
void betoltes(void){
	unsigned char i;
	for(i=0; i<20; i++){
		_user_putc(0xFF);
		Delay_ms(20);
	}
} 

//Hang visszajez� Buzzer
 void Buzzer(unsigned char jelszam, unsigned short int ms){
	 unsigned char i;
	 for(i=0; i<jelszam; i++){
		 BUZZER = 1;
		 Delay_ms(ms);
		 BUZZER = 0;
		 Delay_ms(ms);
	}
}

//Shift Register l�ptet�se
void ShiftBitStep(unsigned char StepCount){
	unsigned char Shiftdata, i;
	Shiftdata = 0x80>>StepCount;
	for(i=0; i<8; i++){
		if((Shiftdata>>i)&1==1){
			SBIT(1);
		}else{
			SBIT(0);
		}
		Delay_ms(2);
	}
}

//Ut�nh�t�s �s rel� figyel�se
void Utanhutes_relefigyeles(void){
	//Biztos�t�kk�nt ellen�rizz�k, hogy a f�t�test ki e van kapcsolva
	if(FelsoFutesRele==1){ 	// ha be van kapcsolva
		FelsoFutesRele=0;	// F�t�test t�lt�sa
	}
	if(Max6675()>300){
		ventillator1(8);
	}else if(Max6675()>200 && Max6675()<300){
		ventillator1(6);
	}else if(Max6675()>100 && Max6675()<200){
		ventillator1(4);
	}else if(Max6675()>80 && Max6675()<100){
		ventillator1(2);
	}else if(Max6675()>50){
		ventillator1(1);
	}else{
		ventillator1(0);
	}
}

/****************************************************************************
+						"SMD_forraszto()" f�ggv�ny
+					SMD leveg�s forraszt� program
*****************************************************************************/
void SMD_forraszto(void){
    unsigned char keyval;
	lcd_clear();
	LowInterruptIni();			// Alacsony pior�t�s� interrupt enged�lyez�se
	INTCONbits.GIE = 1;			// Megszak�t�sok enged�lyez�se

	//timer ind�t�sa
	/*A timernek mindig 65535-46875=18660-r�l kell indulnia, (~1mp) hogy j�kor csorduljon t�l �s k�rje a megszak�t�st.
	Ezt az �rt�ket minden megszak�t�skor(r�gt�n az elej�n) bele kell �rni a TMR0L �s TMR0H regiszterekbe.
	Mivel ezek a regiszterek egyenk�nt 8 bitesek, sz�t kell bontani.
	18660 bin�risan: 01001000 11100100, vagyis
	18660 hex�ban: 	 0x48	  0xE4
	F�lm�sodperces mintav�telez�s 18660*2: 10010001 11001000
	Hex�ban: 0x91	0xC8
	Harmadm�sodperces mintav�telez�s 18660*3: 11011010 10101100
	Hex�ban: 0xDA 0xAC*/

	Timer0Count = 0;		// Null�zuk timer0 sz�ml�l�t
	TMR0H = 0x67;
	TMR0L = 0x69;
	INTCONbits.T0IF = 0;    // Timer0 t�lcsordul�sjelz�j�t t�r�lj�k
	INTCONbits.T0IE = 1;    // Timer0 megszak�t�sainak enged�lyez�se
	T0CONbits.TMR0ON = 1;	// Timer0 ind�t�sa

	while(1){
		keyval = get_key(KeyNumeric);	//numerikus gombok enged�lyez�se
		if(keyval != 0xFF){
			if(keyval=='x'){
				//�j profil hozz�ad�sa
				Navigacio=0;
				break;
			}
		}
		RomStringWrite(szovegek[55]);
		lcd_write(0xC0,0,1,1); 
		RomStringWrite(szovegek[56]);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[57]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[58]);
		lcd_write(0x80,0,1,1);
		Delay_ms(WhileTime);
		//Biztos�t�k a f�t�test kikapcsol�s�ra �s a h�t�sre
		Utanhutes_relefigyeles();
	Delay_ms(WhileTime);
	}
}
