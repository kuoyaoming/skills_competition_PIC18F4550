#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include <xc.h>
#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

unsigned char T0Flag = 0;
unsigned char Timer1000m = 0;
unsigned char Timer500m = 0;
unsigned char pwmbuf;
unsigned char OutPortB = 0;

unsigned char ScanLine[4] = {0x01, 0x02,0x04,0x08};
const unsigned char LED[20]=
{
0x3f,     //0
0x06,     //1
0x5b,	  //2
0x4f,	  //3
0x66,	  //4
0x6d,	  //5
0x7d,	  //6
0x07,	  //7
0x7f,	  //8
0x6f,	  //9
0xbf,     //0.
0x86,     //1.
0xdb,	  //2.
0xcf,	  //3.
0xe6,	  //4.
0xed,	  //5.
0xfd,	  //6.
0x87,	  //7.
0xff,	  //8.
0xef,	  //9.
};
unsigned char disp[4];
void main(void) {
    
    // <editor-fold defaultstate="collapsed" desc="Timer0 setup">
    T0CONbits.T08BIT = 0; // 16Bits Mode
    T0CONbits.T0CS = 0; // Internal Clock
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0; // Enable Prescale
    T0CONbits.T0PS = 0x07; // 1:256#
    WRITETIMER0(65536-788); // 99.98mS Period, 500mS/4uS/256 = 788
    T0CONbits.TMR0ON = 1; // Timer0 Turn On
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    // </editor-fold>    
    
    // <editor-fold defaultstate="collapsed" desc="IO initialization">
    LATBbits.LATB0 = 1;
    TRISBbits.TRISB0= 0;
    LATBbits.LATB1 = 0;
    TRISBbits.TRISB1= 0;
    LATBbits.LATB2 = 1;
    TRISBbits.TRISB2= 0;
    LATBbits.LATB3 = 0;
    TRISBbits.TRISB3= 0;
    LATD = 0;
    TRISD = 0;    
    // </editor-fold>



    
unsigned int t = 0;
unsigned int tt = 0;
unsigned int i = 0;
unsigned int sec = 0;
unsigned int min = 0;



     while(1)
     {
         
            disp[0]=LED[sec%10];
            disp[1]=LED[sec/10%10];
//            disp[2]=LED[min%10];
            disp[3]=LED[min/10%10];
            
            if(Timer500m)
            {
                disp[2]=LED[(min%10)+10];
            }
            else
            {
                 disp[2]=LED[min%10];   
            }
            
            OutPortB=PORTB & 0xf0;
            
            PORTD=0xff;
            PORTB=OutPortB | 0x0f;
            
            if(++i>3)i=0;
            OutPortB = OutPortB | (~ScanLine[i]);
            PORTB = OutPortB;
            PORTD=~disp[i];    

        if (T0Flag)
        {
            T0Flag = 0;
            Timer1000m=1;
            if(++tt>14)
            {
                tt=0;
                Timer500m=~Timer500m;
            }    
            if(++t>29)
            {
                t=0;
                Timer1000m=1;
                if(sec++>58)
                {
                    sec=0;
                    
                    if(min++>58)
                    {
                        min=0;
                    }
                }
            }
        }
        
    }
}

void interrupt HighISR(void)
{
    if (TMR0IE && TMR0IF)
    {
        WRITETIMER0(65536-521);
        TMR0IF=0;
        T0Flag = 1;
    }
}
