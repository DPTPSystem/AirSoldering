void lo_isr(void);        //elõre deklaráljuk a kiszolgáló
void hi_isr(void);        //eljárásokat

/*PIC18F4550 csak a B PORT felsõ 4 bitjén tud megszakítást kezelni, külön nem választható (RB7...RB4)
*/
/** Magas és alacsony prioritású interrupt vektor
    Magas = 0x0008, Alacsony = 0x0018.
*/
#pragma code high_vector_addr=0x08
void high_vector(void) {
  _asm GOTO hi_isr _endasm
}
#pragma code low_vector_addr=0x18
void low_vector(void) {
  _asm GOTO lo_isr _endasm
}
#pragma code            					 //visszatérünk az alapértelmezett kódszekcióba
#pragma interrupt hi_isr
void hi_isr (void) {
/* Itt történik a magas prioritású megszakítás tényleges kiszolgálása */
	TMR0H = 0x91;
	TMR0L = 0xC8;
	/****************************************
	+	Alsó fûtés figyelése és vezérlése	+
	****************************************/
	if(ERVPROFINT[13] > Max6675(3)){		//Ha alsó fûtés még nem érte el a kellõ értéket
		AlsoFutesRele = !AlsoFutesRele;		//Alsófûtés engedélyezve
	}else{
		AlsoFutesRele = 0;					//Alsófûtés tíltása
	}
	
	/****************************************
	+	Felsõ fûtés figyelése és vezérlése	+
	****************************************/
	if(ERVPROFINT[Hofoklepes] > Max6675(2)){	//Ha felsõ fûtés még nem érte el a kellõ értéket
		FelsoFutesRele = !FelsoFutesRele;		//Felsõ fûtés engedélyezve
	}else{
		FelsoFutesRele = 0;						//Felsõ fûtés tíltása
	}	
	
	/****************************************
	+		Hõfok adatok átlagolásához		+
	****************************************/
	if(AtlagBTCount > 4){
		AtlagBTCount = 0;
	}else{
		AtlagBTCount++;
	}
	Tido++;										//Emeljük Tido-t
	INTCONbits.TMR0IF = 0;						// Ezt kell törölni minden megszakítás futás végén
}
#pragma interruptlow lo_isr
void lo_isr (void) {
  /* Itt történik az alacsony prioritású megszakítás tényleges kiszolgálása */
}
