#include <pic18.h>        //調用頭文件，可以去PICC18軟件下去查找PIC18F4550.H
__CONFIG(1,HS&CPUDIV1) ;  //晶振為外部4M,不分頻
__CONFIG(2,WDTDIS) ;      //看門狗關閉
__CONFIG(3,PBADDIS) ;     //復位後PORTB<4:0>配置為數字I/O
__CONFIG(4,XINSTDIS) ;    //禁止使用指令集擴展和變址尋址模式

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

unsigned int ycon;
unsigned char a;

//---------------------------------------
//名稱: T1初始化函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//----------------------------------------- 
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
//---------------------------------------
//名稱: T1中斷函數（250US）		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//----------------------------------------- 
void interrupt  T1(void)
{
	if(TMR1IF==1)           //250us
  {
  		TMR1H=0XFF;
		TMR1L=0x13;		
		TMR1IF=0;
		if(++ycon>3999)  //計數4000次後，為1秒
		{
			ycon=0;
			if(++a>9) a=0;   //0-9間的變化
  			PORTD=LED[a];  	//將查表得出的數據送顯示				
		}
	}
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
  	TRISD=0B00000000;   	    //RD設置為輸出
	PORTD=0B00000000;       //滅掉LED_a-LED_dp 
  	timer1init();	    	  	
  	while(1)
  	{ 		
    		
  	}
}

