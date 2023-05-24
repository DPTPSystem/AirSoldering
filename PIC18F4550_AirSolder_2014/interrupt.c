void lo_isr(void);        //el�re deklar�ljuk a kiszolg�l�
void hi_isr(void);        //elj�r�sokat

/*PIC18F4550 csak a B PORT fels� 4 bitj�n tud megszak�t�st kezelni, k�l�n nem v�laszthat� (RB7...RB4)
*/
/** Magas �s alacsony priorit�s� interrupt vektor
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
#pragma code            					 //visszat�r�nk az alap�rtelmezett k�dszekci�ba
#pragma interrupt hi_isr
void hi_isr (void) {
/* Itt t�rt�nik a magas priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
	TMR0H = 0x91;
	TMR0L = 0xC8;
	/****************************************
	+	Als� f�t�s figyel�se �s vez�rl�se	+
	****************************************/
	if(ERVPROFINT[13] > Max6675(3)){		//Ha als� f�t�s m�g nem �rte el a kell� �rt�ket
		AlsoFutesRele = !AlsoFutesRele;		//Als�f�t�s enged�lyezve
	}else{
		AlsoFutesRele = 0;					//Als�f�t�s t�lt�sa
	}
	
	/****************************************
	+	Fels� f�t�s figyel�se �s vez�rl�se	+
	****************************************/
	if(ERVPROFINT[Hofoklepes] > Max6675(2)){	//Ha fels� f�t�s m�g nem �rte el a kell� �rt�ket
		FelsoFutesRele = !FelsoFutesRele;		//Fels� f�t�s enged�lyezve
	}else{
		FelsoFutesRele = 0;						//Fels� f�t�s t�lt�sa
	}	
	
	/****************************************
	+		H�fok adatok �tlagol�s�hoz		+
	****************************************/
	if(AtlagBTCount > 4){
		AtlagBTCount = 0;
	}else{
		AtlagBTCount++;
	}
	Tido++;										//Emelj�k Tido-t
	INTCONbits.TMR0IF = 0;						// Ezt kell t�r�lni minden megszak�t�s fut�s v�g�n
}
#pragma interruptlow lo_isr
void lo_isr (void) {
  /* Itt t�rt�nik az alacsony priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
}
