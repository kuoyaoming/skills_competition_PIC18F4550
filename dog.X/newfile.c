#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=ON, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define RSPIN  RB5 //Data or Instrument Select
#define RWPIN  RB4 //Write or Read
#define EPIN  RB3 //6800 mode Enable single

unsigned int a;
unsigned char b;

void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++) asm("CLRWDT");
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
   	EPIN=1;	            //使能線電平變化，命令送入1602的8位元資料口
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
   	RSPIN=1;          	//選擇資料寄存器
   	RWPIN=0;           	//選擇寫
   	PORTD=databuf;      //把資料字送入RD口
   	EPIN=1;	            //使能線電平變化，命令送入1602的8位元資料口
   	asm("NOP");
   	EPIN=0;	            //恢復使能線電平
}

void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//列地址限制在0-15
   	y&=0x01;			//行地址限制在0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //第一行的列位址寫入
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //第二行的列位址寫入
}

void lcdreset(void)
{  
	delay(150); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	lcd_write_com_busy(0x38);       //8位元數據，雙列，5*7字形
   	lcd_write_com_busy(0x08);		//顯示功能關，無游標
   	lcd_write_com_busy(0x01);		//清屏指令
   	lcd_write_com_busy(0x06);		//寫入新的資料後，游標右移，顯示幕不移動
   	lcd_write_com_busy(0x0c);		//顯示功能開，無游標，
}  	

void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//寫入位址			
   	lcd_write_data(buf);		//寫入顯示資料    
} 

void main(void)
{
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數位口，類比量輸入禁止
	TRISB=0B11000111;   		//RB3-5設置為輸出
	TRISD=0B00000000;   		//RD設置為輸出	
	lcdreset();	 				//復位1602	 
	lcd_write_char(0,0,'B');   //在座標(0,0)處寫入‘B’
	lcd_write_char(1,0,'o');   //在座標(1,0)處寫入‘o’
	lcd_write_char(2,0,'o');   //在座標(2,0)處寫入‘o’
	lcd_write_char(3,0,'t');   //在座標(3,0)處寫入‘t’
	lcd_write_char(4,0,'i');   //在座標(4,0)處寫入‘i’
	lcd_write_char(5,0,'n');   //在座標(5,0)處寫入‘n’
	lcd_write_char(6,0,'g');   //在座標(6,0)處寫入‘g’
	lcd_write_char(7,0,'.');   //在座標(7,0)處寫入‘.’
	lcd_write_char(8,0,'.');   //在座標(8,0)處寫入‘.’
	lcd_write_char(9,0,'.');   //在座標(9,0)處寫入‘.’
	lcd_write_char(10,0,'.');  //在座標(10,0)處寫入‘.’	
	lcd_write_char(11,0,'.');  //在座標(11,0)處寫入‘.’	
	for(a=0;a<50;a++) delay(300);	//延時幾秒，非精確延時
	lcdreset();	 				//復位1602
  	while(1)
  	{
 //   	asm("CLRWDT"); 
     	if(++a>50000)
		{
			a=0;			
			if(++b>10) 
			{
				b=0;
                lcd_write_char(0,0,0x30+b/10); 			//顯示資料十位元
			lcd_write_char(1,0,0x30+b%10);  	    //顯示資料個位		
				while(1);
			}
			lcd_write_char(0,0,0x30+b/10); 			//顯示資料十位元
			lcd_write_char(1,0,0x30+b%10);  	    //顯示資料個位			
		}    		  	    	
  	}
}


