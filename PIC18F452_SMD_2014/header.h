//PIC18F452 koncigurációs regiszterek
#pragma config OSC      = HSPLL    // Külsõ oscillator + PLL bekapcsolva
#pragma config OSCS     = OFF      // Osc. kapcsoló

#pragma config BOR      = ON       // Alacsony feszültségnél reset
#pragma config BORV     = 27       // 2.7V-nál már resetel
#pragma config PWRT     = OFF      // Feszültség mérõ timer

#pragma config WDT      = OFF      // Watchdog bekapcsolva
#pragma config WDTPS    = 128      // 1:128 Watchdog idõzítés osztoja

#pragma config CCP2MUX  = OFF      // PWM2 kikapcsolva (RB3)

#pragma config STVR     = ON       // Werem túlcsordulás esetén reset
#pragma config LVP      = OFF      // Alacsony feszültségû programozás tiltása
#pragma config DEBUG    = OFF      // Debug, nyomonkövetés kikapcsolva

#pragma config CP0      = OFF      // Kód védelem
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF

#pragma config CPB      = OFF      // Boot block kódvédelem
#pragma config CPD      = OFF      // Eeprom kód védelem

#pragma config WRT0     = OFF      // Írás védelem
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF

#pragma config WRTB     = OFF      // Boot block írásvédelem
#pragma config WRTC     = OFF      // Konfigurációs regiszter írás védelme
#pragma config WRTD     = OFF      // Eeprom írás védelem

#pragma config EBTR0    = OFF      // Tábla olvasási védelem
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF      // Boot block tábla olvasási védelem
