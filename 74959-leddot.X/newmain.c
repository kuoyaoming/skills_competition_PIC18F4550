#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define srclk RA3
#define rclk  RA2
#define ser   RA5

#define U6U7_A RB0    	//定義U6、U7的A腳
#define U6U7_B RB1    	//定義U6、U7的B腳
#define U6U7_C RB2    	//定義U6、U7的C腳
#define U7_E RB3    	//定義U6的E3腳
#define U6_E RB4    	//定義U7的E3腳

unsigned char m;
unsigned char u6data;
unsigned char u7data; 

const unsigned char han[]=
{
/*--  文字:  國  --*/
/*--  Trebuchet MS12;  此字體下對應的點陣為：寬x高=16x16   --*/
0x00,0xFE,0x02,0x0A,0x8A,0x8A,0x8A,0xFA,0x8A,0x8A,0x8A,0x0A,0x02,0xFE,0x00,0x00,
0x00,0xFF,0x40,0x48,0x48,0x48,0x48,0x4F,0x48,0x49,0x4E,0x48,0x40,0xFF,0x00,0x00,
};

void write595(unsigned char U6buf,unsigned char U7buf)
{
	unsigned char i;
	rclk=0;		
	for(i=0;i<8;i++)
	{ 
		if(U6buf&0x80)
			ser=1;
		else
			ser=0;
		U6buf=U6buf<<1;		
		srclk=0; 	
		asm("CLRWDT");
		asm("CLRWDT");	
		srclk=1; 
	}
	for(i=0;i<8;i++)
	{ 
		if(U7buf&0x80)
			ser=1;
		else
			ser=0;
		U7buf=U7buf<<1;		
		srclk=0; 	
		asm("CLRWDT");
		asm("CLRWDT");	
		srclk=1; 
	}
	rclk=1;
	asm("NOP");
  	asm("NOP");
  	rclk=0;
}

void disp16_16(void) 
{  	
	if(++m>31) m=0;	
	write595(0,0);
	PORTB=0;		
	if(m<16)	u7data=(han[m]); 
	else  u6data=(han[m]);
	write595(u6data,u7data);
    PORTB|=(m&0x07);	
	if((m&0x08)>0) U7_E=1;
	else U6_E=1;	
} 


void main(void)
{	
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
  	TRISB=0B11100000;   	//RB0-4設置為輸出
	PORTB=0B00000000;       //失能U6、U7
	TRISA=0B00000000;   	//RA設置為輸出
	PORTA=0B00000000;       //
	while(1)
  	{ 				   				 
  		disp16_16(); 				      	
  	}
}



