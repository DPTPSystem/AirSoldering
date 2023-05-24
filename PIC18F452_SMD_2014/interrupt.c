void hi_isr(void);     ///< Magas prioritású programmegszakítás kiszolgálása
void lo_isr(void);     ///< Alacsony prioritású programmegszakítás kiszolgálása

/*
// Csak bootloader esetén
#define RESET_VECTOR			0x1000
#define HIGH_INTERRUPT_VECTOR	0x1008
#define LOW_INTERRUPT_VECTOR	0x1018

extern void _startup(void);
#pragma code REMAPPED_RESET_VECTOR = RESET_VECTOR
void _reset (void){
	_asm goto _startup _endasm
}

#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = HIGH_INTERRUPT_VECTOR
void Remapped_High_ISR (void){
	 _asm goto hi_isr _endasm
}

#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = LOW_INTERRUPT_VECTOR
void Remapped_Low_ISR (void){
	 _asm goto lo_isr _endasm 
}
*/

// Ha nem használok bootloadert
#pragma code high_vector_addr=0x08
void high_vector(void) {
  _asm GOTO hi_isr _endasm
}
#pragma code low_vector_addr=0x18
void low_vector(void) {
  _asm GOTO lo_isr _endasm
}

#pragma code             //visszatérünk az alapértelmezett kódszekcióba
#pragma interrupt hi_isr
void hi_isr (void) {
/* Itt történik a magas prioritású megszakítás tényleges kiszolgálása */	
	TMR0H = 0xCD;
	TMR0L = 0x23;
	
	/****************************************
	+	Felsõ fûtés figyelése és vezérlése	+
	****************************************/
	if(ERVPROFINT[Hofoklepes] > Max6675()){		// Ha felsõ fûtés még nem érte el a kellõ értéket
		FelsoFutesRele = 1;							// Felsõ fûtés engedélyezése
		if(ERVPROFINT[Hofoklepes]<=100){
			Delay_ms(20);
		}else if(ERVPROFINT[Hofoklepes]>100 && ERVPROFINT[Hofoklepes]<=200){
			Delay_ms(40);
		}else if(ERVPROFINT[Hofoklepes]>200 && ERVPROFINT[Hofoklepes]<=300){
			Delay_ms(60);
		}else if(ERVPROFINT[Hofoklepes]>300 && ERVPROFINT[Hofoklepes]<=400){
			Delay_ms(70);
		}else if(ERVPROFINT[Hofoklepes]>400){
			Delay_ms(80);
		}
		FelsoFutesRele = 0;
	}else{	// Felsõ fûtés pulzálása
		FelsoFutesRele = 1;
		if(ERVPROFINT[Hofoklepes]>100 && ERVPROFINT[Hofoklepes]<=200){
			Delay_ms(15);
		}else if(ERVPROFINT[Hofoklepes]>200 && ERVPROFINT[Hofoklepes]<=300){
			Delay_ms(20);
		}else if(ERVPROFINT[Hofoklepes]>300 && ERVPROFINT[Hofoklepes]<=400){
			Delay_ms(25);
		}else if(ERVPROFINT[Hofoklepes]>400){
			Delay_ms(30);
		}else{
			Delay_ms(5);
		}
		FelsoFutesRele = 0;
	}

	if(Timer0Count>2){
		Tido++;					// Emeljük Tido-t
		Timer0Count = 0;
	}
	Timer0Count++;
	INTCONbits.TMR0IF = 0;		// Ezt kell törölni minden megszakítás futás végén
}

#pragma interruptlow lo_isr
void lo_isr (void) {
  /* Itt történik az alacsony prioritású megszakítás tényleges kiszolgálása */
	TMR0H = 0x67;
	TMR0L = 0x69;
	
	LED_P = 1;
	Delay_ms(1);
	LED_P = 0;
	
	INTCONbits.TMR0IF = 0;		// Ezt kell törölni minden megszakítás futás végén
}
