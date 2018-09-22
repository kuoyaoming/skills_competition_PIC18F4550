#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define U6U7_A RA0    	//定義U6、U7的A腳
#define U6U7_B RA1    	//定義U6、U7的B腳
#define U6U7_C RA2    	//定義U6、U7的C腳
#define U7_E RA3    	//定義U6的E3腳
#define U6_E RA4    	//定義U7的E3腳

unsigned char m; 

const unsigned char han[]=
{
/*--  文字:  國  --*/
/*--  Trebuchet MS12;  此字體下對應的點陣為：寬x高=16x16   --*/
0x00,0xFE,0x02,0x0A,0x8A,0x8A,0x8A,0xFA,0x8A,0x8A,0x8A,0x0A,0x02,0xFE,0x00,0x00,
0x00,0xFF,0x40,0x48,0x48,0x48,0x48,0x4F,0x48,0x49,0x4E,0x48,0x40,0xFF,0x00,0x00,
};


//---------------------------------------
//名稱: 寫一個漢字到LCD函數
//參數：x,y：寫入漢字的地址 address: 寫入漢字在han表格中的位置
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//----------------------------------------- 		
void disp16_16(void) 
{  	
	if(++m>31) m=0;	
	PORTD=0;
	PORTB=0;
	PORTA=0;		
	if(m<16)	PORTB=(han[m]); 
	else  PORTD=(han[m]);
    PORTA|=(m&0x07);	
	if((m&0x08)>0) U7_E=1;
	else U6_E=1;
} 


void main(void)
{	
	unsigned char k;
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
  	TRISB=0B00000000;   	//RB設置為輸出
	PORTB=0B00000000;       //
	TRISD=0B00000000;   	//RD設置為輸出
	PORTD=0B00000000;       //滅掉LED點陣屏的00-07行
	TRISA=0B00000000;   	//RA設置為輸出
	PORTA=0B00000000;       //滅掉LED點陣屏的08-15行	
	USBEN=0;
	UTRDIS=1;
	while(1)
  	{ 				   				 
		for(k=0;k<70;k++);
  		disp16_16(); 				      	
  	}
}

