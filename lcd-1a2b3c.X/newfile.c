#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define RSPIN  RB5 //Data or Instrument Select
#define RWPIN  RB4 //Write or Read
#define EPIN  RB3 //6800 mode Enable single



void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++)
		{
			asm("CLRWDT"); 
		}
	}
}


void lcd_wait_busy(void)
{  
	TRISD7=1;	   			//為讀狀態做準備
   	RSPIN=0;			   //選擇指令寄存器
   	RWPIN=1;			   //選擇讀
   	EPIN=1;				   //使能線電平變化
   	while(RD7==1);         //讀忙狀態，不忙時退出
   	EPIN=0;				   //恢復使能線電平
   	TRISD7=0;
}

void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//選擇指令寄存器
   	RWPIN=0;           	//選擇寫
   	PORTD=combuf;         	//把命令字送入RD 	
   	EPIN=1;	            //使能線電平變化，命令送入1602的8位數據口
   	asm("NOP");
   	EPIN=0;	            //恢復使能線電平
}

void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();				//調用忙檢測函數
   	lcd_write_com(combuf); 	//調用寫命令函數
}

void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//調用忙檢測函數
   	RSPIN=1;          	//選擇數據寄存器
   	RWPIN=0;           	//選擇寫
   	PORTD=databuf;      	//把數據字送入P2
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
		lcd_write_address(x,y); //寫入地址			
   	lcd_write_data(buf);		//寫入顯示數據    
} 

//---------------------------------------
//名稱: 主函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//----------------------------------------- 
void main(void)
{	  	
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
  	TRISB=0B11000111;   	//RB3-5設置為輸出
	TRISD=0B00000000;   	//RD設置為輸出  		   
	lcdreset();	 			//復位1602							
   	lcd_write_char(4,0,'K');   //G
   	lcd_write_char(5,0,'U');   //L
   	lcd_write_char(6,0,'O');   //7
   	lcd_write_char(7,0,'Y');   //-
   	lcd_write_char(8,0,'A');   //4
   	lcd_write_char(9,0,'O');   //5
   	lcd_write_char(10,0,'M');  //5
	lcd_write_char(11,0,'I');  //0
   	lcd_write_char(12,0,'N');  //0
         lcd_write_char(13,0,'G');  //0
         
   	lcd_write_char(1,1,'2');   //M
   	lcd_write_char(2,1,'0');   //o
   	lcd_write_char(3,1,'1');   //v
   	lcd_write_char(4,1,'5');   //i
   	lcd_write_char(5,1,'0');   //n
   	lcd_write_char(6,1,'8');   //g
   	lcd_write_char(7,1,'2');   //C
   	lcd_write_char(8,1,'5');   //h
   	
	
	while(1)
	{		
	
	}  
}

