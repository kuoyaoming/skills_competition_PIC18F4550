#include <pic18.h>        //調用標頭檔，可以去PICC18軟體下去查找PIC18F4550.H
__CONFIG(1,HS&CPUDIV1) ;  //晶振為外部4M,不分頻
__CONFIG(2,WDTDIS) ;      //看門狗關閉
__CONFIG(3,PBADDIS) ;     //復位後PORTB<4:0>配置為數位I/O
__CONFIG(4,XINSTDIS) ;    //禁止使用指令集擴展和變址定址模式


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
//名稱: 主函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//----------------------------------------- 
void main(void)
{	
	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned int d=0;	
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數位口，類比量輸入禁止
  	TRISB=0B11111000;   	//RB0-3設置為輸出
	TRISD=0B00000000;   	//RD設置為輸出
	PORTD=0B00000000;       //滅掉LED_a-LED_dp	
	while(1)
  	{ 		
		if(++a>250)
		{
			a=0;	
			if(++c>200)
			{
				c=0;
				if(++d>9999) d=0;
			}
			PORTD=0;            //關顯示
			if(++b>3) b=0; 	    //顯示位置輪流，需要嚴格限制範圍為0-3		
			PORTB=b;            //輪流選擇顯示位元
			switch(b)
			{
				case 0:				  //千位元顯示內容
					PORTD=LED[d/1000];  	    	//								
				break;
				case 1:				  //百位元顯示內容
					PORTD=LED[(d%1000)/100];  	    //					
				break;
				case 2:				  //十位元顯示內容
					PORTD=LED[(d%100)/10];  	    //					
				break;
				case 3:				  //個位顯示內容
					PORTD=LED[d%10];  	    		//				
				break;			    
				default:
				break;
			}		
		}  			      	
  	}
}
