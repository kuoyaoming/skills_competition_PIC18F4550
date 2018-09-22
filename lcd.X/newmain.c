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
		for(j=0;j<10;j++);		
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
   	lcd_write_data(buf);		//?慈胂允臼?   
} 


void main(void)
{	  
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止	
  	TRISB=0B11000111;   	//RB3-5設置為輸出
	TRISD=0B00000000;   	//RD設置為輸出  		   
	lcdreset();	 								//復位1602					
   	lcd_write_char(0,0,0x41);   //A
   	lcd_write_char(1,0,0x42);   //B
   	lcd_write_char(2,0,0x43);   //C
   	lcd_write_char(3,0,0x44);   //D
   	lcd_write_char(4,0,0x45);   //E
   	lcd_write_char(5,0,0x46);   //F
   	lcd_write_char(6,0,0x47);   //G
   	lcd_write_char(7,0,0x48);   //H
   	lcd_write_char(8,0,0x49);   //I
   	lcd_write_char(9,0,0x4A);   //J
   	lcd_write_char(10,0,0x4B);   //K
   	lcd_write_char(11,0,0x4C);   //L
   	lcd_write_char(12,0,0x4D);   //M
   	lcd_write_char(13,0,0x4E);   //N
   	lcd_write_char(14,0,0x4F);   //O
   	lcd_write_char(15,0,0x50);   //P
   	
   	lcd_write_char(0,1,0x61);   //a
   	lcd_write_char(1,1,0x62);   //b
   	lcd_write_char(2,1,0x63);   //c
   	lcd_write_char(3,1,0x64);   //d
   	lcd_write_char(4,1,0x65);   //e
   	lcd_write_char(5,1,0x66);   //f
   	lcd_write_char(6,1,0x67);   //g
   	lcd_write_char(7,1,0x68);   //h
   	lcd_write_char(8,1,0x69);   //i
   	lcd_write_char(9,1,0x6A);   //j
   	lcd_write_char(10,1,0x6B);   //k
   	lcd_write_char(11,1,0x6C);   //l
   	lcd_write_char(12,1,0x6D);   //m
   	lcd_write_char(13,1,0x6E);   //n
   	lcd_write_char(14,1,0x6F);   //o
   	lcd_write_char(15,1,0x70);   //p
	while(1)
	{		
			
	}  
}
