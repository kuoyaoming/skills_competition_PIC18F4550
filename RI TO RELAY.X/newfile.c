#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define relay RC0     //繼電器

unsigned char tmr_times;
unsigned char code_data;
unsigned char code;
unsigned char int_times;
unsigned char buf1;
bit head_ok;  	
//---------------------------------------
//名稱: T0初始化函數		 
//作者：cby
//公司：寧波芯動電子有限公司
//網址：www.MovingChip.com
//日期：20110601
//-----------------------------------------
void init_timer0(void)
{
	TMR0IF=0;    //T0中斷標誌位清零
    TMR0IE=1;    //T0中斷允許
    T0CON=0xC8;  //8bit定時方式，預分頻關閉，	
	TMR0L=0x13;  //晶振4.0M,定時250us
    GIE=1;   	 //開總中斷
}

void wrong(void)  //如果有錯誤，將相應寄存器清零，準備下一次接收
{
  	int_times=0;
  	tmr_times=0;
  	head_ok=0;
  	code_data=0;  	
}

unsigned char one_zero(void)
{  	
	//參看遙控器芯片HT6221的說明書，0編碼的脈衝週期為1.12ms
	//本例中，時間中斷為0.25ms,所以我們判定取大於0.75ms小於1.5ms的脈衝均為0編碼
  	if((tmr_times>3)&(tmr_times<6))
  	{
    	return 0;
  	}
	//參看遙控器芯片HT6221的說明書，1編碼的脈衝週期為2.24ms
	//本例中，時間中斷為0.25ms,所以我們判定取大於1.75ms小於2.75ms的脈衝均為0編碼
  	else if((tmr_times>7)&(tmr_times<11))
  	{
    	return 1;
  	}
	//如果脈衝週期不在0.75-1.5ms或者1.75-2.75ms區間的，我們認為是錯碼，返回2
  	return 2;
}

void ccp2_int(void)
{
	CCP2CON=0x04;			//捕捉模式，捕捉每個下降沿
	T1CON=0x00;				//CCP要使用捕捉特性，T1必須運行在定時工作或者同步計數器模式下
	TRISC1=1;               //RC1設置為輸入狀態，該引腳接到了紅外接收頭的數據引腳上
	CCP2IF=0;               //捕捉中斷標誌位清零
	CCP2IE=1;				//該捕捉允許中斷
	TMR1ON=1;	            //T1啟動
}

void interrupt ISR(void)    //中斷處理函數
{	
		
  	if(TMR0IF==1)           //如果是T0中斷標誌位為1，即250US時間到並引發中斷
  	{
		TMR0L=0x13;  //晶振4.0M,定時250us
  		TMR0IF=0;           //清除TO中斷標誌位  		
  		tmr_times++;	    //記錄中斷的次數，依據此來判斷接收編碼的狀態
  	}
	 if(CCP2IF==1)          //外部中斷標誌位如果為1，即為CCP2捕捉中斷，轉入處理程序
	 {
		CCP2IF=0;            //清除CCP2捕捉中斷標誌位      
		GIE=0; 				 //全局中斷關掉，以避免解碼過程進入其他中斷	 
  		int_times++;  		 //捕捉中斷計次	
  		if(head_ok==0)      	//如果還沒接收到頭碼，那麼先判斷脈衝編碼是否為頭碼
  		{
			//參看遙控器芯片HT6221的說明書，發射代碼的頭碼由一個9ms的起始碼和一個4.5ms的結果碼組成
			//本例中，時間中斷為0.25ms,所以我們判定大於12.5ms小於14.5ms的脈衝均為13.5ms的頭碼編碼
    		if(tmr_times>45&tmr_times<58)		
    		{
      			head_ok=1;  	//有效頭碼標誌位置一，以示頭碼接收完成
      			int_times=0;    //外部中斷計次寄存器清零，以準備接收後面的4組共32個脈衝
      			tmr_times=0;    //定時器中斷計次寄存器清零，準備開始對後面的脈衝計時	       
    		}
    		else
    		{
      			wrong();        //如果不是有效的頭碼，轉入錯誤處理程序
    		}
  		}
  		else                	//如果已經接收到頭碼，那麼開始對後面的32個脈衝進行解碼                    
  		{    
    		code_data=code_data>>1; //將接收緩衝區右移一位，以準備接收下一位
			buf1=one_zero();    		//判斷編碼是0或1，或者是錯碼
			tmr_times=0;  		    //時間中斷計次寄存器清零			
    		if(buf1==1)			//如果是編碼1
    		{
      			code_data|=0x80;  	//將1移入code_data的最高位
    		}    	
			else if(buf1==0)			//如果是編碼0
	        {
      			code_data&=0x7f;  	//將0移入code_data的最高位
    		}    
			else            		//編碼非0非1，即為錯碼，
            {
				wrong();       		//調用錯誤處理程序
				return;
			}    						
    		if(int_times==8)        //如果收到了8個編碼脈衝，意味著低8位地址碼已經接收完
    		{
				//本遙控器的低8位地址碼為0X00，這個是遙控器廠家決定的，如果要訂做，可以隨便改
        		if(code_data!=0x00)  //如果不是0x00 ，說明不是我們提供的遙控器，我們不予識別
        		{
        			wrong();         //調用錯誤處理程序      
					return;  
        		}    		
    		}
    		else if(int_times==16)        //如果收到了16個編碼脈衝，意味著高8位地址碼已經接收完
    		{
				//本遙控器的低高8位地址碼為0XFF
        		if(code_data!=0xFF)  //如果不是0xff ，說明不是我們提供的遙控器，我們不予識別
        		{
            		wrong();         //調用錯誤處理程序      
					return;
        		}	        			
    		}
    		else if(int_times==24)    //收到的第三組8位字節即為8位數據碼
    		{
      			code=code_data;    //將這個8位數據碼暫存至code_data1  			
    		}
    		else if(int_times==32)    //收到的第四組8位字節即為8位數據碼的反碼
    		{
      			int_times=0;      	  //清零中斷次數寄存器，以便下次從頭開始		
      			head_ok=0;            //將頭碼標誌位清零，以便下次再接收時，從頭碼開始			 			  
      			if((code+code_data)==0xff)    //判斷第三組數據和第四組數據是否互為反碼  			
      			{          			
            		if(code==0x15)            //如果接收到的為'+'鍵的碼值0X15
					{
						relay=1;              //"+"鍵定為吸合繼電器
					}
					if(code==0x07)            //如果接收到的為'-'鍵的碼值0X07	
					{
						relay=0;              //"-"鍵定義為釋放繼電器
					}
            	}      			
    		}
  		}   		
  		  		
  	} 
  	GIE=1;   	//開放全局中斷
}

main(void)
{
  	unsigned char buf;
	OSCCON=0X00;    		//選擇主震盪器
	ADCON1=0X0f;            //所有IO均為數字口，模擬量輸入禁止
	TRISC=0B11111110;   	//Rc0設置為輸出，其他設置為輸出入
	relay=0;                //繼電器默認為釋放狀態
  	init_timer0();          //TO初始化函數
  	ccp2_int();	            //CCP2初始化函數  	
  	while(1)
  	{ 
    		
  	}
}

