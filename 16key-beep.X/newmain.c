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

#define relay RC0       //繼電器

unsigned char keycon;
bit keybit;


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
	TRISC=0B11111110;   	//RC0設置為輸入出
  	TRISB=0B11110000;   	//RB0-3設置為輸出,RB4-7設置為輸入   			 
  	while(1)
  	{
 		keybuf=key();      //調用按鍵掃描函數以確定是否有按鍵按下，以及按下鍵的鍵值
     	if(keybuf==0)
		{
			if(++keycon>200) //消抖，keybit同時也起到微分的作用，即按下一次鍵無論多長時間，都只處理一次
			{
				keycon=0;
				keybit=1;    
			}
		}
		else
		{
			if(keybit==1)
			{
				keybit=0;
				if(keybuf==1)  //如果是K1鍵按下，則進行處理
				{
					relay=!relay;  //繼電器端口取反，以使繼電器改變狀態
				}
			}
		}		  	    	
  	}
}
