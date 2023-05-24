//PIC18F452 koncigur�ci�s regiszterek
#pragma config OSC      = HSPLL    // K�ls� oscillator + PLL bekapcsolva
#pragma config OSCS     = OFF      // Osc. kapcsol�

#pragma config BOR      = ON       // Alacsony fesz�lts�gn�l reset
#pragma config BORV     = 27       // 2.7V-n�l m�r resetel
#pragma config PWRT     = OFF      // Fesz�lts�g m�r� timer

#pragma config WDT      = OFF      // Watchdog bekapcsolva
#pragma config WDTPS    = 128      // 1:128 Watchdog id�z�t�s osztoja

#pragma config CCP2MUX  = OFF      // PWM2 kikapcsolva (RB3)

#pragma config STVR     = ON       // Werem t�lcsordul�s eset�n reset
#pragma config LVP      = OFF      // Alacsony fesz�lts�g� programoz�s tilt�sa
#pragma config DEBUG    = OFF      // Debug, nyomonk�vet�s kikapcsolva

#pragma config CP0      = OFF      // K�d v�delem
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF

#pragma config CPB      = OFF      // Boot block k�dv�delem
#pragma config CPD      = OFF      // Eeprom k�d v�delem

#pragma config WRT0     = OFF      // �r�s v�delem
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF

#pragma config WRTB     = OFF      // Boot block �r�sv�delem
#pragma config WRTC     = OFF      // Konfigur�ci�s regiszter �r�s v�delme
#pragma config WRTD     = OFF      // Eeprom �r�s v�delem

#pragma config EBTR0    = OFF      // T�bla olvas�si v�delem
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF      // Boot block t�bla olvas�si v�delem
