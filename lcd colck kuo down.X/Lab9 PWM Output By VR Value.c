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
#define P1  RB0   	//定義獨立按鍵P1為RB0
#define P2  RB1   	//定義獨立按鍵P2為RB1
#define P3  RB2   	//定義獨立按鍵P3為RB2
#define P4  RB3   	//定義獨立按鍵P4為RB3
#define P5  RB4   	//定義獨立按鍵P5為RB4
#define P6  RB5   	//定義獨立按鍵P6為RB5
#define P7  RB6   	//定義獨立按鍵P7為RB6
#define P8  RB7   	//定義獨立按鍵P8為RB7
const unsigned char MyName[] = "My Clock";
const unsigned char String1[] = ":";

unsigned char StringBuffer[ 32 ];
volatile unsigned char Timer0Flag = 0;
unsigned int Duty = 0;
unsigned int s=59;
unsigned int m=59;
unsigned int h=23;

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
    LCM_SetCursor(3, 1); // Set Cursor to 0,1
    LCM_PutROMString(String1); // Print String 	 
    LCM_SetCursor(6, 1); // Set Cursor to 0,1
    LCM_PutROMString(String1); // Print String
    
    while (1)
    {
        if (Timer0Flag)
        {
            Timer0Flag = 0;
            Duty++;
        }
        if(Duty>1)
        {
            Duty=0;
            s--;
        }
        if(s<1)
        {
            s=59;
            m--;
        }
        if(m<1)
        {
            m=59;
            h--;
         }
        if(h<1)h=23;
        
        if(!P2)h--;
        if(!P3)m--;
        if(!P4)s--;
        
            LCM_SetCursor(1, 1); // Set Cursor to 4,1
            LCM_PutNumber(h, 2); // Print AD Value ( 0 ~ 1023 )
            LCM_SetCursor(4, 1); // Set Cursor to 4,1
            LCM_PutNumber(m, 2); // Print AD Value ( 0 ~ 1023 )    
            LCM_SetCursor(7, 1); // Set Cursor to 4,1
            LCM_PutNumber(s, 2); // Print AD Value ( 0 ~ 1023 )
    }
}

void interrupt HighISR(void)         // 高優先權中斷函數 
{ 
    if (TMR0IE && TMR0IF && P1) 
    { 
 //       WriteTimer0(65536-488);     // 500mS Period, 500mS/4uS/256 = 488 
        TMR0H = TMR0HighValue;
        TMR0L = TMR0LowValue;
         TMR0IF=0; 
        Timer0Flag = 1;
    } 
} 