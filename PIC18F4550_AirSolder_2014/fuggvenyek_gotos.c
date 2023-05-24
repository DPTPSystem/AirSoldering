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
	/** Várakozás arra, hogy az EEPROM befejezze az írást. Amíg az írás tart,
	 *  addig az EEPROM STATUS regiszterének legalsó bitje (WIP) '1'-ben áll.
	 *  A függvény blokkoló típusú, addig nem tér vissza, amíg az EEPROM foglalt.
	 */
void waitFor25LC640(void) {
	  unsigned int flag;
	  do {
		SPI_CS = 0;                      	    //kiadjuk a Chip Enable jelet
		spi_io(CMD_RDSR);                       //Státuszregiszter olvasása parancs
		flag = spi_io(0x00);                    //Olvasunk, de valamit akkor is küldeni kell!
		SPI_CS = 1;                   		    //megszüntetjük a Chip Enable jelet
	  } while (flag & 0x01); 
}
	/** Egy memórialap (? bájt) írása a bemenõ adatbufferbõl, az EEPROM egy
	 *  megadott címétõl kezdõdõen.
	 *  \param MemAddr a memórialap kezdõcíme, ahová írunk
	 *  \param buf mutató az adatbuffer kezdetéhez
	 */
	//---------------- char típusú -----------------------------// 
void memWrite25LC640(union16 MemCim, unsigned char *pbuf) {
		unsigned char i;
		waitFor25LC640();                         //Várunk, ha az EEPROM elfoglalt
		SPI_CS = 0;
		WriteSPI(CMD_WREN);                       //Írás újraengedélyezése
		SPI_CS = 1;
		Delay10KTCYx(1);
		SPI_CS = 0;
		spi_io(CMD_WRITE);                        //Adatblokk írása
		spi_io(MemCim.low);
		spi_io(MemCim.high);
		for (i=0; i < PCHARSIZE; i++) {
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
	  for (i=0; i < PCHARSIZE; i++) {
		*pbuf+=spi_io(0x00);                   //Olvasunk, de valamit akkor is küldeni kell!
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
					printf("~[%c]              ", alfanum[key][x]);
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
				//Delay10KTCYx(2);	//proteus
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
					printf("~[%c]              ", alfanum[key][x]);
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
				//Delay10KTCYx(2);	//proteus
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
					printf("~[%c]              ", alfanum[key][x]);
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
				//Delay10KTCYx(2);	//proteus
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
