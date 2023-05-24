/**************************************************************************
A port definici�ja
**************************************************************************/
/*TRISA*/
#define TRIS_A0		TRISAbits.TRISA0 //2-l�b
#define TRIS_A1		TRISAbits.TRISA1 //3.l�b
#define TRIS_A2		TRISAbits.TRISA2 //4.l�b
#define TRIS_A3		TRISAbits.TRISA3 //5.l�b
#define TRIS_A4		TRISAbits.TRISA4 //6.l�b
#define TRIS_A5		TRISAbits.TRISA5 //7-l�b
//#define TRIS_A6		TRISAbits.TRISA6 //14.l�b OSC 1 (kirst�ly) 
//#define TRIS_A7		TRISAbits.TRISA7 //13.l�b OSC 2 (krist�ly)

/*PORTA*/
#define PORT_A0		PORTAbits.RA0 //2-l�b
#define PORT_A1		PORTAbits.RA1 //3.l�b
#define PORT_A2		PORTAbits.RA2 //4.l�b
#define PORT_A3		PORTAbits.RA3 //5.l�b
#define PORT_A4		PORTAbits.RA4 //6.l�b
#define PORT_A5		PORTAbits.RA5 //7-l�b
//#define PORT_A6		PORTAbits.RA6 //14.l�b OSC 1 (kirst�ly) 
//#define PORT_A7		PORTAbits.RA7 //13.l�b OSC 2 (krist�ly)

/*LATA*/
#define LAT_A0		LATAbits.LATA0 //2-l�b
#define LAT_A1		LATAbits.LATA1 //3.l�b
#define LAT_A2		LATAbits.LATA2 //4.l�b
#define LAT_A3		LATAbits.LATA3 //5.l�b
#define LAT_A4		LATAbits.LATA4 //6.l�b
#define LAT_A5		LATAbits.LATA5 //7-l�b
//#define LAT_A6		LATAbits.LATA6 //14.l�b OSC 1 (kirst�ly) 
//#define LAT_A7		LATAbits.LATA7 //13.l�b OSC 2 (krist�ly)

/**************************************************************************
B port definici�ja
**************************************************************************/
/*TRISB*/
#define TRIS_B0		TRISBbits.TRISB0 //33.l�b
#define TRIS_B1		TRISBbits.TRISB1 //34.l�b
#define TRIS_B2		TRISBbits.TRISB2 //35.l�b
#define TRIS_B3		TRISBbits.TRISB3 //36.l�b
#define TRIS_B4		TRISBbits.TRISB4 //37.l�b
#define TRIS_B5		TRISBbits.TRISB5 //38.l�b
#define TRIS_B6		TRISBbits.TRISB6 //39.l�b
#define TRIS_B7		TRISBbits.TRISB7 //40.l�b

/*PORTB*/
#define PORT_B0		PORTBbits.RB0 //33.l�b
#define PORT_B1		PORTBbits.RB1 //34.l�b
#define PORT_B2		PORTBbits.RB2 //35.l�b
#define PORT_B3		PORTBbits.RB3 //36.l�b
#define PORT_B4		PORTBbits.RB4 //37.l�b
#define PORT_B5		PORTBbits.RB5 //38.l�b
#define PORT_B6		PORTBbits.RB6 //39.l�b
#define PORT_B7		PORTBbits.RB7 //40.l�b

/*LATB*/
#define LAT_B0		LATBbits.LATB0 //33.l�b
#define LAT_B1		LATBbits.LATB1 //34.l�b
#define LAT_B2		LATBbits.LATB2 //35.l�b
#define LAT_B3		LATBbits.LATB3 //36.l�b
#define LAT_B4		LATBbits.LATB4 //37.l�b
#define LAT_B5		LATBbits.LATB5 //38.l�b
#define LAT_B6		LATBbits.LATB6 //39.l�b
#define LAT_B7		LATBbits.LATB7 //40.l�b

/**************************************************************************
C port definici�ja
**************************************************************************/
/*TRISC*/
#define TRIS_C0		TRISCbits.TRISC0 //15.l�b LCD RS
#define TRIS_C1		TRISCbits.TRISC1 //16.l�b LCD RW
#define TRIS_C2		TRISCbits.TRISC2 //17.l�b LCD E
#define TRIS_C3		TRISCbits.TRISC3 //18.l�b VUSB
#define TRIS_C4		TRISCbits.TRISC4 //23.l�b
#define TRIS_C5		TRISCbits.TRISC5 //24.l�b
#define TRIS_C6		TRISCbits.TRISC6 //25.l�b
#define TRIS_C7		TRISCbits.TRISC7 //26.l�b

/*PORTC*/
#define PORT_C0		PORTCbits.RC0 //15.l�b LCD RS
#define PORT_C1		PORTCbits.RC1 //16.l�b LCD RW
#define PORT_C2		PORTCbits.RC2 //17.l�b LCD E
#define PORT_C3		PORTCbits.RC3 //18.l�b VUSB
#define PORT_C4		PORTCbits.RC4 //23.l�b
#define PORT_C5		PORTCbits.RC5 //24.l�b
#define PORT_C6		PORTCbits.RC6 //25.l�b
#define PORT_C7		PORTCbits.RC7 //26.l�b

/*LATC*/
#define LAT_C0		LATCbits.LATC0 //15.l�b LCD RS
#define LAT_C1		LATCbits.LATC1 //16.l�b LCD RW
#define LAT_C2		LATCbits.LATC2 //17.l�b LCD E
#define LAT_C3		LATCbits.LATC3 //18.l�b VUSB
#define LAT_C4		LATCbits.LATC4 //23.l�b
#define LAT_C5		LATCbits.LATC5 //24.l�b
#define LAT_C6		LATCbits.LATC6 //25.l�b
#define LAT_C7		LATCbits.LATC7 //26.l�b

/**************************************************************************
D port definici�ja
**************************************************************************/
/*TRISD*/
#define TRIS_D0		TRISDbits.TRISD0 //19.l�b
#define TRIS_D1		TRISDbits.TRISD1 //20.l�b
#define TRIS_D2		TRISDbits.TRISD2 //21.l�b
#define TRIS_D3		TRISDbits.TRISD3 //22.l�b
#define TRIS_D4		TRISDbits.TRISD4 //27.l�b LCD DATA
#define TRIS_D5		TRISDbits.TRISD5 //28.l�b LCD DATA
#define TRIS_D6		TRISDbits.TRISD6 //29.l�b LCD DATA
#define TRIS_D7		TRISDbits.TRISD7 //30.l�b LCD DATA

/*PORTD*/
#define PORT_D0		PORTDbits.RD0 //19.l�b
#define PORT_D1		PORTDbits.RD1 //20.l�b
#define PORT_D2		PORTDbits.RD2 //21.l�b
#define PORT_D3		PORTDbits.RD3 //22.l�b
#define PORT_D4		PORTDbits.RD4 //27.l�b LCD DATA
#define PORT_D5		PORTDbits.RD5 //28.l�b LCD DATA
#define PORT_D6		PORTDbits.RD6 //29.l�b LCD DATA
#define PORT_D7		PORTDbits.RD7 //30.l�b LCD DATA

/*LATD*/
#define LAT_D0		LATDbits.LATD0 //19.l�b
#define LAT_D1		LATDbits.LATD1 //20.l�b
#define LAT_D2		LATDbits.LATD2 //21.l�b
#define LAT_D3		LATDbits.LATD3 //22.l�b
#define LAT_D4		LATDbits.LATD4 //27.l�b LCD DATA
#define LAT_D5		LATDbits.LATD5 //28.l�b LCD DATA
#define LAT_D6		LATDbits.LATD6 //29.l�b LCD DATA
#define LAT_D7		LATDbits.LATD7 //30.l�b LCD DATA

/**************************************************************************
E port definici�ja
**************************************************************************/
/*TRISE*/
#define TRIS_E0		TRISEbits.TRISE0 //8.l�b
#define TRIS_E1		TRISEbits.TRISE1 //9.l�b
#define TRIS_E2		TRISEbits.TRISE2 //10.l�b
//#define TRIS_E3		TRISEbits.TRISE3 //1.l�b VPP 5v-be (MCLR)

/*PORTE*/
#define PORT_E0		PORTEbits.RE0 //8.l�b
#define PORT_E1		PORTEbits.RE1 //9.l�b
#define PORT_E2		PORTEbits.RE2 //10.l�b
//#define PORT_E3		PORTEbits.RE3 //1.l�b VPP 5v-be (MCLR)

/*LATE*/
#define LAT_E0		LATEbits.LATE0 //8.l�b
#define LAT_E1		LATEbits.LATE1 //9.l�b
#define LAT_E2		LATEbits.LATE2 //10.l�b
//#define LAT_E3		LATEbits.LATE3 //1.l�b VPP 5v-be (MCLR)

/**************************************************************************
Portok �s l�bak be�llt�sai
**************************************************************************/
#define pin_in(PIN) 										PIN=1 //1-l�b bemenetre �ll�t�sa
#define pin_in2(PIN1,PIN2) 									PIN1=1,PIN2=1 //2-l�b bemenetre �ll�t�sa
#define pin_in3(PIN1,PIN2,PIN3) 							PIN1=1,PIN2=1,PIN3=1 //3-l�b bemenetre �ll�t�sa
#define pin_in4(PIN1,PIN2,PIN3,PIN4) 						PIN1=1,PIN2=1,PIN3=1,PIN4=1 //4-l�b bemenetre �ll�t�sa
#define pin_in5(PIN1,PIN2,PIN3,PIN4,PIN5) 					PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1 //5-l�b bemenetre �ll�t�sa
#define pin_in6(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6) 				PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1,PIN6=1 //6-l�b bemenetre �ll�t�sa
#define pin_in7(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7) 		PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1,PIN6=1,PIN7=1 //7-l�b bemenetre �ll�t�sa
#define pin_in8(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7,PIN8) 	PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1,PIN6=1,PIN7=1,PIN8=1 //8-l�b bemenetre �ll�t�sa

#define pin_out(PIN) 										PIN=0 //1-l�b bemenetre �ll�t�sa
#define pin_out2(PIN1,PIN2) 								PIN1=0,PIN2=0 //2-l�b bemenetre �ll�t�sa
#define pin_out3(PIN1,PIN2,PIN3) 							PIN1=0,PIN2=0,PIN3=0 //3-l�b bemenetre �ll�t�sa
#define pin_out4(PIN1,PIN2,PIN3,PIN4) 						PIN1=0,PIN2=0,PIN3=0,PIN4=0 //4-l�b bemenetre �ll�t�sa
#define pin_out5(PIN1,PIN2,PIN3,PIN4,PIN5) 					PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0 //5-l�b bemenetre �ll�t�sa
#define pin_out6(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6) 			PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0,PIN6=0 //6-l�b bemenetre �ll�t�sa
#define pin_out7(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7) 		PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0,PIN6=0,PIN7=0 //7-l�b bemenetre �ll�t�sa
#define pin_out8(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7,PIN8) 	PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0,PIN6=0,PIN7=0,PIN8=0 //8-l�b bemenetre �ll�t�sa

#define pin_high(PIN) 										PIN=1 //1-l�b bemenetre �ll�t�sa
#define pin_high2(PIN1,PIN2) 								PIN1=1,PIN2=1 //2-l�b bemenetre �ll�t�sa
#define pin_high3(PIN1,PIN2,PIN3) 							PIN1=1,PIN2=1,PIN3=1 //3-l�b bemenetre �ll�t�sa
#define pin_high4(PIN1,PIN2,PIN3,PIN4) 						PIN1=1,PIN2=1,PIN3=1,PIN4=1 //4-l�b bemenetre �ll�t�sa
#define pin_high5(PIN1,PIN2,PIN3,PIN4,PIN5) 				PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1 //5-l�b bemenetre �ll�t�sa
#define pin_high6(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6) 			PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1,PIN6=1 //6-l�b bemenetre �ll�t�sa
#define pin_high7(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7) 		PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1,PIN6=1,PIN7=1 //7-l�b bemenetre �ll�t�sa
#define pin_high8(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7,PIN8) 	PIN1=1,PIN2=1,PIN3=1,PIN4=1,PIN5=1,PIN6=1,PIN7=1,PIN8=1 //8-l�b bemenetre �ll�t�sa

#define pin_low(PIN) 										PIN=0 //1-l�b bemenetre �ll�t�sa
#define pin_low2(PIN1,PIN2) 								PIN1=0,PIN2=0 //2-l�b bemenetre �ll�t�sa
#define pin_low3(PIN1,PIN2,PIN3) 							PIN1=0,PIN2=0,PIN3=0 //3-l�b bemenetre �ll�t�sa
#define pin_low4(PIN1,PIN2,PIN3,PIN4) 						PIN1=0,PIN2=0,PIN3=0,PIN4=0 //4-l�b bemenetre �ll�t�sa
#define pin_low5(PIN1,PIN2,PIN3,PIN4,PIN5) 					PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0 //5-l�b bemenetre �ll�t�sa
#define pin_low6(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6) 			PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0,PIN6=0 //6-l�b bemenetre �ll�t�sa
#define pin_low7(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7) 		PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0,PIN6=0,PIN7=0 //7-l�b bemenetre �ll�t�sa
#define pin_low8(PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7,PIN8) 	PIN1=0,PIN2=0,PIN3=0,PIN4=0,PIN5=0,PIN6=0,PIN7=0,PIN8=0 //8-l�b bemenetre �ll�t�sa