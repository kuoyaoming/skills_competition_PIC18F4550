#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "APP001_LCM.h"

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define InstructionFrequency 4000000 / 4

#define T0Peroid 100
#define TMR0Value ( 65536 - ( InstructionFrequency / 256 / T0Peroid ))
#define TMR0HighValue ( ( TMR0Value >> 8 ) & 0xff )
#define TMR0LowValue ( TMR0Value & 0xff )

#define T2Peroid 1000
#define TMR2Value ( InstructionFrequency / 16 / T2Peroid )

const unsigned char MyName[] = "Adam Syu";
const unsigned char String1[] = "VR1:";

unsigned char StringBuffer[ 32 ];
volatile unsigned char Timer0Flag = 0;
unsigned int Duty = 0;


void main(void)
{
    LATCbits.LATC2 = 0;
    TRISCbits.TRISC2 = 0;
    LATD = 0;
    TRISD = 0;

    T0CONbits.T08BIT = 0; // 16Bits Mode
    T0CONbits.T0CS = 0; // Internal Clock
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0; // Enable Prescale
    T0CONbits.T0PS = 0x07; // 1:256
    TMR0H = TMR0HighValue;
    TMR0L = TMR0LowValue;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    T0CONbits.TMR0ON = 1; // Timer0 Turn On

  

    LCM_Init(); // Initial LCD Module

    LCM_SetCursor(0, 0); // Set Cursor to 0,0
    LCM_PutROMString(MyName); // Print String   
    LCM_SetCursor(0, 1); // Set Cursor to 0,1
    LCM_PutROMString(String1); // Print String 	 


    while (1)
    {
        if (Timer0Flag)
        {
            Timer0Flag = 0;
            Duty++;

            LCM_SetCursor(4, 1); // Set Cursor to 4,1
            LCM_PutNumber(Duty, 4); // Print AD Value ( 0 ~ 1023 )




        }
    }
}

void interrupt HighISR(void)         // 高優先權中斷函數 
{ 
    if (TMR0IE && TMR0IF) 
    { 
 //       WriteTimer0(65536-488);     // 500mS Period, 500mS/4uS/256 = 488 
        TMR0H = TMR0HighValue;
        TMR0L = TMR0LowValue;
         TMR0IF=0; 
        Timer0Flag = 1;
    } 
} 