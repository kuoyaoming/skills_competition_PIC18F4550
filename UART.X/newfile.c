#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define RSPIN  RB5 //Data or Instrument Select
#define RWPIN  RB4 //Write or Read
#define EPIN  RB3 //6800 mode Enable single

bit clrbit;
unsigned char recebuf;


//---------------------------------------
//名稱: 延時函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601 
//-----------------------------------------
void delay1(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++);
	}
}

//---------------------------------------
//名稱: 1602忙檢測函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//----------------------------------------- 
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
//---------------------------------------
//名稱: 1602寫命令函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//選擇指令寄存器
   	RWPIN=0;           	//選擇寫
   	PORTD=combuf;       //把命令字送入RD 	
   	EPIN=1;	            //使能線電平變化，命令送入1602的8位數據口
   	asm("NOP");         //來一個空操作，以延時片刻
   	EPIN=0;	            //恢復使能線電平
}
//---------------------------------------
//名稱: 1602寫命令函數（帶忙檢測）		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//調用忙檢測函數
   	lcd_write_com(combuf); 	//調用寫命令函數
}
//---------------------------------------
//名稱: 1602寫數據函數（帶忙檢測）		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
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
//---------------------------------------
//名稱: 1602顯示地址寫函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//列地址限制在0-15
   	y&=0x01;			//行地址限制在0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //第一行的列地址寫入
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //第二行的列地址寫入
}
//---------------------------------------
//名稱: 1602初始化函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
void lcdreset(void)
{  
	delay1(150); 
	lcd_write_com(0x38);
   	delay1(50); 
	lcd_write_com(0x38);
   	delay1(50); 
	lcd_write_com(0x38);
   	lcd_write_com_busy(0x38);       //8位數據，雙列，5*7字形
   	lcd_write_com_busy(0x08);		//顯示功能關，無光標
   	lcd_write_com_busy(0x01);		//清屏指令
   	lcd_write_com_busy(0x06);		//寫入新的數據後，光標右移，顯示屏不移動
   	lcd_write_com_busy(0x0c);		//顯示功能開，無光標，
}  	
//---------------------------------------
//名稱: 指定地址寫入函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//寫入地址			
   	lcd_write_data(buf);		//寫入顯示數據    
}  
//---------------------------------------
//名稱: 串口初始化函數
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
void sci_Init(void)
{
	BRGH = 1;	// high baud rate 
	SPBRG =25;  // 設置波特率9600 
	SYNC = 0;	// 1=同步接收方式 0=異步接收方式 
	SPEN = 1;	// 串口使能位 
	CREN = 1;	// 1=允許連續接收 0=禁止連續接收 
	SREN = 0;	// 異步方式：此位未用 
	TXIE = 0;	// 關閉發送 
	RCIE = 0;	// 關閉接收中斷 
	TX9  = 0;	// 1：選擇9位接收 0：選擇8位接收 
	RX9  = 0;	// 1：選擇9位接收 0：選擇8位接收 
	TXEN = 1;	// 發送允許 
	TRISC6=1;   // 作為串口使用，RC6必須設置為輸入
	TRISC7=1;   // 作為串口使用，RC7必須設置為輸入	
}
//---------------------------------------
//名稱: 串口發送函數
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
void send232byte(unsigned char bytebuf)
{
	TXREG=bytebuf;
	while(TRMT==0);
		
}
//---------------------------------------
//名稱: 串口接收函數
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
unsigned char WaitComm(void)
{   	
	if(RCIF==1)           //如果接收標誌位為1，即有內容需要接收
    {        		
		recebuf=RCREG;	  //將緩衝區內容接收下來	
	  	if(OERR)	      //如果有溢出錯誤
		{
			CREN = 0;     //清零CREN位可將此位OERR清零
			CREN = 1;		
		}	 
		return 1; 	       //接收完畢後，返回標誌1
	}	
	return 0;              //沒有接收，返回標誌0	
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
	unsigned char buf;
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
  	TRISB=0B11000111;   	//RB3-5設置為輸出
    TRISD=0B00000000;   	//RD設置為輸出   		   
	lcdreset();  
  	sci_Init(); 
  	lcd_write_char(0,0,'R');   	//在坐標(0,0)處寫入R
	lcd_write_char(1,0,'E');   	//在坐標(1,0)處寫入e		
	lcd_write_char(2,0,'C');   	//在坐標(2,0)處寫入c
	lcd_write_char(3,0,'E');   	//在坐標(3,0)處寫入e	
	lcd_write_char(4,0,'I');   	//在坐標(4,0)處寫入i		
	lcd_write_char(5,0,'V');   	//在坐標(5,0)處寫入v
	lcd_write_char(6,0,'E');   	//在坐標(6,0)處寫入e

	lcd_write_char(8,0,'A');   	//在坐標(8,0)處寫入a

	lcd_write_char(10,0,'B');  	//在坐標(10,0)處寫入b
	lcd_write_char(11,0,'Y');  	//在坐標(11,0)處寫入y
	lcd_write_char(12,0,'T');  	//在坐標(12,0)處寫入t
	lcd_write_char(13,0,'E');  	//在坐標(13,0)處寫入e 
	

	lcd_write_char(0,1,'B');   	//在坐標(0,1)處寫入B
	lcd_write_char(1,1,'A');   	//在坐標(1,1)處寫入a		
	lcd_write_char(2,1,'U');   	//在坐標(2,1)處寫入u
	lcd_write_char(3,1,'D');   	//在坐標(3,1)處寫入d	

	lcd_write_char(5,1,'R');   	//在坐標(5,1)處寫入r
	lcd_write_char(6,1,'A');   	//在坐標(6,1)處寫入a
	lcd_write_char(7,1,'T');   	//在坐標(7,1)處寫入t
	lcd_write_char(8,1,'E');   	//在坐標(8,1)處寫入e
	lcd_write_char(9,1,':');   	//在坐標(9,1)處寫入:
	
	lcd_write_char(10,1,'9');   //在坐標(10,1)處寫入9
	lcd_write_char(11,1,'6');   //在坐標(11,1)處寫入6
	lcd_write_char(12,1,'0');   //在坐標(12,1)處寫入0
	lcd_write_char(13,1,'0');   //在坐標(13,1)處寫入0
	clrbit=1;			        //置清屏標誌位,以便在第一個
  	while(1)
  	{
 	
    	if(WaitComm()==1)				//如果有收到上位機數據
		{  						
			if(clrbit)		        	//如果為第一次收到數據啟動清屏操作
			{
		 		clrbit=0;			 		
				lcdreset();         	//清屏幕 
			}							  
			lcd_write_char(0,0,'R');   	//在坐標(0,0)處寫入R
			lcd_write_char(1,0,'E');   	//在坐標(1,0)處寫入e		
			lcd_write_char(2,0,'C');   	//在坐標(2,0)處寫入c
			lcd_write_char(3,0,'E');   	//在坐標(3,0)處寫入e
			lcd_write_char(4,0,':');   	//在坐標(4,0)處寫入:
			lcd_write_char(6,0,'0');   	//在坐標(6,0)處寫入0
			lcd_write_char(7,0,'X');   	//在坐標(7,0)處寫入x
			buf=recebuf>>4;            	//收到的數據高4位放入BUF
			if(buf<10)                 	//因為1602需要送入ASCII碼，需要如下轉換
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(8,0,buf);   	//在坐標(8,0)處寫入該數據
			buf=recebuf&0x0f;          	//收到的數據低4位放入BUF
			if(buf<10)                 	//因為1602需要送入ASCII碼，需要如下轉換
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(9,0,buf);   	//在坐標(9,0)處寫入該數據	

			recebuf+=1;                 //做一次加一動作
			send232byte(recebuf);		//回覆數據為接收數據
		
			lcd_write_char(0,1,'S');   	//在坐標(0,1)處寫入S
			lcd_write_char(1,1,'E');   	//在坐標(1,1)處寫入e		
			lcd_write_char(2,1,'N');   	//在坐標(2,1)處寫入n
			lcd_write_char(3,1,'D');   	//在坐標(3,1)處寫入d
			lcd_write_char(4,1,':');   	//在坐標(4,1)處寫入:
			lcd_write_char(6,1,'0');   	//在坐標(6,1)處寫入0
			lcd_write_char(7,1,'X');   	//在坐標(7,1)處寫入x
			
			buf=recebuf>>4;        		//發送的數據高4位放入BUF
			if(buf<10)                 	//因為1602需要送入ASCII碼，需要如下轉換
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(8,1,buf);   	//在坐標(8,1)處寫入該數據
			buf=recebuf&0x0f;      		//發送的數據低4位放入BUF
			if(buf<10)                 	//因為1602需要送入ASCII碼，需要如下轉換
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(9,1,buf);   	//在坐標(9,1)處寫入該數據				
		}	           	
  	}
}

