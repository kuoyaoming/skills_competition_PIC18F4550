#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include <xc.h>
#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON
// <editor-fold defaultstate="collapsed" desc="宣告區">
unsigned char T0Flag = 0;
unsigned char Timer10m = 0;
unsigned char Timer500m = 0;
unsigned char pwmbuf;
unsigned char OutPortB = 0;
unsigned int a = 0;
unsigned int t = 0;
unsigned int i = 0;
unsigned int sec = 0;
unsigned int min = 0;
unsigned int hur = 0;
unsigned char ScanLine[4] = {0x01, 0x02,0x04,0x08};
unsigned int rowkey, kcode, keypress;
unsigned int col, row;
int inval;
int ggg=0;
int r;
int rrr;
int e;
int  Timer250m=0;
unsigned char disp[4];
int mq = 0;
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="陣列區">
const unsigned char KeyMap[16] = {
    0x01, 0x05,0x09,0x0d,
    0x02, 0x06,0x0a,0x0e,
    0x03, 0x07,0x0b,0x0f,
    0x04, 0x08,0x0c,0x00
};
const unsigned char LED[20]={
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
const unsigned char MQ[48] = {
    0x01,0x02,0x04,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//A
    0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x01,//B    
    0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x01,0x00,//C
    0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x20,0x01,0x00,0x00,//D
};
// </editor-fold>
void main(void) {
    // <editor-fold defaultstate="collapsed" desc="Timer0 setup">
    ADCON1=0x0A;
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
    ADCON1=0x0A;
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
TRISAbits.TRISA1= 1;    
    // </editor-fold>
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
    // <editor-fold defaultstate="collapsed" desc="模式選擇區">
    a = PORTE & 0x07;
    switch(a)
    {
        // <editor-fold defaultstate="collapsed" desc="case 1">
        case 1:
            disp[0]=LED[0];
            disp[1]=LED[0];
            disp[2]=LED[0];
            disp[3]=LED[0];
            break;
            // </editor-fold>//不動
        // <editor-fold defaultstate="collapsed" desc="case 2">
        case 2:
            if (rrr){
                rrr=0;
                if(ggg++>9999)ggg=0;
            }
            disp[0]=LED[ggg%10];
            disp[1]=LED[ggg/10%10];
            disp[2]=LED[ggg/100%10];
            disp[3]=LED[ggg/1000%10];
            break;
            // </editor-fold>//上數
        // <editor-fold defaultstate="collapsed" desc="case 3">    
        case 3:
            if (rrr){
                rrr=0;
                if(ggg--<0)ggg=9999;
            }
            disp[0]=LED[ggg%10];
            disp[1]=LED[ggg/10%10];
            disp[2]=LED[ggg/100%10];
            disp[3]=LED[ggg/1000%10];
            break;
            // </editor-fold>//下數
        // <editor-fold defaultstate="collapsed" desc="case 4">
        case 4:
            if (Timer500m){
            Timer500m=0;
                if(sec++>58)
                {
                    sec=0;
                    if(min++>58)
                    {
                        min=0;
                    }
                }
            }
            disp[0]=LED[sec%10];
            disp[1]=LED[sec/10%10];
            if(Timer250m)
            {
            disp[2]=LED[(min%10)+10];
            }
            else
            {
            disp[2]=LED[min%10];
            }
            disp[3]=LED[min/10%10];
            break;
            // </editor-fold>//時鐘
        // <editor-fold defaultstate="collapsed" desc="case 5">
        case 5:
            if (Timer500m){
            Timer500m=0;
                if(sec--<1)
                {
                    sec=59;
                    if(min--<1)
                    {
                        min=59;
                    }
                }
            }
            disp[0]=LED[sec%10];
            disp[1]=LED[sec/10%10];
            if(Timer250m){
            disp[2]=LED[(min%10)+10];
            }else{
            disp[2]=LED[min%10];
            }
            disp[3]=LED[min/10%10];
            break;
            // </editor-fold>//倒數時鐘
        // <editor-fold defaultstate="collapsed" desc="case 6">
        case 6:
            if (rrr){
            rrr=0;
            if(++mq>11)mq=0;
            }
            disp[0]=MQ[mq];
            disp[1]=MQ[mq+12];
            disp[2]=MQ[mq+24];
            disp[3]=MQ[mq+36];
            break;
            // </editor-fold>//順跑馬燈
        // <editor-fold defaultstate="collapsed" desc="case 7">
        case 7:
            if (rrr){
            rrr=0;
            if(--mq<0)mq=11;
            }
            disp[0]=MQ[mq];
            disp[1]=MQ[mq+12];
            disp[2]=MQ[mq+24];
            disp[3]=MQ[mq+36];
            break;
            // </editor-fold>//逆跑馬燈
        // <editor-fold defaultstate="collapsed" desc="case 8">
        case 8:
            disp[0]=LED[0];
            disp[1]=LED[0];
            disp[2]=LED[0];
            disp[3]=LED[0];
            break;
            // </editor-fold>//鍵盤
        default:
            break;
    }
    // </editor-fold>
    // <editor-fold defaultstate="collapsed" desc="七段掃描區">
    OutPortB=PORTB & 0xf0;
    PORTD=0xff;
    PORTB=OutPortB | 0x0f;
    if(++i>3)i=0;
    OutPortB = OutPortB | (~ScanLine[i]);
    PORTB = OutPortB;
    PORTD=~disp[i];
    // </editor-fold>
    // <editor-fold defaultstate="collapsed" desc="旗標區">
    if (T0Flag)
        {
            T0Flag = 0;
            if(++r>1)
            {
                r=0;
                rrr=1;
            }
            if(++e>14)
            {
                e=0;
                Timer250m=~Timer250m;
            }
            if(++t>29)
            {
                t=0;
                Timer500m=1;
            }
        }
    // </editor-fold>    
    }
}
// <editor-fold defaultstate="collapsed" desc="T0 計時區">
void interrupt HighISR(void)
{
    if (TMR0IE && TMR0IF)
    {
        WRITETIMER0(65536-521);
        TMR0IF=0;
        T0Flag = 1;
    }
}
// </editor-fold>