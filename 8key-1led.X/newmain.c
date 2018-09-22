#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define P1  RB0   	//隅砱黃蕾偌瑩P1峈RB0
#define P2  RB1   	//隅砱黃蕾偌瑩P2峈RB1
#define P3  RB2   	//隅砱黃蕾偌瑩P3峈RB2
#define P4  RB3   	//隅砱黃蕾偌瑩P4峈RB3
#define P5  RB4   	//隅砱黃蕾偌瑩P5峈RB4
#define P6  RB5   	//隅砱黃蕾偌瑩P6峈RB5
#define P7  RB6   	//隅砱黃蕾偌瑩P7峈RB6
#define P8  RB7   	//隅砱黃蕾偌瑩P8峈RB7

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
//---------------------------------------
//靡備: 翋滲杅		 
//釬氪ㄩcby
//鼠侗ㄩ譴疏郋雄萇赽衄癹鼠侗
//厙硊ㄩwww.MovingChip.com
//ㄩ20110601
//----------------------------------------- 
void main(void)
{
	OSCCON=0X00;    		//恁寁翋涾絕
	ADCON1=0X0f;            //垀衄IO歙峈杅趼諳ㄛ耀攜講怀輦砦
	TRISD=0B00000000;   	//RD扢离峈怀堤  
	TRISB=0B11111111;   	//RB扢离峈怀	 	
  	while(1)
  	{    	
     	if(P1==0) PORTD=LED[1]; //彆P1瑩偌狟ㄛ1弇杅鎢奪奻珆尨1  
		else if(P2==0) PORTD=LED[2]; //彆P2瑩偌狟ㄛ1弇杅鎢奪奻珆尨2   
		else if(P3==0) PORTD=LED[3]; //彆P3瑩偌狟ㄛ1弇杅鎢奪奻珆尨3
		else if(P4==0) PORTD=LED[4]; //彆P4瑩偌狟ㄛ1弇杅鎢奪奻珆尨4
		else if(P5==0) PORTD=LED[5]; //彆P5瑩偌狟ㄛ1弇杅鎢奪奻珆尨5
		else if(P6==0) PORTD=LED[6]; //彆P6瑩偌狟ㄛ1弇杅鎢奪奻珆尨6
		else if(P7==0) PORTD=LED[7]; //彆P7瑩偌狟ㄛ1弇杅鎢奪奻珆尨7 	
		else if(P8==0) PORTD=LED[8]; //彆P8瑩偌狟ㄛ1弇杅鎢奪奻珆尨8
		else PORTD=0;  		         //拸偌瑩偌狟ㄛ壽珆尨 
  	}
}
