/*********************************************
+		Union deklarálása
**********************************************/
/*- 16bit-es UNION - (SPI memória címzéshez és 2 bájtos (16bit-es) adatok tárolásához kell)*/
typedef union _union16 { 
  unsigned int value;
  struct {
    unsigned char high;
	unsigned char low;
  };
} union16;

/*********************************************
+		Inicializálás
**********************************************/

void init(void){

    /*********************************************
	+		Ki / Bemenetek
	**********************************************/
	TRISA = 0;		//Gombmeghajtas kimenetek (RA0-RA3)
	TRISD = 0; 		//D PORT 0:2 és 4:7 LCD kivezetések, 3-as láb LED	
	TRISB = 0xf0; 	//B port felsõ 4 bit (nagyobb helyi értéken 7:4) bemenet
					//alsó 4 bit (kisebb helyi értéken 3:0) kimenet
					//0b11110000 
	TRISE = 0;		//MAX6675 ChipSelect kimenetek
	TRISC = 0;		//PWM portok és egyébbek, teljes port kimenetre állítva
	ADCON1 = 0x0F;  //Minden port legyen digitális

	/*********************************************
	+		LCD inicializálás
	**********************************************/
    lcd_init();             // Az LCD modulinicializálása 
    //lcd_init_cgram();       // Az ékezetes betûk feltöltése
    stdout = _H_USER;       // Az LCD legyen a standard kimenetnt
    /****************************LCD*************************************/
	
	/*********************************************
	+		TIMER & INTERRUPT INIT
	+	Timer0 init 1 másodperces megszakításkéréshez (20 Mhz órajel mellett)
	**********************************************/
	RCONbits.IPEN = 1;			// Kétszintû megszakítási mód beállítása
    INTCONbits.GIEH = 1;		// A magas prioritású interrupt engedélyezése
    INTCONbits.GIEL = 0;		// Az alacsony prioritású interrupt tiltása
    INTCON2bits.TMR0IP = 1;		// Timer0 megszakítás magas prioritású legyen
	
	T0CONbits.TMR0ON = 0;		// Timer0 kikapcsolva
	T0CONbits.T08BIT = 0;		// 16 bites mód kiválasztás
	T0CONbits.T0CS = 0;			// Meghajtás belsõ órajelrõl
	T0CONbits.T0SE = 0;			// Külsõ órajel fel- vagy lefutó élének választása
	T0CONbits.PSA = 0;			// Elõosztás bekapcsolása
	//Elõosztási arány beállítása (000 = 1:2, 001 = 1:4, 010 = 1:8, 011 = 0:16,
								// 100 = 1:32, 101 = 1:64, 110 = 1:128, 111 = 1:256)
	T0CONbits.T0PS2 = 1;		// Elõosztás 1:256 osztásra állítása
	T0CONbits.T0PS1 = 1;
	T0CONbits.T0PS0 = 1;
	INTCONbits.GIE = 0;			// Megszakítások tíltása
	
	/*********************************************
	+		USB kikapcsolása
	**********************************************/
	/*UCONbits.SUSPND = 0;
	UCONbits.USBEN = 0;*/
	
	/*********************************************
	+		SPI init
	**********************************************/
	SPI_CS_TRIS = 0;		//A5-es láb kimenet
	SPI_CS = 1;				//A5 láb magas szinten
	MAX1CS = 1;				//E0 láb MAX6675 ChipSelect
	MAX2CS = 1;				//E1 láb MAX6675 ChipSelect
	MAX3CS = 1;				//E2 láb MAX6675 ChipSelect
	SPI_SO_TRIS = 1;		//B0 láb SDI bemenet
	SPI_SCK_TRIS = 0;		//B1 láb SCK kimenet
	SPI_SI = 0;				//C7 láb SI kimenet

	SSPSTATbits.SMP = 0;	//Mintavételezés idõzítése Master mód esetén 1: mintavételezés a kimeneti idõ végén, 0: mintavételezés a kimeneti idõ közepén). Slave módban kötelezõen nullába kell állítani.
	SSPSTATbits.CKE = 0;	//Órajel fázisának beállítása (1: az adatkimenet beállítása akkor történik,amikor az órajel aktív állapotból tétlenre vált, 0: az adatkimenet beállítása akkor történik, amikor az órajel tétlen állapotból aktív szintre vált)
	SSPSTATbits.BF = 0;		//A buffer foglaltságát jelzõ állapotbit  (1: az adatátvitel lezajlott, SSPBUF kiolvasható, 0: az adatbuffer üres)
	
	SSPCON1bits.WCOL = 	0;	//Ütközés detektálás írásnál (1: írást/adatküldést kíséreltünk meg, amikor nem lehetett, 0: nincs ütközés)
	SSPCON1bits.SSPOV = 0;	//Vételi túlcsordulás-jelzõ (1: újabb bájt érkezett, de SSPBUF meg nincs kiolvasva, 0: nincs túlfolyás)
	SSPCON1bits.SSPEN = 1;	//Master Szinkron Soros Port engedélyezése (1: engedélyezve, 0: letiltva)
	SSPCON1bits.CKP = 	0;	//Órajel polaritása (1: aktív állapotban alacsony szint, 0: aktív állapotban magas szint)
							//Master Szinkron Soros Port üzemmódjának beállítása:
							//0101 = SPI Slave mód, az SS láb használata nélkül
							//0100 = SPI Slave mód, amelynél az SS láb is használatban van
							//0011 = SPI Master mód, Timer2/2 órajel frekvenciával
							//0010 = SPI Master mód, Fosc/64 órajel frekvenciával
							//0001 = SPI Master mód, Fosc/16 órajel frekvenciával
							//0000 = SPI Master mód, Fosc/4 órajel frekvenciával
	SSPCON1bits.SSPM0 = 0;
	SSPCON1bits.SSPM1 = 1;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM3 = 0;
}


/*********************************************
+		Prototípusok
**********************************************/
void main(void);								//Main()
void init(void);								//Inicializálás
void Delay_ms(unsigned int ms); 				//Késleltetés
void ventillator1(unsigned int szint);			//PWM1 ventillátor1
void ventillator2(unsigned int speed);			//PWM2 ventillátor2
void lcd_clear(void);							//LCD törlése
unsigned char EepromRead(void);					//Eeprom olvasása
void EepromWrite(unsigned char adat);			//Eeprom rása
unsigned char spi_io(unsigned char adat);		//SPI írás olvasás
void waitFor25LC640(void);						//Várakozás a SPI memóriára
void memWrite25LC640(union16 MemCim, unsigned char *pbuf);	//Memória írása
void memRead25LC640(union16 MemCim, unsigned char *pbuf);	//Meria olvasása
unsigned char get_key(unsigned char alfavagynem);		//Gomb figyelés
unsigned char get_alfa_value(unsigned char key);		//Alfanumerikus gomb érték
unsigned char get_num_value(unsigned char key);			//Numerikus gomb érték
void profiltext(unsigned char x);						//Profil felvételének és szerkesztésének kiírásai
unsigned int szamvagykarakter(unsigned char tomb[]);	//Szám vagy karakter ellenõrzõ
void profilt_mutat(void);								//Profil betöltve és várakozik a gombnyomásra
void menu(void);										//Menû és várakozik a gombnyomásra
void uj_profil(void);									//Új profil felvétele
void profilt_beletolt(unsigned char profilszam);		//Betölti az Eepromban tárolt profilt
unsigned char profilok_szama(unsigned char pszam);		//0 == Profilok száma, 0-nál nagyobb == 0 nem érvényes, 1 létezik
void start(void);										//timer0 és interrupt vagy is a forrasztás elindítása
short int Max6675(unsigned char MaxICcs);				//Max6675 chip adatainak kiolvasása és fokra váltása
short int hofokatlagolo(short int foktomb[4], 
						unsigned char tsize);			//Hõfok átlagoló
