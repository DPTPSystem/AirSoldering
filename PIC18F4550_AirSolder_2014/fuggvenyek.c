/*********************************************
+		Statikus változók
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
+		Késleltetés
**********************************************/
void Delay_ms(unsigned int ms){ //Bemenet*ms várakozás
   unsigned int i=0;
   for (i=0;i<=ms;i++) Delay10KTCYx(2);
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
		case 1:			//fórdulat 492rpm (1v)
		SetDCPWM1(205);
		break;
		case 2:			//fórdulat 984rpm (2v)
		SetDCPWM1(410);
		break;
		case 3:			//fórdulat 1474rpm (3v)
		SetDCPWM1(614);
		break;
		case 4:			//fórdulat 1963rpm (4v)
		SetDCPWM1(818);
		break;
		case 5: 		//Max fórdulat 2450rpm (5v)
		SetDCPWM1(1023);
		break;
		default:		//Ha az érték hibás akkor a ventillátor nem indul
		SetDCPWM1(0);
	}
	
}

/*********************************************
+		PWM Veltillátor 2
**********************************************/
void ventillator2(unsigned int speed){
	OpenPWM2(0xff);
	SetDCPWM2(speed);
}

/*********************************************
+		LCD Törlés
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
		Delay10KTCYx(2);
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
	for(i=0; i<4; i++){
		SOROK = (0x08>>i); 	//Léptetjük a sorok szintjét
		if(k!=0) k++;
		if(oszlop1){
			if(alfavagynum){	//ha 1 akkor alfanumerikus gombosort várunk
				return get_alfa_value(k);
			}else{	//ha nulla akkor numerikus gomsort várunk
				return get_num_value(k);
			}
		}
		k++;
		Delay_ms(2);		//késleltetünk kicsit
		if(oszlop2){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		k++;
		Delay_ms(2);		//késleltetünk kicsit
		if(oszlop3){
			if(alfavagynum){
				return get_alfa_value(k);
			}else{
				return get_num_value(k);
			}
		}
		k++;
		Delay_ms(2);		//késleltetünk kicsit
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

/*Ellenõrizük gombon belül melyik karaktert választjuk (alfanumerikus)*/
unsigned char get_num_value(unsigned char key){
	unsigned char i = 50, x = 0, result;
	if(oszlop1){
		while(oszlop1); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}
	if(oszlop2){	
		while(oszlop2); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}
	if(oszlop3){
		while(oszlop3); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}
	if(oszlop4){
		while(oszlop4); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}
	return result;
}

/*Ellenõrizük gombon belül melyik számot választjuk (numerikus)*/
unsigned char get_alfa_value(unsigned char key){
	unsigned char i = 50, x = 0, result;
	
	if(oszlop1){
		if(key!=12 & key!=0){	//Ha 1-est vagy ok-ot nyomtunk átugrik az else ágra és azonna vissza tér
			do{
				if(oszlop2 | oszlop3 | oszlop4) break;
				if(oszlop1){
					while(oszlop1); //Várakozunk a gomb felengedéséig
					
					printf("                    ");
					lcd_write(0xC0,0,1,1);	
					printf("~[%c]               ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("                    ");
					lcd_write(0xD4,0,1,1);
					printf("                    ");
					lcd_write(0x80,0,1,1);
					
					result = alfanum[key][x];
					
					if(x==3){	//Háromig emeljük x értékét majd nullázuk (Körbeforgatjuk a tomb tartamát)
						x=0;
					}else{
						x++;
					}
				}
				Delay10KTCYx(50);
			}while(i--);	//Várakozik hátha érkezik ebben a ciklusidõben további kérés
		}else{
			while(oszlop1); //Várakozunk a gomb felengedéséig
			result = alfanum[key][0];
		}
	}
	if(oszlop2){	
			do{
				if(oszlop1 | oszlop3 | oszlop4) break;
				if(oszlop2){
					while(oszlop2); //Várakozunk a gomb felengedéséig
						
					printf("                    ");
					lcd_write(0xC0,0,1,1);	
					printf("~[%c]               ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("                    ");
					lcd_write(0xD4,0,1,1);
					printf("                    ");
					lcd_write(0x80,0,1,1);
						
					result = alfanum[key][x];
					
					if(x==3){	//Háromig emeljük x értékét majd nullázuk (Körbeforgatjuk a tomb tartamát)
						x=0;
					}else{
						x++;
					}
				}
				Delay10KTCYx(50);
			}while(i--);	//Várakozik hátha érkezik ebben a ciklusidõben további kérés
	}
	if(oszlop3){
		if(key!=2){	//Ha DELL-t nyomtunk azonnal az else ágra ugrik és vissza tér
			do{
				if(oszlop1 | oszlop2 | oszlop4) break;
				if(oszlop3){
					while(oszlop3); //Várakozunk a gomb felengedéséig
							
					printf("                    ");
					lcd_write(0xC0,0,1,1);	
					printf("~[%c]               ", alfanum[key][x]);
					lcd_write(0x94,0,1,1);
					printf("                    ");
					lcd_write(0xD4,0,1,1);
					printf("                    ");
					lcd_write(0x80,0,1,1); 
						
					result = alfanum[key][x];
					
					if(x==3){	//Háromig emeljük x értékét majd nullázuk (Körbeforgatjuk a tomb tartamát)
						x=0;
					}else{
						x++;
					}
				}
				Delay10KTCYx(50);
			}while(i--);	//Várakozik hátha érkezik ebben a ciklusidõben további kérés
		}else{
			while(oszlop3); //Várakozunk a gomb felengedéséig
			result = alfanum[key][0];
		}
	}
	if(oszlop4){
		while(oszlop4); //Várakozunk a gomb felengedéséig
		result = alfanum[key][0];
	}
	return result;
}
			
/*********************************************
+		Profil adatok bekérõ üzenetei
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
		if(keyval=='a'){
			//Start
			Navigacio=5;
			break;
		}else if(keyval=='c'){
			//goto menu; 	//Ugrás a menüre
			Navigacio=0;
			break;
		}
		//ERVPROFCHAR név tároló adat tömb
		printf("%s", ERVPROFCHAR);
		lcd_write(0xC0,0,1,1);
	   	
		//ERVPROFINT hõprofil tároló kiírása
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
+						"menu()" függvény
+					A menü kezelését teszi lehetõvé
*****************************************************************************/
void menu(void){
	unsigned char keyval;
	lcd_clear();
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
+						"uj_profil()" függvény
+						Új profil létrehozásához
*****************************************************************************/
void uj_profil(void){
	unsigned char buffer[PMEMBLOCK+1] = {0};		//Buffer memória
	unsigned char text[PCHARSIZE+1] = {0};			//Ideiglenes karaktereket tárolunk benne (4x4 matrix)
	unsigned char i=0, count=0, cursor=0xFF, kitoltes, ptleptet=0;
	unsigned char adatcount=PCHARSIZE;				//Adatok mentésének kezdete a PCHARSIZE. bájttól
	unsigned char keyval;							//gomb értékét tárolja
	union16 bytes, MemCim;							//16bites adat kettéválasztásához
	bytes.value = 0;
	MemCim.value = 0;
	lcd_clear();
	while(1){
		
		keyval = ptleptet > 0 ? get_key(KeyNumeric) : get_key(KeyAlfanum);	//Alfanumerikus gombok engedélyezése
		
		if(keyval != '#'){		//Ha a feltétel igaz akkor történik gombnyomás
			if(keyval == 'e'){	//DELL gomb megnyomásánál következik be
				if(i!=0 && strlen(text)>0){	//Ha van mit törölni
					i--;
					text[i] = 0;	//"\0"-val jelezzük a tömb végét (gyakorlatilag törlünk a tömb végérõl visszafelé egyet)
				}
			}else if(keyval == 'd'){	//OK gomb megnyomásánál következik be
				//Ha a feltétel igaz, mentünk... (itt majd a memóriába fogunk írni)
				if(ptleptet > 0){	//Ha igaz akkor a nevet elmentettük
					if(szamvagykarakter(text)){	//Ellenõrizük, hogy biztos csak számot küldtek
						if(atoi(text)<256 && atoi(text)>0 && ptleptet<13){	//Ha "ptleptet" lépés számláló ejut 13-ig akkor az az alsó fûtés értékét várja ami nagyobb mint 255 és nagyobb mint nulla
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
							buffer[adatcount] = '\0';	//Lezárjuk a buffert
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
						printf("Profilnev tarolva!  ");
						ptleptet++;
					}else{
						printf("Hiba:Min 3 karakter!");
					}
				}
				memset(text, 0, sizeof(text));	//Törõljük a tömb tartalmát
				lcd_write(0xC0,0,1,1); 
				
				Delay_ms(600);	//kb 1mp
				
				i=0;	//Nullázuk a tömbnövelõ i változót is
			}else if(keyval == 'c'){	//Kis c (c == Menü) nyomásánál következik be
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
		if(ptleptet==14){
			lcd_clear();
			
			//Mentés memóriába
			MemCim.value = profilok_szama(0) == 0 ? 0 : PMEMBLOCK*profilok_szama(0);
			memWrite25LC640(MemCim, buffer);         //Memória lap kiírása
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
			
			//Betöltjük az elmentett profilt
			//Név letárolása
			kitoltes=1;	//Ha nincs rá szükség
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
			//Hõprofil és idõk letárolása
			for(i=0; i<PINTSIZE-1; i++){
				ERVPROFINT[i] = buffer[PCHARSIZE+i];
			}
			//Alsó fûtés 2bájtjának összefûzése és letárolása
			ERVPROFINT[PINTSIZE] = bytes.value;	//Az összevont 16bites adatot beletõltjuk a változónkba
			
			//Nullázuk a változókat az újra kezdés miatt
			ptleptet=0;
			bytes.value = 0;
			MemCim.value = 0;
			memset(text, 0, sizeof(text));			//Törõljük a tömb tartalmát
			memset(buffer, 0, sizeof(buffer));		//Törõljük a tömb tartalmát
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
	union16 ReadCim, AlsoFutes;
	unsigned char Membuffer[PMEMBLOCK+1] = {0}; //Profil blokk buffer
	unsigned char i, kitoltes;					//i nõvekszik, kitoltos : profilnevet üres hellyel tölti ki ha az kevesebb mint 18karakter
	ReadCim.value = 0;
	AlsoFutes.value = 0;
	//Az Eepromban elmentett címzést beállítjuk
	ReadCim.value = ((PMEMBLOCK*profilszam)-PMEMBLOCK) == 0 ? 0 : (PMEMBLOCK*profilszam)-PMEMBLOCK;
	memRead25LC640(ReadCim, Membuffer);
	
	if(Membuffer[0]=='#'){
		//Név letárolása
		kitoltes=1;	//Ha nincs rá szükség
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
		//Hõprofil és idõk letárolása
		for(i=0; i<PINTSIZE-1; i++){
			ERVPROFINT[i] = Membuffer[PCHARSIZE+i];
		}
		//Alsó fûtés 2bájtjának összefûzése és letárolása
		AlsoFutes.value = 0;	//Nullázzuk az adatstruktúrát
		i = PCHARSIZE+i;
		AlsoFutes.low = Membuffer[i++];
		AlsoFutes.high = Membuffer[i++];
		ERVPROFINT[PINTSIZE] = AlsoFutes.value;	//Az összevont 16bites adatot beletõltjuk a változónkba
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
		
		if(keyval != '#'){
			
			if(keyval == '6'){ //-> Lapozunk jobbra.
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
			}else if(keyval == 'c'){	//Kis c (c == Menü) nyomásánál következik be
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
		printf("[Menu]         [6->]"); 
		lcd_write(0x80,0,1,1);
		Delay_ms(5);
		
	}
	
}

/****************************************************************************
+						"profil_szerk()" függvény
+					Betölti a szerkesztendõ profil adatait
*****************************************************************************/
void profil_szerk(void){
	unsigned char buffer[PMEMBLOCK+1] = {0};		//Buffer memória
	unsigned char text[PCHARSIZE+1] = {0};			//Ideiglenes karaktereket tárolunk benne (4x4 matrix)
	unsigned char i=0, count=0, cursor=0xFF, kitoltes, ptleptet=0;
	unsigned char adatcount=PCHARSIZE;				//Adatok mentésének kezdete a PCHARSIZE. bájttól
	unsigned char keyval;							//gomb értékét tárolja
	union16 bytes, MemCim;							//16bites adat kettéválasztásához
	unsigned char fljelzo=0, stop=1;				//Feltõltést jelzõ változó
	
	bytes.value = 0;
	MemCim.value = 0;
	lcd_clear();
	while(1){
		
		//Szerkesztésnél felelõs az adatok feltöltéséért
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
			
		keyval = ptleptet > 0 ? get_key(KeyNumeric) : get_key(KeyAlfanum);	//Alfanumerikus gombok engedélyezése
		
		if(keyval != '#'){		//Ha a feltétel igaz akkor történik gombnyomás
			if(keyval == 'e'){	//DELL gomb megnyomásánál következik be
				i= i==0 ? i=strlen(text) : i;
				if(i!=0 && strlen(text)>0){	//Ha van mit törölni
					i--;
					text[i] = 0;	//"\0"-val jelezzük a tömb végét (gyakorlatilag törlünk a tömb végérõl visszafelé egyet)
				}
			}else if(keyval == 'd'){	//OK gomb megnyomásánál következik be
				//Ha a feltétel igaz, mentünk... (itt majd a memóriába fogunk írni)
				if(ptleptet > 0){	//Ha igaz akkor a nevet elmentettük
					if(szamvagykarakter(text)){	//Ellenõrizük, hogy biztos csak számot küldtek
						if(atoi(text)<256 && atoi(text)>0 && ptleptet<13){	//Ha "ptleptet" lépés számláló ejut 13-ig akkor az az alsó fûtés értékét várja ami nagyobb mint 255 és nagyobb mint nulla
							buffer[adatcount] = (char)atoi(text);
							printf("Adat eltarolva!     ");
							adatcount++;
							ptleptet++;
							fljelzo++; stop=1;	//Szerkesztésnél emeljük feltöltéstjelzõ-t és engedélyezzük a lefutást
						}else if(ptleptet==13 && atoi(text)>0){
							bytes.value = atoi(text);
							buffer[adatcount] = bytes.low;
							adatcount++;
							buffer[adatcount] = bytes.high;
							adatcount++;
							buffer[adatcount] = '\0';	//Lezárjuk a buffert
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
						printf("Profilnev tarolva!  ");
						ptleptet++;
						fljelzo++; stop=1;	//Szerkesztésnél emeljük feltöltéstjelzõ-t és engedélyezzük a lefutást
					}else{
						printf("Hiba:Min 3 karakter!");
					}
				}
				memset(text, 0, sizeof(text));	//Törõljük a tömb tartalmát
				lcd_write(0xC0,0,1,1); 
				
				Delay_ms(600);	//kb 1mp
				
				i=0;	//Nullázuk a tömbnövelõ i változót is
			}else if(keyval == 'c'){	//Kis c (c == Menü) nyomásánál következik be
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
		if(ptleptet==14){
			lcd_clear();
			
			//Mentés memóriába
			MemCim.value = (PMEMBLOCK*EepromRead())-PMEMBLOCK;
			memWrite25LC640(MemCim, buffer);         //Memória lap kiírása
			
			profiltext(ptleptet);
			lcd_write(0xC0,0,1,1);  
			printf("Adatok elmentve!    ");
			lcd_write(0x94,0,1,1);
			printf("Profil betoltese    ");
			lcd_write(0xD4,0,1,1);
			printf("folyamatban....     ");
			lcd_write(0x80,0,1,1);
			Delay_ms(600*3);	//kb ~3mp
			
			//Betöltjük az elmentett profilt
			//Név letárolása
			kitoltes=1;	//Ha nincs rá szükség
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
			//Hõprofil és idõk letárolása
			for(i=0; i<PINTSIZE-1; i++){
				ERVPROFINT[i] = buffer[PCHARSIZE+i];
			}
			//Alsó fûtés 2bájtjának összefûzése és letárolása
			ERVPROFINT[PINTSIZE] = bytes.value;	//Az összevont 16bites adatot beletõltjuk a változónkba
			
			//Nullázuk a változókat az újra kezdés miatt
			ptleptet=0;
			bytes.value = 0;
			MemCim.value = 0;
			memset(text, 0, sizeof(text));			//Törõljük a tömb tartalmát
			memset(buffer, 0, sizeof(buffer));		//Törõljük a tömb tartalmát
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
+							"Max6675()" függvény
+					Max6675 olvasó függvény, paraméterei 1-3-ig
*****************************************************************************/
short int Max6675(unsigned char MaxICcs){
	short int temp = 0;		// 8bites változó
	short int result = 0;		// 16bites változó
	float fokra_valt = 0;		// Lebegõpontos változó
	//-- Engedélyezzük a kivállasztott chip-et
	if(MaxICcs==1){
		MAX1CS = 0;       		// 1. MAX6675-öt engedélyezzük
	}else if(MaxICcs==2){
		MAX2CS = 0;       		// 2. MAX6675-öt engedélyezzük
	}else if(MaxICcs==3){
		MAX3CS = 0;       		// 3. MAX6675-öt engedélyezzük
	}
	Delay10KTCYx(2); 			// Idõzítés
	// --------- Beolvassuk  az 1-es MAX6675 adatait ---------------//
	temp = spi_io(0x00);		// Olvasunk, de valamit akkor is küldeni kell
	result = spi_io(0x00);		// Olvasunk, de valamit akkor is küldeni kell
	temp = temp << 8;    		// Léptetjuk ballra 8 bitet és letároljuk az elsõ 8bitet
	result = result + temp; 	// Vissza alakítjuk 16bites-re az adatot
	result = result >> 3;  		// Léptetünk jobbra 3bitet így megkapjuk a 12bitet
	//-- Engedélyezzük a kivállasztott chip-et
	if(MaxICcs==1){
		MAX1CS = 1;       		// 1. MAX6675-öt tiltjuk
	}else if(MaxICcs==2){
		MAX2CS = 1;       		// 2. MAX6675-öt tiltjuk
	}else if(MaxICcs==3){
		MAX3CS = 1;       		// 3. MAX6675-öt tiltjuk
	}
	fokra_valt = result*0.25;	// Átváltás fokra
	return (int)fokra_valt;		// Visszatérünk a fokkal
}

/****************************************************************************
+							"hofokatlagolo()" függvény
+					Elinddúl a program és a forrasztás
*****************************************************************************/
//Hõfok átlagoló
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
+							"start()" függvény
+					Elinddúl a program és a forrasztás
*****************************************************************************/
void start(void){
	unsigned char i, x, visszaszamlalo;		//Belsõ léptetéshez
	unsigned char keyval;					//Gombsor figyelése
	short int homerseklet = 0;				//Hõmérsékletet fogja tárolni
	unsigned char Fej=0;					//Fej kiválasztása
	
	//Globális változók
	//ERVPROFCHAR[18] név tároló 
	//ERVPROFINT[13] Profil adat tároló
	Tido = 0;								//timer0 idõ méréséhez
	Hofoklepes = 0;							//Hõfokot léptetjük globálisan a timer0 miatt
	
	lcd_clear();
	
	//Fejméret kiválasztása
	while(Fej == 0){							//Ha fej még nincs kiválasztva
		keyval = get_key(KeyNumeric);			// Gombsor figyelése
		if(keyval!='#'){
			if(keyval=='1'){
				//Ventillátor indítása (1-5v) (PWM 1)
				ventillator1(1);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='2'){
				ventillator1(2);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='3'){
				ventillator1(3);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='4'){
				ventillator1(4);
				Fej=1;	//Ezzel a ciklusból kilép
			}else if(keyval=='5'){
				ventillator1(5);
				Fej=1;	//Ezzel a ciklusból kilép
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
	
	INTCONbits.GIE = 1;						// Megszakítások engedélyezése

	//timer indítása
	/*A timernek mindig 65535-46875=18660-ról kell indulnia, (~1mp) hogy jókor csorduljon túl és kérje a megszakítást.
	Ezt az értéket minden megszakításkor(rögtön az elején) bele kell írni a TMR0L és TMR0H regiszterekbe.
	Mivel ezek a regiszterek egyenként 8 bitesek, szét kell bontani.
	18660 binárisan: 01001000 11100100, vagyis
	18660 hexában: 	 0x48	  0xE4
	Félásodperces mintavételezés 18660*2: 10010001 11001000
	Hexában: 0x91	0xC8*/
	TMR0H = 0x91;
	TMR0L = 0xC8;
	INTCONbits.T0IF = 0;    // Timer0 túlcsordulásjelzõjét töröljük
	INTCONbits.T0IE = 1;    // Timer0 megszakításainak engedélyezése
	T0CONbits.TMR0ON = 1;	// Timer0 indítása
	
	lcd_clear();
	
	//Program elindul
	//Addig fut ameddig Hofoklepes kisebb mint maximum 6 lépés (0-5)
	for(i=0; i<6; i++){
		x = i==0 ? 0 : i*2;
		visszaszamlalo = ERVPROFINT[x+1];		// Vissza számlálóba betöltjük az idõt
		while(ERVPROFINT[x+1]>Tido/2){			// Addig fut ameddig Tido le nem jár
			keyval = get_key(KeyNumeric);		// Gombsor figyelése
			if(keyval != '#'){
				if(keyval == 'b'){				// Program leállítása
					T0CONbits.TMR0ON = 0; 		// Timer0 tíltása
					AlsoFutesRele = 0;			// Alsófûtés letíltása
					FelsoFutesRele = 0;			// Felsõfûtés letíltása
					Navigacio=6;
					return;
				}
			}
			//Átlagoló indítása
			if(AtlagBTCount<4){											// Ha Átlag számláló kissebb mint 4
				HoBufferTomb[AtlagBTCount] = Max6675(2);				// Eltároljuk a hõmintát
				if(homerseklet==0){ homerseklet=Max6675(2); }			// Ha még nincs kijelezhetõ hõminta
			}else if(AtlagBTCount==4){									// Ha átlag számláló nagyobb mint 4
				homerseklet = hofokatlagolo(HoBufferTomb, AtlagTMeret);	// Átlagoljuk a beérkezett hõadatokat
			}
			printf("%s", ERVPROFCHAR);	//Kiírjuk a nevet
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
		Tido=0;			//Nullázzuk Tido-t az újra számláláshoz
	}
	T0CONbits.TMR0ON = 0; 	// Timer0 tíltása
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
	ventillator1(5);						//Maximum ventillátor fórdulat
	AlsoFutesRele = 0;						//Alsófûtés tíltása
	FelsoFutesRele = 0;						//Felsõ fûtés tíltása

	while(Max6675(3)>80){
		keyval = get_key(KeyNumeric);		//Gombsor figyelése
		if(keyval != '#'){
			if(keyval == 'c'){				//Vissza tér a menübe
				ventillator1(0);			//Ventillátor leállítása
				Navigacio=1;
				break;
			}
		}
		printf("%s                  ", ERVPROFCHAR);	//Kiírjuk a nevet
		lcd_write(0xC0,0,1,1); 
		printf("Hutes: %03i%c       ", Max6675(3), 0xdf);
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("Tovabbi opcio: MENU ");
		lcd_write(0x80,0,1,1);
		Delay_ms(20);
	}
	ventillator1(0);	//Ventillátor leállítása
	Navigacio=1;
}
