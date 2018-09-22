/* 
 * File:   4-7seg.c
 * Author: school
 *
 * Created on 2015年10月11日, 上午 9:52
 */
#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include <xc.h>
#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON 

/*這句話設定的意義要查出來。
FOSC=HS  時脈速度,
WDT=OFF, 看門狗關閉
BOR = ON_ACTIVE, 低電源自動重置 
BORV = 1  低電源自動重置
LVP=OFF,  Sing-Supply ICSP Enable
PBADEN=OFF, Port B A/D Enable bit
ADCON1=0x0F;
XINST = OFF,  Extend Instruction Set Enable 
MCLRE = ON, MCLR pin enable
*/

//#define InstructionFrequency 16000000 / 4
//#define T2Peroid 1000
//#define TMR2Value ( InstructionFrequency / 16 / T2Peroid )
unsigned int t = 0;
unsigned int i = 0;
unsigned int test = 0;
unsigned char T0Flag = 0;
unsigned char Timer500m = 0;
unsigned char pwmbuf;
unsigned char OutPortB = 0;//掃描用port 暫存質
int timebase = 245;
unsigned char ScanLine[4] = {0x01, 0x02,0x04,0x08};
const unsigned char LED[10]=
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
};
unsigned char disp[4];
int inval;



void main(void) {
    
    // <editor-fold defaultstate="collapsed" desc="Timer0 setup">
    T0CONbits.T08BIT = 0; // 16Bits Mode
    T0CONbits.T0CS = 0; // Internal Clock
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0; // Enable Prescale
    T0CONbits.T0PS = 0x03; // 1:256
    WRITETIMER0(65536-timebase); // 99.98mS Period, 500mS/4uS/256 = 788
    T0CONbits.TMR0ON = 1; // Timer0 Turn On
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    // </editor-fold>    
    
    // <editor-fold defaultstate="collapsed" desc="IO initialization">
    
    ADCON1=0x0A;
    TRISBbits.TRISB0= 0;
    TRISBbits.TRISB1= 0;
    TRISBbits.TRISB2= 0;
    TRISBbits.TRISB3= 0;
    TRISBbits.TRISB4= 0;
    TRISD = 0;    
    TRISEbits.TRISE0= 1;
    TRISEbits.TRISE1= 1;
    TRISEbits.TRISE2= 1;   
    // </editor-fold>

     while(1)
     {
         switch(inval)
         {
            case 1:
                 
                 break;
            case 2:
                 
                 break;
            case 3:
                 
                 break;
         }
         
         
         
         disp[0]=LED[test%10];
        disp[1]=LED[test/10%10];
        disp[2]=LED[test/100%10];
       // disp[3]=LED[inval%10];
            
        inval=PORTE & 0x07;
        disp[3]=LED[inval%10];
        OutPortB=PORTB & 0xf0;
        PORTD=0xff;
        PORTB=OutPortB | 0x0f;
            
        if(++i>3)i=0;
        OutPortB = OutPortB | (~ScanLine[i]);
        PORTB = OutPortB;
        PORTD=~disp[i];    
            

        
        
 // <editor-fold defaultstate="collapsed" desc="時間基準程式碼">
   
        if (T0Flag)
        {
            T0Flag = 0;
            Timer500m=1;

            if(++t>10)
            {
                t=0;
                Timer500m=1;
                test++;
            }
        }
         // </editor-fold>
    }
}


// <editor-fold defaultstate="collapsed" desc="T0 中斷">


void interrupt HighISR(void) // ????????
{
    if (TMR0IE && TMR0IF)
    {
        WRITETIMER0(65536-timebase); // 99.98mS Period, 500mS/4uS/256 = 788
        TMR0IF=0;
        T0Flag = 1;
//        LATBbits.LATB7 = !LATBbits.LATB7 ;
    }
}
// </editor-fold>