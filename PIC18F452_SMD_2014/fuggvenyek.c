/*********************************************
+		Statikus változók
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
+		Késleltetés
**********************************************/
void Delay_ms(unsigned int ms){ //Bemenet*ms várakozás
   unsigned int i=0;
   for (i=0;i<=ms;i++) Delay1KTCYx(10);
}

/*********************************************
+		PWM PS3 ventillátor beállítása
+		Egység: (volt*1000)/1023 (10bit-es felbontás) = 1 egység == 4,89
+		Volt: 1 egység / 1000 = volt == 0,00489v
+		Fordulat: 2450/1023 = 2.4 (1 egységnyi idõ alatt 2.4 fordulat, Maximum 2450RPM (12v))
+		Minimum 0.7335v == 150 (10bit-es felbontásban) == 360rpm
+		PWM1 = RC2 láb
+		PWM2 = RC1 láb
**********************************************/
void ventillator1(unsigned int szint){
	OpenTimer2( TIMER_INT_OFF & T2_PS_1_4 & T2_POST_1_8 );
	OpenPWM1(0xff);
	switch(szint){
		case 0:			//Nincs forgás 0rpm (0v)
		SetDCPWM1(0);
		break;
		case 1:			//fórdulat 492rpm (1v) (205)
		SetDCPWM1(205);
		break;
		case 2:			//fórdulat 492rpm (1v) (205)
		SetDCPWM1(350);
		break;
		case 3:			//fórdulat 492rpm (1v) (205)
		SetDCPWM1(550);
		break;
		case 4:			//fórdulat 984rpm (2v) (410)
		SetDCPWM1(650);
		break;
		case 5:			//fórdulat 1474rpm (3v) (614)
		SetDCPWM1(750);
		break;
		case 6:			//fórdulat 1963rpm (4v) (818)
		SetDCPWM1(850);
		break;
		case 7: 		//Max fórdulat 2450rpm (5v) (1023)
		SetDCPWM1(950);
		break;
		case 8: 		//Max fórdulat 2450rpm (5v) (1023)
		SetDCPWM1(1023);
		break;
		default:		//Ha az érték hibás akkor a ventillátor nem indul
		SetDCPWM1(0);
	}
}

/*********************************************
+		PWM Veltillátor 2
**********************************************/
/*void ventillator2(unsigned int speed){
	OpenPWM2(0xff);
	SetDCPWM2(speed);
}*/

/*********************************************
+		LCD Törlés
**********************************************/
void lcd_clear(void){
	lcd_write(0x01,0,1,1);
}

/*********************************************
+		Eeprom olvasása
**********************************************/
unsigned char EepromRead (void){
	unsigned char buffer;
	EEADR = 0;				// Memória címtõl kezdi az olvasást
	EECON1bits.EEPGD = 0;	// Flash Program vagy adat EEPROM memória vélasztó bit
							// 1 = Hozzáférés a flash program memóriához
							// 0 = Hozzáférés az EEPROM memóriához
	EECON1bits.CFGS = 0;	// Flash Program / adat EEPROM vagy konfiguráció választó bit
							// 1 = Hozzáférés a konfigurációs nyilvántartásokhoz
							// 0 = Hozzáférés a Flash program vagy adat EEPROM memóriához	
	EECON1bits.RD = 1;		// Olvasást vezérlõ bit
							// 1 = kezdeményezi az EEPROM olvasását (RD bitet nem lehet beállítani , ha EEPGD = 1 vagy CFGS = 1 )
							// 0 = Nem kezdeményez EEPROM olvasás
	buffer = EEDATA;		// Kiolvassuk az adatot
	return buffer;
}

/*********************************************
+		Eeprom írása
**********************************************/
void EepromWrite (unsigned char adat){
	EEADR = 0;				// Memória címtõl kezdjük az írást
	EEDATA = adat;
	EECON1bits.EEPGD = 0;	// Flash Program vagy adat EEPROM memória vélasztó bit
							// 1 = Hozzáférés a flash program memóriához
							// 0 = Hozzáférés az EEPROM memóriához
	EECON1bits.CFGS = 0;	// Flash Program / adat EEPROM vagy konfiguráció választó bit
							// 1 = Hozzáférés a konfigurációs nyilvántartásokhoz
							// 0 = Hozzáférés a Flash program vagy adat EEPROM memóriához
	EECON1bits.WREN = 1;	// Flash Program / adat EEPROM írást engedélyezõ bit
							// 1 = Engedélyezi a Flash program / adat EEPROM írását
							// 0 = Gátolja a Flash program / adat EEPROM írását
	INTCONbits.GIE = 0;		//Tiltjuk a interrupt-ot (megszakítást)
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;		// Írást vezérlõ bit
							// 1 = kezdeményez adat EEPROM törlés / írás ciklust vagy a program memória törlési ciklust vagy írási ciklus
							// 0 = Write ciklus az EEPROM teljes
	while (EECON1bits.WR == 1);	//Várakozunk az írás befejezéséig
	INTCONbits.GIE = 1;		// engedélyezzük az interrupt-ot (megszakítást)
	EECON1bits.WREN = 0;	// Flash Program / adat EEPROM írást engedélyezõ bit
							// 1 = Engedélyezi a Flash program / adat EEPROM írását
							// 0 = Gátolja a Flash program / adat EEPROM írását
}

/*
EECON1bits.EEPGD = 0;	// Flash Program vagy adat EEPROM memória vélasztó bit
						// 1 = Hozzáférés a flash program memóriához
						// 0 = Hozzáférés az EEPROM memóriához
EECON1bits.CFGS = 0;	// Flash Program / adat EEPROM vagy konfiguráció választó bit
						// 1 = Hozzáférés a konfigurációs nyilvántartásokhoz
						// 0 = Hozzáférés a Flash program vagy adat EEPROM memóriához					
//bit 5 Unimplemented: Read as '0'
EECON1bits.FREE = 0;	// flash törlésének engedélyezése
						// 1 = Törlés engedélyezése
						// 0 = Csak olvasás
EECON1bits.WRERR = 0;	// Flash Program / adat EEPROM hiba jelzõ bit
						// 1 = írási mûvelet idõ elõtt megszûnik ( reset során önálló idõzített programozás normál mûködés , vagy nem megfelelõ írási kísérlet )
						// 0 = Az írási mûvelet befejezõdött
EECON1bits.WREN = 0;	// Flash Program / adat EEPROM írást engedélyezõ bit
						// 1 = Engedélyezi a Flash program / adat EEPROM írását
						// 0 = Gátolja a Flash program / adat EEPROM írását
EECON1bits.WR = 0;		// Írást vezérlõ bit
						// 1 = kezdeményez adat EEPROM törlés / írás ciklust vagy a program memória törlési ciklust vagy írási ciklus
						// 0 = Write ciklus az EEPROM teljes
EECON1bits.RD = 0;		// Olvasást vezérlõ bit
						// 1 = kezdeményezi az EEPROM olvasását (RD bitet nem lehet beállítani , ha EEPGD = 1 vagy CFGS = 1 )
						// 0 = Nem kezdeményez EEPROM olvasás
//megjegyzés : Ha hiba ( WRERR ) történik, a EEPGD és CFGS bit nem törlõdik ez álltal lehetõvé teszi a hiba nyomon követését
*/


/*********************************************
+		SPI 25LC640 memória
**********************************************/
unsigned char spi_io(unsigned char adat){
	unsigned char tmp;  
	SSPSTATbits.BF = 0;			// törli foglaltság jelzõ álapotát a státusz regiszterben
	SSPCON1bits.WCOL = 0;		// törli az esetleges írás ütközés hibajelzõt
	tmp = SSPBUF;				// törli a BF jelzõbitet
	SSPBUF = adat;				// kirakja a kimenõ adatot az SSPBUF regiszterbe
	while(!SSPSTATbits.BF);		// megvárjuk a busz ciklus végét 
	return (SSPBUF);			// a vett bájttal térünk vissza
}

void waitFor25LC640(void) {
	unsigned int flag;
	do {
		SPI_CS = 0;                      	    //kiadjuk a Chip Enable jelet
		spi_io(CMD_RDSR);                       //Státuszregiszter olvasása parancs
		flag = spi_io(0x00);                    //Olvasunk, de valamit akkor is küldeni kell!
		SPI_CS = 1;                   		    //megszüntetjük a Chip Enable jelet
	} while (flag & 0x01); 
}

void memWrite25LC640(union16 MemCim, unsigned char *pbuf) {
	unsigned char i,flag;
	waitFor25LC640();                         //Várunk, ha az EEPROM elfoglalt
	SPI_CS = 0;
	WriteSPI(CMD_WREN);                       //Írás újraengedélyezése
	SPI_CS = 1;
	Delay_ms(1);
	SPI_CS = 0;
	spi_io(CMD_WRITE);                        //Adatblokk írása
	spi_io(MemCim.low);
	spi_io(MemCim.high);
	for (i=0; i < PMEMBLOCK; i++) {
		spi_io(*pbuf++);                      //Az adatbuffer kiírása
	}
	SPI_CS = 1;
}

void memRead25LC640(union16 MemCim, unsigned char *pbuf) {
	unsigned char i;
	waitFor25LC640();                         //Várunk, ha az EEPROM elfoglalt
	SPI_CS = 0;
	spi_io(CMD_READ);                         //Adatblokk olvasása
	spi_io(MemCim.low);
	spi_io(MemCim.high);
	for (i=0; i < PMEMBLOCK; i++) {
		*pbuf++=spi_io(0x00);                   //Olvasunk, de valamit akkor is küldeni kell!
	}
	SPI_CS = 1;
}

/*********************************************
+		4x4 MATRIX függvények
**********************************************/
/*Elenõrizük a melyik gombt nyomtuk le*/
unsigned char get_key(unsigned char alfavagynum){
	unsigned char i, k=0, result;
	unsigned char flag;
	for(i=0; i<5; i++){
		
		ShiftBitStep(i);	//Léptetjük a sorokat
		//SOROK = 0x10>>i; 	//Léptetjük a sorok szintjét

		if(k!=0) k++;
		if(oszlop1){
			if(alfavagynum){				//ha 1 akkor alfanumerikus gombosort várunk
				return get_alfa_value(k);
			}else{							//ha nulla akkor numerikus gomsort várunk
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

/*Ellenõrizük gombon belül melyik számot választjuk (numerikus)*/
unsigned char get_num_value(unsigned char key){
	unsigned char result;
	if(oszlop1){
		while(oszlop1); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}else if(oszlop2){
		while(oszlop2); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}else if(oszlop3){
		while(oszlop3); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}else if(oszlop4){
		while(oszlop4); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}
	Buzzer(1, 10);
	return result;
}

/*Ellenõrizük gombon belül melyik karaktert választjuk (alfanumerikus)*/
unsigned char get_alfa_value(unsigned char key){
	unsigned char i = 50, x = 0, result;
	
	if(oszlop1){
		if(key!=0 && key!=4 && key!=16){	//Ha F1-est vagy Ball gombot nyomtunk átugrik az else ágra és azonna vissza tér
			do{
				if(oszlop2 | oszlop3 | oszlop4) break;
				if(oszlop1){
					while(oszlop1); //Várakozunk a gomb felengedéséig
					
					RomStringWrite(szovegek[29]);
					lcd_write(0xC0,0,1,1);	
					printf("  ~[%c]             ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("%s                  ", alfanum[key]);
					lcd_write(0xD4,0,1,1);
					RomStringWrite(szovegek[20]);
					lcd_write(0x80,0,1,1);
					
					result = alfanum[key][x];
					
					if(x==3){	//Háromig emeljük x értékét majd nullázuk (Körbeforgatjuk a tomb tartamát)
						x=0;
					}else{
						x++;
					}
				}
				Delay_ms(60);
			}while(i--);	//Várakozik hátha érkezik ebben a ciklusidõben további kérés
		}else{
			while(oszlop1); //Várakozunk a gomb felengedéséig
			result = alfanum[key][0];
		}
	}else if(oszlop2){	
		if(key!=1){	//Ha F2-es gombot nyomtunk átugrik az else ágra és azonna vissza tér
			do{
				if(oszlop1 | oszlop3 | oszlop4) break;
				if(oszlop2){
					while(oszlop2); //Várakozunk a gomb felengedéséig
						
					RomStringWrite(szovegek[29]);
					lcd_write(0xC0,0,1,1);	
					printf("  ~[%c]             ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("%s                  ", alfanum[key]);
					lcd_write(0xD4,0,1,1);
					RomStringWrite(szovegek[20]);
					lcd_write(0x80,0,1,1);
						
					result = alfanum[key][x];
					
					if(x==3){	//Háromig emeljük x értékét majd nullázuk (Körbeforgatjuk a tomb tartamát)
						x=0;
					}else{
						x++;
					}
				}
				Delay_ms(60);
			}while(i--);	//Várakozik hátha érkezik ebben a ciklusidõben további kérés
		}else{
			while(oszlop2); //Várakozunk a gomb felengedéséig
			result = alfanum[key][0];
		}
	}else if(oszlop3){
		if(key!=2 && key!=18){	//Ha # vagy Jobbra gombot nyomtunk azonnal az else ágra ugrik és vissza tér
			do{
				if(oszlop1 | oszlop2 | oszlop4) break;
				if(oszlop3){
					while(oszlop3); //Várakozunk a gomb felengedéséig
							
					RomStringWrite(szovegek[29]);
					lcd_write(0xC0,0,1,1);	
					printf("  ~[%c]             ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("%s                  ", alfanum[key]);
					lcd_write(0xD4,0,1,1);
					RomStringWrite(szovegek[20]);
					lcd_write(0x80,0,1,1); 
						
					result = alfanum[key][x];
					
					if(x==3){	//Háromig emeljük x értékét majd nullázuk (Körbeforgatjuk a tomb tartamát)
						x=0;
					}else{
						x++;
					}
				}
				Delay_ms(60);
			}while(i--);	//Várakozik hátha érkezik ebben a ciklusidõben további kérés
		}else{
			while(oszlop3); //Várakozunk a gomb felengedéséig
			result = alfanum[key][0];
		}
	}else if(oszlop4){
		while(oszlop4); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}
	Buzzer(1, 10);
	return result;
}

/*********************************************
+		Karakter vagy szám ellenõrzõ
+		1=szám (TRUE), 0=nem szám (FALSE)
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
+						"profilt_mutat()" függvény
+					A már betöltött profilt mutatja
*****************************************************************************/
void profilt_mutat(void){
	unsigned int ciklus=0, i=0, x=1;
	unsigned char keyval;
	lcd_clear();
	while(1){
		//Ha van érvényes profil betöltve akkor kiírjuk az LCD-re és indítható egybõl
		keyval = get_key(KeyNumeric);	//numerikus gombok engedélyezése
		if(keyval=='e'){
			//Start
			Navigacio=5;
			break;
		}else if(keyval=='s'){
			//goto menu; 	//Ugrás a menüre
			Navigacio=0;
			break;
		}
		//ERVPROFCHAR név tároló adat tömb
		printf("%s (%03i)", ERVPROFCHAR, Max6675());	//Kiírjuk a nevet
		lcd_write(0xC0,0,1,1);
	   	
		//ERVPROFINT hõprofil tároló kiírása
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
	//Biztosíték a fûtõtest kikapcsolására és a hûtésre
	Utanhutes_relefigyeles();
	}//while
}

/****************************************************************************
+						"menu()" függvény
+					A menü kezelését teszi lehetõvé
*****************************************************************************/
void menu(void){
	unsigned char keyval;
	unsigned char lap = 0;	// Menü lap
	while(1){
		keyval = get_key(KeyNumeric);	//numerikus gombok engedélyezése
		if(keyval=='1'){
			//Új profil hozzáadása
			Navigacio=2;
			break;
		}else if(keyval=='2'){
			//Aktuális profil szerkesztése
			Navigacio=3;
			break;
		}else if(keyval=='3'){
			//Profilok listázása
			Navigacio=4;
			break;
		}else if(keyval=='4'){
			//Ugrás a már betöltött profilra
			Navigacio=1;
			break;
		}else if(keyval=='y'){
			//SMD forrasztó
			Navigacio=7;
			break;
		}else if(keyval=='j' || keyval=='b'){		//Lapozás jobbra vagy ballra
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
		//Biztosíték a fûtõtest kikapcsolására és a hûtésre
		Utanhutes_relefigyeles();
		LED_P = ! LED_P;
	}
}

/****************************************************************************
+						"uj_profil()" függvény
+						Új profil létrehozásához
*****************************************************************************/
void uj_profil(unsigned char mod){
	unsigned char buffer[PMEMBLOCK+1] = {0};		//Buffer memória
	unsigned char text[PCHARSIZE+1] = {0};			//Ideiglenes karaktereket tárolunk benne (4x4 matrix)
	unsigned char i=0, x=0, count=0, cursor=0xFF, kitoltes, ptleptet=0;
	unsigned char adatcount=PCHARSIZE;				//Adatok mentésének kezdete a PCHARSIZE. bájttól
	unsigned char keyval;							//gomb értékét tárolja
	union16 bytes, MemCim;							//16bites adat kettéválasztásához
	unsigned char fljelzo=0, stop=1;				//Feltõltést jelzõ változó
	
	bytes.value = 0;
	MemCim.value = 0;
	lcd_clear();
	while(1){
		
		//Szerkesztésnél felelõs az adatok feltöltéséért
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
			
		keyval = ptleptet > 0 ? get_key(KeyNumeric) : get_key(KeyAlfanum);	//Alfanumerikus gombok engedélyezése
		
		if(keyval != 0xFF){		//Ha a feltétel igaz akkor történik gombnyomás
			if(keyval == 'b'){	//DELL gomb megnyomásánál következik be
				if(mod==1){	//Ha szerkesztés van
					i= i==0 ? i=strlen(text) : i;
				}
				if(i!=0 && strlen(text)>0){	//Ha van mit törölni
					i--;
					text[i] = 0;	//"\0"-val jelezzük a tömb végét (gyakorlatilag törlünk a tömb végérõl visszafelé egyet)
				}
			}else if(keyval == 'e'){	//OK gomb megnyomásánál következik be
				//Ha a feltétel igaz, mentünk... (itt majd a memóriába fogunk írni)
				if(ptleptet > 0){	//Ha igaz akkor a nevet elmentettük
					if(szamvagykarakter(text)){	//Ellenõrizük, hogy biztos csak számot küldtek
						
						if(ptleptet%2==0){
							// 8bites adat
							if(atoi(text)<256 && atoi(text)>0){
								buffer[adatcount] = (char)atoi(text);
								RomStringWrite(szovegek[38]);
								adatcount++;
								ptleptet++;
								fljelzo++; stop=1;	//Szerkesztésnél emeljük feltöltéstjelzõ-t és engedélyezzük a lefutást
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
								fljelzo++; stop=1;	//Szerkesztésnél emeljük feltöltéstjelzõ-t és engedélyezzük a lefutást
							}else{
								RomStringWrite(szovegek[40]);
							}
						}
						
						// Lezárjuk a buffert
						if(ptleptet>=PINTSIZE){
							buffer[adatcount] = '\0';	//Lezárjuk a buffert
						}
						
					}else{
						RomStringWrite(szovegek[41]);
					}
				}else{
					if(strlen(text)>2){ 
						buffer[0] = '#';
						kitoltes = 1;	//1, ha nincs rá szükség
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
						fljelzo++; stop=1;	//Szerkesztésnél emeljük feltöltéstjelzõ-t és engedélyezzük a lefutást
					}else{
						RomStringWrite(szovegek[43]);
					}
				}
				memset(text, 0, sizeof(text));	//Törõljük a tömb tartalmát
				lcd_write(0xC0,0,1,1); 
				
				Delay_ms(1000);	//kb 1mp
				
				i=0;	//Nullázuk a tömbnövelõ i változót is
			}else if(keyval == 's'){	//Kis c (c == Menü) nyomásánál következik be
				Navigacio=0;
				break;
			}else{
				if(strlen(text)<PCHARSIZE && ptleptet==0){	//Addig, amddig elérjük a maximum megengedett beírható karaktert (PROF_NAME_MAX == 18)
					text[i] = keyval;
					i++;
				}else if(ptleptet>0 && strlen(text)<3){	//Ha már a profilnév mentésre került (jöhetnek a számadatok)
					text[i] = keyval;
					i++;
				}
			}
		}
		if(count%Cursors==0) cursor = cursor==' ' ? ptleptet>0 ? ptleptet%2==0 ? 's' : 0xdf : 0xff : ' ';	//Kurzórt villogtatjuk
		if(ptleptet==13){
			lcd_clear();
			
			if(mod==0){	//Ha új profilt mentünk
				//Mentés memóriába
				MemCim.value = profilok_szama(0) == 0 ? 0 : PMEMBLOCK*profilok_szama(0);
				memWrite25LC640(MemCim, buffer);         //Memória lap kiírása
				EepromWrite(profilok_szama(0));
			}else if(mod==1){	//Ha szerkesztett profilt mentünk
				//Mentés memóriába
				MemCim.value = (PMEMBLOCK*EepromRead())-PMEMBLOCK;
				memWrite25LC640(MemCim, buffer);         //Memória lap kiírása
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
			
			//Betöltjük az elmentett profilt
			//Név letárolása
			kitoltes=1;	//Ha nincs rá szükség
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
			//Hõprofil és idõk letárolása
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
			
			ERVPROFINT[PINTSIZE] = '\0';	// lezárjuk a buffert
			
			//Nullázuk a változókat az újra kezdés miatt
			ptleptet=0;
			bytes.value = 0;
			MemCim.value = 0;
			memset(text, 0, sizeof(text));			//Törõljük a tömb tartalmát
			memset(buffer, 0, sizeof(buffer));		//Törõljük a tömb tartalmát
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
+						"profilok_szama()" függvény
+			Profilok számát adja vissza, ha 0-val paraméterezzük
+			Ha 0-nál nagyobb szám a paraméter akkor vizsgálodik, és ha 
+			van a címen érvényes adat 1-el tér vissza		
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
+						"profilt_beletolt()" függvény
+			Betölti az utoljára elmentett vagy kiválasztott profilt
*****************************************************************************/
void profilt_betolt(unsigned char profilszam){
	union16 ReadCim, Futes;
	unsigned char Membuffer[PMEMBLOCK+1] = {0}; //Profil blokk buffer
	unsigned char i, x=0, kitoltes;					//i nõvekszik, kitoltos : profilnevet üres hellyel tölti ki ha az kevesebb mint 18karakter
	ReadCim.value = 0;
	Futes.value = 0;
	//Az Eepromban elmentett címzést beállítjuk
	ReadCim.value = ((PMEMBLOCK*profilszam)-PMEMBLOCK) == 0 ? 0 : (PMEMBLOCK*profilszam)-PMEMBLOCK;
	memRead25LC640(ReadCim, Membuffer);
	
	if(Membuffer[0]=='#'){
		//Név letárolása
		kitoltes=1;	//Ha nincs rá szükség
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
		
		//Hõprofil és idõk letárolása
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
			
		Futes.value = 0;	//Nullázzuk az adatstruktúrát
		ERVPROFINT[PINTSIZE] = '\0';	// lezárjuk a buffert
		ReadCim.value = 0;
		memset(Membuffer, 0, sizeof(Membuffer));			//Törõljük a tömb tartalmát
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
+						"profil_lista()" függvény
+					Betölti az osszes érvényes profil nevét
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

	//Elsõ lap lekérése
	ReadCim.value = 0;
	while(lista<=3){
		if(sorszam>0){
			ReadCim.value += PMEMBLOCK;
		}
		memRead25LC640(ReadCim, MemBlockBuffer);
		for(i=0; i<PCHARSIZE; i++){
			if(MemBlockBuffer[0]=='#'){
				listabuffer[lista-1][i] = MemBlockBuffer[i+1];	//+1 mert nem kell az azonosító
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
				//Tömbbök törlése
				lista = 1;
				memset(MemBlockBuffer, 0, sizeof(MemBlockBuffer));
				memset(listabuffer, 0, sizeof(listabuffer));
				//Lap lekérése
				if(sorszam>=PPROGSZAM){	//A sorszámot reseteljük, ha elértük a listázható maximum PPROGSZAM-ot. (15)
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
							listabuffer[lista-1][i] = MemBlockBuffer[i+1];	//+1 mert nem kell az azonosító
						}else{
							listabuffer[lista-1][i] = ' ';
						}
						if(i==PCHARSIZE-1) listabuffer[lista-1][PCHARSIZE] = '\0';
					}
					lista++;
					sorszam++;
				}
				lcd_clear();	//Töröljük az LCD-t
			}else if(keyval == '1' || keyval == '2' || keyval == '3'){
				//Profil szám megállapítása
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
			}else if(keyval == 's'){	//Kis c (c == Menü) nyomásánál következik be
				Navigacio=0;
				break;
			}	
		}
	
		//Kiírjuk a képernyõre a listát
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
+							"Max6675()" függvény
+					Max6675 olvasó függvény, paraméterei 1-3-ig
*****************************************************************************/
unsigned short int Max6675(void){
	unsigned short int temp = 0;		// 16bites változó
	unsigned short int result = 0;		// 16bites változó
	float fokra_valt = 0;				// Lebegõpontos változó
	//-- Engedélyezzük a chip-et
	MAX1CS = 0;       					// MAX6675-öt engedélyezzük
	Delay_ms(1); 						// Idõzítés
	// --------- Beolvassuk  a MAX6675 adatait ---------------//
	temp = spi_io(0x00);				// Olvasunk, de valamit akkor is küldeni kell
	result = spi_io(0x00);				// Olvasunk, de valamit akkor is küldeni kell
	temp = temp << 8;    				// Léptetjuk ballra 8 bitet és letároljuk az elsõ 8bitet
	result = result + temp; 			// Vissza alakítjuk 16bites-re az adatot
	result = result >> 3;  				// Léptetünk jobbra 3bitet így megkapjuk a 12bitet
	//-- Tiltjuk a chip-et
	MAX1CS = 1;       					// MAX6675-öt tiltjuk
	fokra_valt = result*0.25;			// Átváltás fokra
	if(fokra_valt>1023){fokra_valt=0;}	// Ha hõfok nagyobb mint 1023 akkor nyilván hibás az érték és nullázuk
	return (int)fokra_valt;				// Visszatérünk a fokkal
}

/****************************************************************************
+							"hofokatlagolo()" függvény
+					Elinddúl a program és a forrasztás
*****************************************************************************/
//Hõfok átlagoló
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
+							"start()" függvény
+					Elinddúl a program és a forrasztás
*****************************************************************************/
void start(void){
	unsigned char i, x, visszaszamlalo;		//Belsõ léptetéshez
	unsigned char keyval;					//Gombsor figyelése
	unsigned short int homerseklet = 0;				//Hõmérsékletet fogja tárolni
	unsigned char Fej=0;					//Fej kiválasztása
	unsigned char figyelmeztet=1;			//Figyelmeztetés utolsó 5. másodpercnél
	unsigned short int WhileCount = 0;
	unsigned short int Max6675Buff = 0;

	//Globális változók
	//ERVPROFCHAR[14] név tároló 
	//ERVPROFINT[12] Profil adat tároló
	
	Tido = 0;								//timer0 idõ méréséhez
	Hofoklepes = 0;							//Hõfokot léptetjük globálisan a timer0 miatt
	
	lcd_clear();
	
	//Fejméret kiválasztása
	while(Fej == 0){							//Ha fej még nincs kiválasztva
		keyval = get_key(KeyNumeric);			// Gombsor figyelése
		if(keyval!=0xFF){
			if(keyval=='1'){
				//Ventillátor indítása (1-5v) (PWM 1)
				ventillator1(3);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='2'){
				ventillator1(4);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='3'){
				ventillator1(5);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='4'){
				ventillator1(6);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='5'){
				ventillator1(7);
				Fej=1;	//Ezzel a ciklusból kilép
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
	Delay_ms(1000*4);			// Várakozunk venntillátor elidulásig (~4mp)

	INTCONbits.GIE = 1;			// Megszakítások engedélyezése

	//timer indítása
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

	Timer0Count = 0;		// Nullázuk timer0 számlálót
	TMR0H = 0x67;
	TMR0L = 0x69;
	INTCONbits.T0IF = 0;    // Timer0 túlcsordulásjelzõjét töröljük
	INTCONbits.T0IE = 1;    // Timer0 megszakításainak engedélyezése
	T0CONbits.TMR0ON = 1;	// Timer0 indítása
	
	lcd_clear();
	
	//Program elindul
	//Addig fut ameddig Hofoklepes kisebb mint maximum 6 lépés (0-5)
	for(i=0; i<6; i++){
		
		//Hang visszajelzés profillépésenként
		Buzzer(2, 10);
		
		x = i==0 ? 0 : i*2;
		visszaszamlalo = ERVPROFINT[x+1];		// Vissza számlálóba betöltjük az idõt
		while(ERVPROFINT[x+1]>Tido){			// Addig fut ameddig Tido le nem jár
			keyval = get_key(KeyNumeric);		// Gombsor figyelése
			if(keyval != 0xFF){
				if(keyval == 's'){				// Program leállítása
					T0CONbits.TMR0ON = 0; 		// Timer0 tíltása
					FelsoFutesRele = 0;			// Felsõfûtés letíltása
					Navigacio=6;
					return;
				}
			}

			Max6675Buff = Max6675();
			//Átlagoló indítása
			/****************************************
			+		Hõfok adatok átlagolásához		+
			****************************************/
			if(AtlagBTCount<AtlagTMeret){								// Ha Átlag számláló kissebb mint 4
				HoBufferTomb[AtlagBTCount] = Max6675Buff;					// Eltároljuk a hõmintát
				if(homerseklet>1023 && homerseklet<=0){					// Ha még nincs kijelezhetõ hõminta
					homerseklet=Max6675Buff;
				}
			}else if(AtlagBTCount==AtlagTMeret){						// Ha átlag számláló nagyobb mint 4
				HoBufferTomb[AtlagTMeret]='\0';							// Lezárjuk a tömböt
				homerseklet = hofokatlagolo(HoBufferTomb, AtlagTMeret);	// Átlagoljuk a beérkezett hõadatokat
			}

			if(AtlagBTCount > AtlagTMeret){
				AtlagBTCount = 0;
			}else{
				AtlagBTCount++;
			}
			
			printf("%s", ERVPROFCHAR);	//Kiírjuk a nevet
			lcd_write(0xC0,0,1,1); 
			printf("%i. [%03i%c]  [%03is]   ", i+1, ERVPROFINT[x], 0xdf, visszaszamlalo);
			lcd_write(0x94,0,1,1);
			printf("%i. [%03i%c]  [%03is]   ", i+1, homerseklet, 0xdf, visszaszamlalo-Tido);
			lcd_write(0xD4,0,1,1);
			//printf("\"%i\"", WhileCount); 
			RomStringWrite(szovegek[54]);
			lcd_write(0x80,0,1,1);

			//Utolsó profillépés, utolsó 5. másodpercében figyelmeztetõ hangot adunk
			if(i==5 && (visszaszamlalo-Tido)<6 && figyelmeztet){
				Buzzer(5, 50);
				figyelmeztet=0;
			}
		
		Delay_ms(WhileTime);
		WhileCount++;
		}
		Hofoklepes = (i+1)*2;
		Tido=0;					//Nullázzuk Tido-t az újra számláláshoz
	}
	T0CONbits.TMR0ON = 0; 		// Timer0 tíltása
	FelsoFutesRele = 0;			// Felsõfûtés letíltása
	Navigacio=6;
	return;
}

/****************************************************************************
+							"stop()" függvény
+				Forrasztási program kényszerített leállítása
*****************************************************************************/
void stop(void){
	unsigned char keyval;					//Gombsor figyelése
	
	//Hûtés vezérlése
	ventillator1(8);						//Maximum ventillátor fórdulat
	Buzzer(1, 100);							//Hosszú sípolással jelezzük a hûtést
	while(Max6675()>80){
		Utanhutes_relefigyeles();			//Hûtés
		keyval = get_key(KeyNumeric);		//Gombsor figyelése
		if(keyval != 0xFF){
			if(keyval == 's'){				//Vissza tér a menübe
				ventillator1(0);			//Ventillátor leállítása
				Navigacio=1;
				break;
			}
		}
		printf("%s                  ", ERVPROFCHAR);	//Kiírjuk a nevet
		lcd_write(0xC0,0,1,1); 
		printf("Hutes: %03i%c       ", Max6675(), 0xdf);
		lcd_write(0x94,0,1,1);
		RomStringWrite(szovegek[20]);
		lcd_write(0xD4,0,1,1);
		RomStringWrite(szovegek[30]);
		lcd_write(0x80,0,1,1);
	Delay_ms(WhileTime);
	}
	ventillator1(1);	//Ventillátor legalsó állapotba állítva kiléphetünk a programból
	Navigacio=1;
}

// Töltés jelzõ
void betoltes(void){
	unsigned char i;
	for(i=0; i<20; i++){
		_user_putc(0xFF);
		Delay_ms(20);
	}
} 

//Hang visszajezõ Buzzer
 void Buzzer(unsigned char jelszam, unsigned short int ms){
	 unsigned char i;
	 for(i=0; i<jelszam; i++){
		 BUZZER = 1;
		 Delay_ms(ms);
		 BUZZER = 0;
		 Delay_ms(ms);
	}
}

//Shift Register léptetése
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

//Utánhûtés és relé figyelése
void Utanhutes_relefigyeles(void){
	//Biztosítékként ellenõrizzük, hogy a fûtõtest ki e van kapcsolva
	if(FelsoFutesRele==1){ 	// ha be van kapcsolva
		FelsoFutesRele=0;	// Fûtõtest tíltása
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
+						"SMD_forraszto()" függvény
+					SMD levegõs forrasztó program
*****************************************************************************/
void SMD_forraszto(void){
    unsigned char keyval;
	lcd_clear();
	LowInterruptIni();			// Alacsony piorítású interrupt engedélyezése
	INTCONbits.GIE = 1;			// Megszakítások engedélyezése

	//timer indítása
	/*A timernek mindig 65535-46875=18660-ról kell indulnia, (~1mp) hogy jókor csorduljon túl és kérje a megszakítást.
	Ezt az értéket minden megszakításkor(rögtön az elején) bele kell írni a TMR0L és TMR0H regiszterekbe.
	Mivel ezek a regiszterek egyenként 8 bitesek, szét kell bontani.
	18660 binárisan: 01001000 11100100, vagyis
	18660 hexában: 	 0x48	  0xE4
	Félmásodperces mintavételezés 18660*2: 10010001 11001000
	Hexában: 0x91	0xC8
	Harmadmásodperces mintavételezés 18660*3: 11011010 10101100
	Hexában: 0xDA 0xAC*/

	Timer0Count = 0;		// Nullázuk timer0 számlálót
	TMR0H = 0x67;
	TMR0L = 0x69;
	INTCONbits.T0IF = 0;    // Timer0 túlcsordulásjelzõjét töröljük
	INTCONbits.T0IE = 1;    // Timer0 megszakításainak engedélyezése
	T0CONbits.TMR0ON = 1;	// Timer0 indítása

	while(1){
		keyval = get_key(KeyNumeric);	//numerikus gombok engedélyezése
		if(keyval != 0xFF){
			if(keyval=='x'){
				//Új profil hozzáadása
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
		//Biztosíték a fûtõtest kikapcsolására és a hûtésre
		Utanhutes_relefigyeles();
	Delay_ms(WhileTime);
	}
}
