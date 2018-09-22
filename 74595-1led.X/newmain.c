#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define srclk RA3
#define rclk  RA2
#define ser   RA5
//7  6  5  4  3  2  1  0
//v7 v6 v5 v4 v3 v2 v1 v0  
const unsigned char liushui[64]=
{
0B00000001,  //0
0B00000010,
0B00000100,
0B00001000,
0B00010000,
0B00100000,
0B01000000,
0B10000000,
0B11000000,
0B11100000,
0B11110000,   //10
0B11111000,
0B11111100,
0B11111110,
0B11111111,
0B11111110,
0B11111100,
0B11111000,
0B11110000,
0B11100000,
0B11000000,   //20
0B10000000,
0B10000000,
0B01000000,
0B00100000,
0B00010000,
0B00001000,
0B00000100,
0B00000010,
0B00000001,
0B00000010,  //30
0B00000100,
0B00001000,
0B00010000,
0B00100000,
0B01000000,
0B10000000,
0B01000000,
0B00100000,
0B00010000,
0B00001000,  //40
0B00000100,
0B00000010,
0B00000001,
0B00000011,
0B00000110,
0B00001100,
0B00011000,
0B00110000,
0B01100000,
0B11000000,  //50
0B00000000,
0B00000001,
0B00000101,
0B00010101,
0B01010101,  
0B01010111,
0B01011111,
0B01111111,
0B11111111,
0B00000000,  //60
0B10101010,
0B01010101,
0B00000000,  //63
};
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

void main(void)
{	
	unsigned int a;
	unsigned char b,c;
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
	TRISA=0B00000000;   	//RA設置為輸出 		
	write595(0x00,0x00);
	a=b=c=0;
  	while(1)
  	{
    	if(++a>10000)    //延時10000個軟件週期，非精確定時
   		{
   			a=0;        //清零以備下次重新計數
			if(++b>63)  //步驟參數加1，因為我們定義的表格長度為64，所以這裡需要限制步驟參數在0-63間
			{
				b=0;
				if(++c>9) c=0;  //流水一個循環，數碼管顯示加一，因為是1位數碼管，我們把它限制在0-9
			}		
			write595(liushui[b],LED[c]);  //把流水燈及1位數碼管顯示的數據通過SPI接口寫到74HC595中，在08-15輸出口中輸出流水，00-07輸出11數碼管	
   		}  
  	}
}
