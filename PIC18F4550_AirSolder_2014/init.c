/*********************************************
+		Union deklar�l�sa
**********************************************/
/*- 16bit-es UNION - (SPI mem�ria c�mz�shez �s 2 b�jtos (16bit-es) adatok t�rol�s�hoz kell)*/
typedef union _union16 { 
  unsigned int value;
  struct {
    unsigned char high;
	unsigned char low;
  };
} union16;

/*********************************************
+		Inicializ�l�s
**********************************************/

void init(void){

    /*********************************************
	+		Ki / Bemenetek
	**********************************************/
	TRISA = 0;		//Gombmeghajtas kimenetek (RA0-RA3)
	TRISD = 0; 		//D PORT 0:2 �s 4:7 LCD kivezet�sek, 3-as l�b LED	
	TRISB = 0xf0; 	//B port fels� 4 bit (nagyobb helyi �rt�ken 7:4) bemenet
					//als� 4 bit (kisebb helyi �rt�ken 3:0) kimenet
					//0b11110000 
	TRISE = 0;		//MAX6675 ChipSelect kimenetek
	TRISC = 0;		//PWM portok �s egy�bbek, teljes port kimenetre �ll�tva
	ADCON1 = 0x0F;  //Minden port legyen digit�lis

	/*********************************************
	+		LCD inicializ�l�s
	**********************************************/
    lcd_init();             // Az LCD modulinicializ�l�sa 
    //lcd_init_cgram();       // Az �kezetes bet�k felt�lt�se
    stdout = _H_USER;       // Az LCD legyen a standard kimenetnt
    /****************************LCD*************************************/
	
	/*********************************************
	+		TIMER & INTERRUPT INIT
	+	Timer0 init 1 m�sodperces megszak�t�sk�r�shez (20 Mhz �rajel mellett)
	**********************************************/
	RCONbits.IPEN = 1;			// K�tszint� megszak�t�si m�d be�ll�t�sa
    INTCONbits.GIEH = 1;		// A magas priorit�s� interrupt enged�lyez�se
    INTCONbits.GIEL = 0;		// Az alacsony priorit�s� interrupt tilt�sa
    INTCON2bits.TMR0IP = 1;		// Timer0 megszak�t�s magas priorit�s� legyen
	
	T0CONbits.TMR0ON = 0;		// Timer0 kikapcsolva
	T0CONbits.T08BIT = 0;		// 16 bites m�d kiv�laszt�s
	T0CONbits.T0CS = 0;			// Meghajt�s bels� �rajelr�l
	T0CONbits.T0SE = 0;			// K�ls� �rajel fel- vagy lefut� �l�nek v�laszt�sa
	T0CONbits.PSA = 0;			// El�oszt�s bekapcsol�sa
	//El�oszt�si ar�ny be�ll�t�sa (000 = 1:2, 001 = 1:4, 010 = 1:8, 011 = 0:16,
								// 100 = 1:32, 101 = 1:64, 110 = 1:128, 111 = 1:256)
	T0CONbits.T0PS2 = 1;		// El�oszt�s 1:256 oszt�sra �ll�t�sa
	T0CONbits.T0PS1 = 1;
	T0CONbits.T0PS0 = 1;
	INTCONbits.GIE = 0;			// Megszak�t�sok t�lt�sa
	
	/*********************************************
	+		USB kikapcsol�sa
	**********************************************/
	/*UCONbits.SUSPND = 0;
	UCONbits.USBEN = 0;*/
	
	/*********************************************
	+		SPI init
	**********************************************/
	SPI_CS_TRIS = 0;		//A5-es l�b kimenet
	SPI_CS = 1;				//A5 l�b magas szinten
	MAX1CS = 1;				//E0 l�b MAX6675 ChipSelect
	MAX2CS = 1;				//E1 l�b MAX6675 ChipSelect
	MAX3CS = 1;				//E2 l�b MAX6675 ChipSelect
	SPI_SO_TRIS = 1;		//B0 l�b SDI bemenet
	SPI_SCK_TRIS = 0;		//B1 l�b SCK kimenet
	SPI_SI = 0;				//C7 l�b SI kimenet

	SSPSTATbits.SMP = 0;	//Mintav�telez�s id�z�t�se Master m�d eset�n 1: mintav�telez�s a kimeneti id� v�g�n, 0: mintav�telez�s a kimeneti id� k�zep�n). Slave m�dban k�telez�en null�ba kell �ll�tani.
	SSPSTATbits.CKE = 0;	//�rajel f�zis�nak be�ll�t�sa (1: az adatkimenet be�ll�t�sa akkor t�rt�nik,amikor az �rajel akt�v �llapotb�l t�tlenre v�lt, 0: az adatkimenet be�ll�t�sa akkor t�rt�nik, amikor az �rajel t�tlen �llapotb�l akt�v szintre v�lt)
	SSPSTATbits.BF = 0;		//A buffer foglalts�g�t jelz� �llapotbit  (1: az adat�tvitel lezajlott, SSPBUF kiolvashat�, 0: az adatbuffer �res)
	
	SSPCON1bits.WCOL = 	0;	//�tk�z�s detekt�l�s �r�sn�l (1: �r�st/adatk�ld�st k�s�relt�nk meg, amikor nem lehetett, 0: nincs �tk�z�s)
	SSPCON1bits.SSPOV = 0;	//V�teli t�lcsordul�s-jelz� (1: �jabb b�jt �rkezett, de SSPBUF meg nincs kiolvasva, 0: nincs t�lfoly�s)
	SSPCON1bits.SSPEN = 1;	//Master Szinkron Soros Port enged�lyez�se (1: enged�lyezve, 0: letiltva)
	SSPCON1bits.CKP = 	0;	//�rajel polarit�sa (1: akt�v �llapotban alacsony szint, 0: akt�v �llapotban magas szint)
							//Master Szinkron Soros Port �zemm�dj�nak be�ll�t�sa:
							//0101 = SPI Slave m�d, az SS l�b haszn�lata n�lk�l
							//0100 = SPI Slave m�d, amelyn�l az SS l�b is haszn�latban van
							//0011 = SPI Master m�d, Timer2/2 �rajel frekvenci�val
							//0010 = SPI Master m�d, Fosc/64 �rajel frekvenci�val
							//0001 = SPI Master m�d, Fosc/16 �rajel frekvenci�val
							//0000 = SPI Master m�d, Fosc/4 �rajel frekvenci�val
	SSPCON1bits.SSPM0 = 0;
	SSPCON1bits.SSPM1 = 1;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM3 = 0;
}


/*********************************************
+		Protot�pusok
**********************************************/
void main(void);								//Main()
void init(void);								//Inicializ�l�s
void Delay_ms(unsigned int ms); 				//K�sleltet�s
void ventillator1(unsigned int szint);			//PWM1 ventill�tor1
void ventillator2(unsigned int speed);			//PWM2 ventill�tor2
void lcd_clear(void);							//LCD t�rl�se
unsigned char EepromRead(void);					//Eeprom olvas�sa
void EepromWrite(unsigned char adat);			//Eeprom r�sa
unsigned char spi_io(unsigned char adat);		//SPI �r�s olvas�s
void waitFor25LC640(void);						//V�rakoz�s a SPI mem�ri�ra
void memWrite25LC640(union16 MemCim, unsigned char *pbuf);	//Mem�ria �r�sa
void memRead25LC640(union16 MemCim, unsigned char *pbuf);	//Meria olvas�sa
unsigned char get_key(unsigned char alfavagynem);		//Gomb figyel�s
unsigned char get_alfa_value(unsigned char key);		//Alfanumerikus gomb �rt�k
unsigned char get_num_value(unsigned char key);			//Numerikus gomb �rt�k
void profiltext(unsigned char x);						//Profil felv�tel�nek �s szerkeszt�s�nek ki�r�sai
unsigned int szamvagykarakter(unsigned char tomb[]);	//Sz�m vagy karakter ellen�rz�
void profilt_mutat(void);								//Profil bet�ltve �s v�rakozik a gombnyom�sra
void menu(void);										//Men� �s v�rakozik a gombnyom�sra
void uj_profil(void);									//�j profil felv�tele
void profilt_beletolt(unsigned char profilszam);		//Bet�lti az Eepromban t�rolt profilt
unsigned char profilok_szama(unsigned char pszam);		//0 == Profilok sz�ma, 0-n�l nagyobb == 0 nem �rv�nyes, 1 l�tezik
void start(void);										//timer0 �s interrupt vagy is a forraszt�s elind�t�sa
short int Max6675(unsigned char MaxICcs);				//Max6675 chip adatainak kiolvas�sa �s fokra v�lt�sa
short int hofokatlagolo(short int foktomb[4], 
						unsigned char tsize);			//H�fok �tlagol�
