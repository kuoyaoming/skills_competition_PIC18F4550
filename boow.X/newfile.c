#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define RSPIN  RB5 //Data or Instrument Select
#define RWPIN  RB4 //Write or Read
#define EPIN  RB3 //6800 mode Enable single
#define beep RC2

unsigned int a=0;
unsigned int b=59;//預設秒
unsigned int c=59;//預設分
unsigned int d=23;//預設時
unsigned int e=3600;
bit beepon=0;
int beepcont=0;

void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++);
	}
}

void lcd_wait_busy(void)
{  
	TRISD7=1;	   		   //為讀狀態做準備，把RD7設為輸入
   	RSPIN=0;			   //選擇指令寄存器
   	RWPIN=1;			   //選擇讀
   	EPIN=1;				   //使能線電平變化
   	while(RD7==1);         //讀忙狀態，不忙時退出
   	EPIN=0;				   //恢復使能線電平
   	TRISD7=0;			   //把RD7設置為輸出
}

void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//選擇指令寄存器
   	RWPIN=0;           	//選擇寫
   	PORTD=combuf;       //把命令字送入RD 	
   	EPIN=1;	            //使能線電平變化，命令送入1602的8位數據口
   	asm("NOP");         //來一個空操作，以延時片刻
   	EPIN=0;	            //恢復使能線電平
}

void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//調用忙檢測函數
   	lcd_write_com(combuf); 	//調用寫命令函數
}

void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//調用忙檢測函數
   	RSPIN=1;          	//選擇數據寄存器
   	RWPIN=0;           	//選擇寫
   	PORTD=databuf;      //把數據字送入RD口
   	EPIN=1;	            //使能線電平變化，命令送入1602的8位數據口
   	asm("NOP");
   	EPIN=0;	            //恢復使能線電平
}

void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//列地址限制在0-15
   	y&=0x01;			//行地址限制在0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //第一行的列地址寫入
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //第二行的列地址寫入
}

void lcdreset(void)
{  
	delay(150); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	lcd_write_com_busy(0x38);       //8位數據，雙列，5*7字形
   	lcd_write_com_busy(0x08);		//顯示功能關，無光標
   	lcd_write_com_busy(0x01);		//清屏指令
   	lcd_write_com_busy(0x06);		//寫入新的數據後，光標右移，顯示屏不移動
   	lcd_write_com_busy(0x0c);		//顯示功能開，無光標，
}  	

void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//寫入地址			
   	lcd_write_data(buf);		//寫入顯示數據    
} 

void timer1init(void)
{
	T1CON=0X01;	
	TMR1IF=0;                //清除TMR1中斷標誌
	TMR1IE=1;                //TMR1中斷允許	
	TMR1H=0xFF;             //賦初值，以便TMR1每250US中斷一次
	TMR1L=0x13;
	PIR1=0;
	PEIE=1;
	GIE=1;
}

void interrupt  T1(void)
{
    if(TMR1IF==1)
    {
        TMR1H=0XFF;
        TMR1L=0x13;		
        TMR1IF=0;
        if(++a>4000)
        {
            a=0;
            b=--b;
        }        
    }   
}

void main(void)
{	
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
	TRISB=0B11000111;   	//RB3-5設置為輸出
  	TRISD=0B00000000;   	//RD設置為輸出  	  	  	
  	TRISC=0B11111011;
         lcdreset();	 			//復位1602	
	timer1init();  	  
  	if(b<1&&c>0)
        {
            b=59;
            c=--c;
            if(c<1&&d>0)
            {
                c=59;
                d=--d;
                if(d<0)
                d=0;
                               
            }
        }
//        if(b>30&&a%2000>1)beep=0;
//        else if(b>20&&a%1320>1)beep=0;
//        else if(b>10&&a%1000>1)beep=0;
//        else if(b>5&&a%800>1)beep=0;
//        else if(b>1)beep=0;
//        else beep =0;
        while(1)
  	{ 		
            lcd_write_char(0,0,0x0a);
            lcd_write_char(4,0,'M');
            lcd_write_char(5,0,'y');
            lcd_write_char(7,0,'C');
            lcd_write_char(8,0,'l');
            lcd_write_char(9,0,'o');
            lcd_write_char(10,0,'c');
            lcd_write_char(11,0,'k');
            lcd_write_char(15,0,0x0a);
            lcd_write_char(0,1,0x0a);
            lcd_write_char(4,1,(d%60)/10+0x30);	
            lcd_write_char(5,1,(d%10)+0x30);
            lcd_write_char(6,1,0x3a);
            lcd_write_char(7,1,(c%60)/10+0x30);	
            lcd_write_char(8,1,(c%10)+0x30); 
            lcd_write_char(9,1,0x3a);
            lcd_write_char(10,1,(b%60)/10+0x30);
            lcd_write_char(11,1,(b%10)+0x30);
            lcd_write_char(15,1,0x0a);
  	}
}

