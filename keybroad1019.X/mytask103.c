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

// <editor-fold defaultstate="collapsed" desc="comment">
//FOSC=HS  時脈速度,
//WDT=OFF, 看門狗關閉
//BOR = ON_ACTIVE, 低電源自動重置 
//BORV = 1  低電源自動重置
//LVP=OFF,  Sing-Supply ICSP Enable
//PBADEN=OFF, Port B A/D Enable bit
//XINST = OFF,  Extend Instruction Set Enable 
//MCLRE = ON, MCLR pin enable
// </editor-fold>





//#define InstructionFrequency 16000000 / 4
//#define T2Peroid 1000
//#define TMR2Value ( InstructionFrequency / 16 / T2Peroid )

unsigned char T0Flag = 0;
unsigned char Timer500m = 0;
unsigned char T10Flag = 0;
unsigned char Timer10m = 0;
unsigned char pwmbuf;
int timebase = 245;     //250-5 = 1ms
const unsigned char ScanLine[4] = {0x01, 0x02,0x04,0x08};
const unsigned char KeyMap[16] = {
    0x01, 0x05,0x09,0x0d,
    0x02, 0x06,0x0a,0x0e,
    0x03, 0x07,0x0b,0x0f,
    0x04, 0x08,0x0c,0x10
};
const unsigned char LED[18]={
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
    0x77,     //a
    0x7C,     //b
    0x39,     //c
    0x5E,     //d
    0x79,     //e
    0x71,     //f
    0x3D,     //g
    0x74      //h
    
    };
unsigned char disp[4];

int inval;
unsigned char OutPortB = 0;


void main(void) {
    
    // <editor-fold defaultstate="collapsed" desc="Timer0 setup">
    T0CONbits.T08BIT = 0;   // 16Bits Mode
    T0CONbits.T0CS = 0;     // Internal Clock  16M/4=4M  0.25uS
    T0CONbits.T0SE = 0;     // low-to-high transition on T0CKI(extreal clock)
    T0CONbits.PSA = 0;      // Enable Prescale
    T0CONbits.T0PS = 0x03;  // 1:16 0.25u*16=4uS
    WRITETIMER0(65536-timebase); //4u*250= 1ms
    T0CONbits.TMR0ON = 1;   // Timer0 Turn On
    INTCONbits.TMR0IF = 0;  //
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    // </editor-fold>    
    
    // <editor-fold defaultstate="collapsed" desc="IO initialization">
    
    ADCON1=0x0A;        //enable PORTE digital input
    TRISB0= 0;
    TRISB1= 0;
    TRISB2= 0;
    TRISB3= 0;
    TRISB4= 1;
    TRISB5= 1;
    TRISB6= 1;
    TRISB7= 1;
    TRISC6= 0;
    TRISC7=0;
    TRISD = 0;    
    TRISE0= 1;
    TRISE1= 1;
    TRISE2= 1;   
    // </editor-fold>
   
    unsigned int t = 0;
    unsigned int dig = 0;
    unsigned int test = 0;
    
    unsigned int col, row;
    unsigned int rowkey, kcode, keypress;
    
            
//==============================================================================
//Main Loop START---------------------------------------------------------------    
    
     while(1)
     {

 // <editor-fold defaultstate="collapsed" desc="4X4 key pad">
        if (Timer10m)      // 10mS for keyscan  
            {
                Timer10m = 0;
                LATD = 0xff;
                LATB = OutPortB | 0x0f;
                keypress=0;
                for(col=0;col<4;col++)
                {
                    LATB= ~ScanLine[col];
                    LATB= ~ScanLine[col];                                        
                    rowkey= (~PORTB) & 0xf0;
                    if(rowkey !=0)
                    {
                        if (rowkey==0x10) row=0;
                        if (rowkey==0x20) row=1;
                        if (rowkey==0x40) row=2;
                        if (rowkey==0x80) row=3;
                        inval= KeyMap[row + col *4];
                        keypress=1;
                    }
                }
                if (keypress==0) inval=17;
            }        
        // </editor-fold>       
        
        if (Timer500m)      // 500mS  
        {
            Timer500m = 0;
            test++;
 //           LATC7 = !LATC7;
        }            

        
//Display
        
        disp[0]=LED[test%10];
        disp[1]=LED[test/10%10];
        disp[2]=LED[test/100%10];
        disp[3]=LED[test/1000%10];
            
        //inval = PORTE & 0x07;
        disp[3] = LED[inval];        
        
 // <editor-fold defaultstate="collapsed" desc="時間基準程式碼">
   
        if (T0Flag)
        {
            T0Flag = 0;
            t++;
            if( t%10 ==0)
            {
                Timer10m=1;
            } 
            
            if(t>=500)
            {
                t=0;
                Timer500m=1;
            }
            
            // 7-seg LED Scan Out 
            
            LATD = 0xff;
            OutPortB = PORTB & 0xf0;
            LATB = OutPortB | 0x0f;
            OutPortB = OutPortB | (~ScanLine[dig]);
            LATB = OutPortB;
            LATD = ~disp[dig]; 
            if(++dig>3) dig=0;
        }
 // </editor-fold>
    }
}

//Main Loop END ----------------------------------------------------------------
//==============================================================================

// <editor-fold defaultstate="collapsed" desc="T0 int">

void interrupt HighISR(void) 
{
    if (TMR0IE && TMR0IF)
    {
        TMR0IF=0;
        WRITETIMER0(65536-timebase); // 99.98mS Period, 500mS/4uS/256 = 788
        T0Flag = 1;
    }
}
// </editor-fold>