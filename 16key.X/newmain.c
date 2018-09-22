#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define keyO1  RB0   	//定義鍵盤接口
#define keyO2  RB1   	//定義鍵盤接口
#define keyO3  RB2   	//定義鍵盤接口
#define keyO4  RB3   	//定義鍵盤接口
#define keyI1  RB4   	//定義鍵盤接口
#define keyI2  RB5   	//定義鍵盤接口
#define keyI3  RB6   	//定義鍵盤接口
#define keyI4  RB7   	//定義鍵盤接口

const unsigned char LED[16]=
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
0x77,	  //A
0x7C,	  //B
0x39,	  //C
0x5E,	  //D
0x79,	  //E
0x71,	  //F
};

unsigned char key(void)
{
	keyO1=0;    		//第1行寫為0，是為了掃描第1行是否有鍵按下
	keyO2=1;    		//第2行寫為1
	keyO3=1;    		//第3行寫為1
	keyO4=1;    		//第4行寫為1	
	asm("NOP");	    	//略作延時
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)    	//如果第1列讀出為0，說明K1鍵被按下，返回1
	{		
		return 1;				
	}
	else if(keyI2==0)   //如果第2列讀出為0，說明K2鍵被按下，返回2
	{
		return 2;
	}
	else if(keyI3==0)   //如果第3列讀出為0，說明K3鍵被按下，返回3
	{
		return 3;
	}
	else if(keyI4==0)   //如果第4列讀出為0，說明K4鍵被按下，返回4
	{
		return 4;
	}
	keyO1=1;    		//第1行寫為1
	keyO2=0;    		//第2行寫為0，是為了掃描第2行是否有鍵按下
	keyO3=1;    		//第3行寫為1
	keyO4=1;    		//第4行寫為1	
	asm("NOP");		   	//略作延時
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)    	//如果第1列讀出為0，說明K5鍵被按下，返回5
	{		
		return 5;				
	}
	else if(keyI2==0)   //如果第2列讀出為0，說明K6鍵被按下，返回6
	{
		return 6;
	}
	else if(keyI3==0)   //如果第3列讀出為0，說明K7鍵被按下，返回7
	{
		return 7;
	}	
	else if(keyI4==0)   //如果第4列讀出為0，說明K8鍵被按下，返回8
	{
		return 8;
	}
	keyO1=1;    		//第1行寫為1
	keyO2=1;    		//第2行寫為1
	keyO3=0;    		//第3行寫為0，是為了掃描第3行是否有鍵按下
	keyO4=1;    		//第4行寫為1	
	asm("NOP");		   	//略作延時	
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)    	//如果第1列讀出為0，說明K9鍵被按下，返回9
	{		
		return 9;				
	}
	else if(keyI2==0)   //如果第2列讀出為0，說明K10鍵被按下，返回10
	{
		return 10;
	}
	else if(keyI3==0)   //如果第3列讀出為0，說明K11鍵被按下，返回11
	{
		return 11;
	}	
	else if(keyI4==0)   //如果第4列讀出為0，說明K12鍵被按下，返回12
	{
		return 12;
	}
	keyO1=1;    		//第1行寫為1
	keyO2=1;    		//第2行寫為1
	keyO3=1;    		//第3行寫為1
	keyO4=0;    		//第4行寫為0，是為了掃描第4行是否有鍵按下	
	asm("NOP");		   	//略作延時	
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)    	//如果第1列讀出為0，說明K13鍵被按下，返回13
	{		
		return 13;				
	}
	else if(keyI2==0)   //如果第2列讀出為0，說明K14鍵被按下，返回14
	{
		return 14;
	}
	else if(keyI3==0)   //如果第3列讀出為0，說明K15鍵被按下，返回15
	{
		return 15;
	}	
	else if(keyI4==0)   //如果第4列讀出為0，說明K16鍵被按下，返回16
	{
		return 16;
	}
	return 0;           //如果沒有鍵被按下，返回0
}
void main(void)
{
	unsigned char keybuf;
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
	TRISD=0B00000000;   	//RD設置為輸出  
	TRISB=0B11110000;   	//RB0-3設置為輸出,RB4-7設置為輸入  			 
  	while(1)
  	{    	
     	keybuf=key();      //調用按鍵掃描函數以確定是否有按鍵按下，以及按下鍵的鍵值
		if(keybuf==1) PORTD=LED[0]; //如果k1鍵按下，1位數碼管上顯示0  
		else if(keybuf==2) PORTD=LED[1]; //如果k2鍵按下，1位數碼管上顯示1   
		else if(keybuf==3) PORTD=LED[2]; //如果k3鍵按下，1位數碼管上顯示2
		else if(keybuf==4) PORTD=LED[3]; //如果k4鍵按下，1位數碼管上顯示3
		else if(keybuf==5) PORTD=LED[4]; //如果k5鍵按下，1位數碼管上顯示4
		else if(keybuf==6) PORTD=LED[5]; //如果k6鍵按下，1位數碼管上顯示5
		else if(keybuf==7) PORTD=LED[6]; //如果k7鍵按下，1位數碼管上顯示6 	
		else if(keybuf==8) PORTD=LED[7]; //如果k8鍵按下，1位數碼管上顯示7
		else if(keybuf==9) PORTD=LED[8]; //如果k9鍵按下，1位數碼管上顯示8   
		else if(keybuf==10) PORTD=LED[9]; //如果k10鍵按下，1位數碼管上顯示9
		else if(keybuf==11) PORTD=LED[10]; //如果k11鍵按下，1位數碼管上顯示A
		else if(keybuf==12) PORTD=LED[11]; //如果k12鍵按下，1位數碼管上顯示B
		else if(keybuf==13) PORTD=LED[12]; //如果k13鍵按下，1位數碼管上顯示C
		else if(keybuf==14) PORTD=LED[13]; //如果k14鍵按下，1位數碼管上顯示D 	
		else if(keybuf==15) PORTD=LED[14]; //如果k15鍵按下，1位數碼管上顯示E
		else if(keybuf==16) PORTD=LED[15]; //如果k16鍵按下，1位數碼管上顯示F
		else PORTD=0;  		         	   //無按鍵按下，關顯示     		  	    	
  	}
}
