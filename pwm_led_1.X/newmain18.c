/*
 * File:   newmain18.c
 * Author: kuoyaoming
 *
 * Created on 2015年9月30日, 下午 3:05
 */
#include <xc.h>
#include <p18cxxx.h>
#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

//pwm===========================================================================
#define _XTAL_FREQ 16000000
#define TMR2PRESCALE 4
long freq;

int PWM_Max_Duty()
{
  return(_XTAL_FREQ/(freq*TMR2PRESCALE));
}

PWM1_Init(long fre)
{
  PR2 = (_XTAL_FREQ/(fre*4*TMR2PRESCALE)) - 1;
  freq = fre;
}

PWM2_Init(long fre)
{
  PR2 = (_XTAL_FREQ/(fre*4*TMR2PRESCALE)) - 1;
  freq = fre;
}

PWM1_Duty(unsigned int duty)
{
  if(duty<1024)
  {
    duty = ((float)duty/1023)*PWM_Max_Duty();
      DC1B1 = duty & 2;
      DC1B0 = duty & 1;
    
    CCPR1L = duty>>2;
  }
}

PWM2_Duty(unsigned int duty)
{
  if(duty<1024)
  {
    duty = ((float)duty/1023)*PWM_Max_Duty();
    DC2B1 = duty & 2;
    DC2B0 = duty & 1;
    CCPR2L = duty>>2;
  }
}

PWM1_Start()
{
  CCP1M3 = 1;
  CCP1M2 = 1;
  #if TMR2PRESCALAR == 1
    T2CKPS0 = 0;
    T2CKPS1 = 0;
  #elif TMR2PRESCALAR == 4
    T2CKPS0 = 1;
    T2CKPS1 = 0;
  #elif TMR2PRESCALAR == 16
    T2CKPS0 = 1;
    T2CKPS1 = 1;
  #endif
  TMR2ON = 1;
  TRISC2 = 0;
}

PWM1_Stop()
{
  CCP1M3 = 0;
  CCP1M2 = 0;
}

PWM2_Start()
{
  CCP2M3 = 1;
  CCP2M2 = 1;
  #if TMR2PRESCALE == 1
    T2CKPS0 = 0;
    T2CKPS1 = 0;
  #elif TMR2PRESCALE == 4
    T2CKPS0 = 1;
    T2CKPS1 = 0;
  #elif TMR2PRESCALE == 16
    T2CKPS0 = 1;
    T2CKPS1 = 1;
  #endif
    TMR2ON = 1;
    TRISC1 = 0;
}

PWM2_Stop()
{
  CCP2M3 = 0;
  CCP2M2 = 0;
}
//timer=========================================================================
#define InstructionFrequency 16000000 / 4

#define T2Peroid 1000
#define TMR2Value ( InstructionFrequency / 16 / T2Peroid )

#define T0Peroid 1
#define TMR0Value ( 65536 - ( InstructionFrequency / 256 / T0Peroid ))
#define TMR0HighValue ( ( TMR0Value >> 8 ) & 0xff )
#define TMR0LowValue ( TMR0Value & 0xff )

volatile unsigned char Timer0Flag = 0;
//main==========================================================================
void main()
{
    //timer=====================================================================
    unsigned int i = 0;

    LATCbits.LATC1 = 0;
    TRISCbits.TRISC1 = 0;
    LATCbits.LATC2 = 0;
    TRISCbits.TRISC2 = 0;
    
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
    //pwm=======================================================================
    unsigned int f=0,j=0,ff=1,jj=10;
    PWM1_Init(5000);
    PWM2_Init(5000);
    TRISD = 0xFF;
    TRISB = 0;
    PWM1_Duty(0);
    PWM2_Duty(0);
    PWM1_Start();
    PWM2_Start();
    
    while (1)
    {
        if (PIR1bits.TMR2IF)
        {
            PIR1bits.TMR2IF = 0;
            if (f < 0 || f > 1024)ff = -ff ;
            f=f+ ff;
            PWM1_Duty(f);
            
        }

        if (Timer0Flag)
        {
            Timer0Flag = 0;
            if (j < 0 || j > 1024)jj = -jj ;
            j=j+ jj;
            PWM2_Duty(j);
        }
    }
}

void interrupt HighISR(void) // 高優先權中斷函數
{
    if (TMR0IE && TMR0IF)
    {
        WRITETIMER0(65536-781); // 500mS Period, 500mS/4uS/256 = 488
        TMR0IF=0;
        Timer0Flag = 1;
    }
}
void interrupt low_priority LowISR(void) // 低優先權中斷函數
{
if (TMR1IF && TMR1IE)
{
WRITETIMER1(65536-7812); // 250mS Period, 250mS/4uS/8 = 7812
//LATDbits.LATD7 = !LATDbits.LATD7;
TMR1IF = 0;
}
}
