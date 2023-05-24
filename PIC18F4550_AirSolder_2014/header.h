#pragma config PLLDIV   = 5         // PLL oszt� 20 MHz-es krist�lyhoz
#pragma config CPUDIV   = OSC1_PLL2   
#pragma config USBDIV   = 2         // �rajel forr�sa a 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS  // HS oszcill�tor �s PLL az USB-hez is
#pragma config FCMEN    = OFF       // Fail Safe Clock Monitor tilt�sa
#pragma config IESO     = OFF       // Bels�/k�ls� oszcill�tor tilt�sa
#pragma config PWRT     = ON        // Bekapcsol�skor 62 ms v�rakoz�s
#pragma config BOR      = ON        // Brown our Reset enged�lyezve
#pragma config BORV     = 2         // Reset szint = 2,7 V
#pragma config VREGEN   = ON        // VUSB bels� stabiliz�torr�l megy 
#pragma config WDT      = OFF       // Watchdog timer letiltva
#pragma config WDTPS    = 256       // Watchdog ut�sz�ml�l� be�ll�t�sa
#pragma config MCLRE    = ON        // MCLR (reset) bemenet enged�lyezve
#pragma config LPT1OSC  = OFF       // T1OSC nagyobb teljes�tm�ny� �zemm�d
#pragma config PBADEN   = OFF       // PORTB<4:0> digit�lis I/O-k�nt inicializ�l�dik
#pragma config CCP2MX   = ON        // RC1 legyen a CCP2 kimenet
#pragma config STVREN   = ON        // A verem t�lcsordul�sa RESET-et okoz
#pragma config LVP      = OFF       // Alacsonyfesz�lts�g� programoz�s letiltva
#pragma config ICPRT    = OFF       // Dedik�lt programoz�i kivezet�s nincs
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config DEBUG    = OFF       // H�tt�r nyomk�vet�s letiltva, RB6/7 norm�l I/O
#pragma config CP0      = OFF       // K�dv�delem kikapcsolva
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#if defined(MCHPUSB_BOOTLOADER)     // A Boot blokkba csak ez a bootloader f�r bele!
	#pragma config CPB      = ON    // Boot Blokk k�dv�delem bekapcsolva
#else 
	 #pragma config CPB      = OFF   // Boot Blokk k�dv�delem kikapcsolva
#endif
#pragma config CPD      = OFF
#pragma config WRT0     = OFF       // �r�sv�delem kikapcsolva
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#if defined(MCHPUSB_BOOTLOADER)
	#pragma config WRTB     = ON    // Boot Blokk �r�sv�delem bekapcsolva
#else
	#pragma config WRTB     = OFF   // Boot Blokk �r�sv�delem kikapcsolva
#endif
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF       // T�bl�zatolvas�s ne legyen letiltva
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF       //Boot blokk t�blaolvas�s ne legyen letiltva
#define CLOCK_FREQ 48000000