#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define P1  RB0   	//??????P1?RB0
#define P2  RB1   	//??????P2?RB1
#define P3  RB2   	//??????P3?RB2
#define P4  RB3   	//??????P4?RB3
#define P5  RB4   	//??????P5?RB4
#define P6  RB5   	//??????P6?RB5
#define P7  RB6   	//??????P7?RB6
#define P8  RB7   	//??????P8?RB7
#define relay RC0       //???

unsigned char keycon;
bit keybit;


void main(void)
{
	OSCCON=0X00;    		//??????
	ADCON1=0X0f;            //??IO?????????????
	TRISB=0B11111111;   	//RB?????  
	TRISC=0B11111110;   	//RC?????	 	  			 
  	while(1)
  	{    	
     	if(PORTB==0XFF)    
		{
			if(++keycon>200) //???keybit??????????????????????????????
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
				if(P1==0)  //???P1?????????
				{
					relay=!relay;  //?????????????????
				}
			}
		}
  	}
}
