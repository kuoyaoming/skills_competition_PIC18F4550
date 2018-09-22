#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define bujin_io1   RA2 //
#define bujin_io2   RA3 //
#define bujin_io3   RA4 //
#define bujin_io4   RA5 //

unsigned int  bujincon1;
unsigned char bujincon2;
const unsigned char bujin_table[]={
0B00000001,
0B00000011,
0B00000010,
0B00000110,
0B00000100,
0B00001100,
0B00001000,
0B00001001,
};


void main(void)
{	
	unsigned char buf;
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
        TRISA=0B11000011;   	    //RA設置為輸出
	PORTA=0B00000000;           //
  	while(1)
  	{
    	if(++bujincon1>70)
    	{
    		 bujincon1=0;
    		 bujincon2++;
    		 bujincon2&=0x07;	
    		 buf=bujin_table[bujincon2];
    		 if(buf&0x01) bujin_io1=1;
    		 else bujin_io1=0;
    		 if(buf&0x02) bujin_io2=1;
    		 else bujin_io2=0;
    		 if(buf&0x04) bujin_io3=1;
    		 else bujin_io3=0;
    		 if(buf&0x08) bujin_io4=1;
    		 else bujin_io4=0;		
    	}
  	}
}

