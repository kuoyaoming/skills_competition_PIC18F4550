#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON


#define V0 RD0    	//隅砱V0峈RD0傷諳
#define V1 RD1    	//隅砱V1峈RD1傷諳
#define V2 RD2    	//隅砱V2峈RD2傷諳
#define V3 RD3    	//隅砱V3峈RD3傷諳
#define V4 RD4    	//隅砱V4峈RD4傷諳
#define V5 RD5    	//隅砱V5峈RD5傷諳
#define V6 RD6    	//隅砱V6峈RD6傷諳
#define V7 RD7    	//隅砱V7峈RD7傷諳
#define test_value 1
void delay(unsigned int );

void main(void)
{	
	OSCCON=0X00;    		//恁寁翋涾絕
	ADCON1=0X0f;            //垀衄IO歙峈杅趼諳ㄛ耀攜講怀輦砦
  	TRISD=0B00000000;   	//RD扢离峈怀堤
	PORTD=0B00000000;       //鏢裁V0-V7
  	
        
    while(1)
    {
        delay(test_value);
        V0=!V0;
    }
}
void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<120;j++);
	}
}
