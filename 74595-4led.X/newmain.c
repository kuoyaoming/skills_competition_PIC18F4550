#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define srclk RA3
#define rclk  RA2
#define ser   RA5
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
	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned int d=0;
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止	
	TRISA=0B00000000;   	//RA設置為輸出 
	write595(0x00,0x00);		
	while(1)
  	{ 		
		if(++a>250)
		{
			a=0;	
			if(++c>200)
			{
				c=0;
				if(++d>9999) d=0;
			}
			write595(0x00,0x00);            //關顯示
			if(++b>3) b=0; 	    //顯示位置輪流，需要嚴格限制範圍為0-3			
			switch(b)
			{
				case 0:				  //千位顯示內容
					write595(LED[d/1000],b);  	    	//								
				break;
				case 1:				  //百位顯示內容
					write595(LED[(d%1000)/100],b);     	//					
				break;
				case 2:				  //十位顯示內容
					write595(LED[(d%100)/10],b);  	   	//					
				break;
				case 3:				  //個位顯示內容
					write595(LED[d%10],b);  	    	//				
				break;			    
				default:
				break;
			}		
		}  			      	
  	}
}
