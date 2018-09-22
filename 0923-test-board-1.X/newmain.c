#include <p18cxxx.h>
#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

const unsigned char liushui[9]=
{
0B11111111,
0B11111110,
0B11111101,
0B11111011,
0B11110111,
0B11101111,
0B11011111,
0B10111111,
0B01111111,
};
//---------------------------------------
//名稱: 主函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//----------------------------------------- 
void main(void)
{	
	unsigned int n;
	unsigned char m=0;	
        unsigned char Q =0;
        unsigned char h=0;	
  	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數位口，類比量輸入禁止
  	TRISD= 0B00000000;   	//RD設置為輸出
	PORTD=0B00000000;       //滅掉V0-V7
        TRISB= 0B00000000;   	//RD設置為輸出
	PORTB=0B00000000;       //滅掉V0-V7
        while(1)
  	{
            switch(Q)
            {
                case 0:
                    for(n=0;n<40000;n++);   		
                    if(++m>7){m=0;Q=1;}		
                    PORTD=liushui[m];  //根据步骤参数查表格，将查出的参数放入临时缓冲寄存器中
                    PORTB=0B11111111;
                    break;
                case 1:
                    for(n=0;n<40000;n++);   		
                    if(++m>7){m=8;Q=2;}	
                    PORTB=liushui[m];  //根据步骤参数查表格，将查出的参数放入临时缓冲寄存器中
                    PORTD=0B11111111;
                    break;
                case 2:
                    for(n=0;n<40000;n++);   		
                    if(--m<1){m=8;Q=3;}	
                    PORTB=liushui[m];  //根据步骤参数查表格，将查出的参数放入临时缓冲寄存器中
                    PORTD=0B11111111;
                    break;
                case 3:
                    for(n=0;n<40000;n++);   		
                    if(--m<1){m=0;Q=0;}			
                    PORTD=liushui[m];  //根据步骤参数查表格，将查出的参数放入临时缓冲寄存器中
                    PORTB=0B11111111;
            }
         }
}

