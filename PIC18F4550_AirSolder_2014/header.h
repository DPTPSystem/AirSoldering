#pragma config PLLDIV   = 5         // PLL osztó 20 MHz-es kristályhoz
#pragma config CPUDIV   = OSC1_PLL2   
#pragma config USBDIV   = 2         // órajel forrása a 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS  // HS oszcillátor és PLL az USB-hez is
#pragma config FCMEN    = OFF       // Fail Safe Clock Monitor tiltása
#pragma config IESO     = OFF       // Belsõ/külsõ oszcillátor tiltása
#pragma config PWRT     = ON        // Bekapcsoláskor 62 ms várakozás
#pragma config BOR      = ON        // Brown our Reset engedélyezve
#pragma config BORV     = 2         // Reset szint = 2,7 V
#pragma config VREGEN   = ON        // VUSB belsõ stabilizátorról megy 
#pragma config WDT      = OFF       // Watchdog timer letiltva
#pragma config WDTPS    = 256       // Watchdog utószámláló beállítása
#pragma config MCLRE    = ON        // MCLR (reset) bemenet engedélyezve
#pragma config LPT1OSC  = OFF       // T1OSC nagyobb teljesítményû üzemmód
#pragma config PBADEN   = OFF       // PORTB<4:0> digitális I/O-ként inicializálódik
#pragma config CCP2MX   = ON        // RC1 legyen a CCP2 kimenet
#pragma config STVREN   = ON        // A verem túlcsordulása RESET-et okoz
#pragma config LVP      = OFF       // Alacsonyfeszültségû programozás letiltva
#pragma config ICPRT    = OFF       // Dedikált programozói kivezetés nincs
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config DEBUG    = OFF       // Háttér nyomkövetés letiltva, RB6/7 normál I/O
#pragma config CP0      = OFF       // Kódvédelem kikapcsolva
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#if defined(MCHPUSB_BOOTLOADER)     // A Boot blokkba csak ez a bootloader fér bele!
	#pragma config CPB      = ON    // Boot Blokk kódvédelem bekapcsolva
#else 
	 #pragma config CPB      = OFF   // Boot Blokk kódvédelem kikapcsolva
#endif
#pragma config CPD      = OFF
#pragma config WRT0     = OFF       // Írásvédelem kikapcsolva
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#if defined(MCHPUSB_BOOTLOADER)
	#pragma config WRTB     = ON    // Boot Blokk írásvédelem bekapcsolva
#else
	#pragma config WRTB     = OFF   // Boot Blokk írásvédelem kikapcsolva
#endif
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF       // Táblázatolvasás ne legyen letiltva
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF       //Boot blokk táblaolvasás ne legyen letiltva
#define CLOCK_FREQ 48000000