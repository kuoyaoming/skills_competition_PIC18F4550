#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define InstructionFrequency 4000000 / 4

#define T2Peroid 1000
#define TMR2Value ( InstructionFrequency / 16 / T2Peroid )

#define T0Peroid 100
#define TMR0Value ( 65536 - ( InstructionFrequency / 256 / T0Peroid ))
#define TMR0HighValue ( ( TMR0Value >> 8 ) & 0xff )
#define TMR0LowValue ( TMR0Value & 0xff )

volatile unsigned char Timer0Flag = 0;
int sec1;
int sec1flag;
int sec5;
int sec5flag;
void IVTHigh(void);
void ISRHigh(void);

void main(void)
{
    unsigned int i = 0;

    LATCbits.LATC2 = 0;
    TRISCbits.TRISC2 = 0;
    LATDbits.LATD0 = 0;
    TRISDbits.TRISD0 = 0;
    LATDbits.LATD7 = 0;
    TRISDbits.TRISD7 = 0;

    T2CONbits.TOUTPS = 0x00; // 1:1
    T2CONbits.T2CKPS = 0x02; // 1:16
    T2CONbits.TMR2ON = 1; // Timer2 Turn On
    PR2 = TMR2Value;

    T0CONbits.T08BIT = 0; // 16Bits Mode
    T0CONbits.T0CS = 0; // Internal Clock
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0; // Enable Prescale
    T0CONbits.T0PS = 0x07; // 1:256
    TMR0H = TMR0HighValue;
    TMR0L = TMR0LowValue;
    T0CONbits.TMR0ON = 1; // Timer0 Turn On

    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;

    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    while (1)
    {
//----- A -----
        if (sec1==3) 
        {
            sec1=0;
              LATDbits.LATD7 = !LATDbits.LATD7;
        }
 //-------------------B ------------       
             if (sec5==2) 
        {
            sec5=0;
              LATDbits.LATD0 = !LATDbits.LATD0;
        } 
// ---------------------------time -------
        if (Timer0Flag)
        {
            Timer0Flag = 0;
            if (++sec1flag>100) 
            {
                sec1flag=0;
                sec1++;
            }
            if (++sec5flag>500) 
            {
                sec5flag=0;
                sec5++;
            }
          
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

//void interrupt low_priority LowISR(void)     // 低優先權中斷函數 
//{ 
//    if (TMR1IF && TMR1IE) 
//    { 
//        WriteTimer1(65536-7812);    // 250mS Period, 250mS/4uS/8 = 7812 
//        LATDbits.LATD7 = !LATDbits.LATD7; 
//        TMR1IF = 0; 
//    } 
//}
