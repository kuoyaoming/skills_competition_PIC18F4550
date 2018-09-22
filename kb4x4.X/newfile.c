#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define _XTAL_FREQ 16000000

#define LCM_DB70_LAT	LATD
#define LCM_DB70_PORT	PORTD
#define LCM_DB70_TRIS	TRISD

#define LCM_RS_LAT	LATB0
#define LCM_RS_TRIS	TRISB0

#define LCM_RW_LAT	LATB1
#define LCM_RW_TRIS	TRISB1

#define LCM_E_LAT	LATB2
#define LCM_E_TRIS	TRISB2

#define KB_C0_PORT	RA0
#define KB_C0_TRIS	TRISA0
#define KB_C1_PORT	RA1
#define KB_C1_TRIS	TRISA1
#define KB_C2_PORT	RA2
#define KB_C2_TRIS	TRISA2
#define KB_C3_PORT	RA3
#define KB_C3_TRIS	TRISA3

#define KB_R0_LAT	LATB3
#define KB_R0_TRIS	TRISB3
#define KB_R1_LAT	LATB4
#define KB_R1_TRIS	TRISB4
#define KB_R2_LAT	LATB5
#define KB_R2_TRIS	TRISB5
#define KB_R3_LAT	LATB6
#define KB_R3_TRIS	TRISB6
#define KB_R4_LAT	LATB7
#define KB_R4_TRIS	TRISB7
//============================================================================
unsigned char mes[] = {"HelloWorld"};

unsigned char const IdxCol[]= {
	0 , 1 , 0 , 2 ,
	0 , 1 , 0 , 3 ,
	0 , 1 , 0 , 2 ,
	0 , 1 , 0 , 4 };

unsigned char const *keycode[]= {
	"0","1","2","3" ,
	"4","5","6","7" ,
	"8","9","A","B" ,
	"C","D","E","F" ,
	"F1","F2","F3","CR" };
//============================================================================
void delay_ms(unsigned int a)       //  延遲 ms副程式
{
	for(unsigned int i=0;i<a;i++) __delay_us(996);
}
//============================================================================
void waitlcd()
{
	unsigned char status;
	LCM_DB70_TRIS = 0xff;	//Set LCM_DB70 As Inputs
	LCM_DB70_PORT = 0xff;
	__delay_us(1);
	do{
		LCM_RS_LAT = 0;	//RS=0 ;RW=1; E=1
		LCM_RW_LAT = 1;
		LCM_E_LAT = 1;
		__delay_us(1);
		status= LCM_DB70_PORT;
		LCM_E_LAT = 0;		//E=0
	}
	while(status&0x80) ; // wait until BF=0
}
//----------------------------------------------------------------------------
void Command(unsigned char cmd){
	LCM_DB70_LAT = cmd;
	LCM_DB70_TRIS = 0x00;	//Set LCM_DB70 As Outputs
	__delay_us(1);
	
	LCM_RS_LAT = 0;	//RS=0 ;RW=0; E=1;E=0 ; RW=1
	LCM_RW_LAT = 0;
	LCM_E_LAT = 1;
	__delay_us(1);
	LCM_E_LAT = 0;
	LCM_RW_LAT = 1;
	waitlcd() ;
}
void Data(unsigned char data1){
	LCM_DB70_LAT = data1;
	LCM_DB70_TRIS = 0x00;	//Set LCM_DB70 As Outputs
	__delay_us(1);
	
	LCM_RS_LAT = 1;	//RS=1 ;RW=0; E=1;E=0 ;RW=1
	LCM_RW_LAT = 0;
	LCM_E_LAT = 1;
	__delay_us(1);
	LCM_E_LAT = 0;
	LCM_RW_LAT = 1;
 	waitlcd() ;
}
//============================================================================
void KB_ROW_WR(unsigned char a)
{
	KB_R0_LAT = a & 0x01;
	KB_R1_LAT = (a>>1) & 0x01;
	KB_R2_LAT = (a>>2) & 0x01;
	KB_R3_LAT = (a>>3) & 0x01;
	KB_R4_LAT = (a>>4) & 0x01;
	
	KB_R0_TRIS = 0;
	KB_R1_TRIS = 0;
	KB_R2_TRIS = 0;
	KB_R3_TRIS = 0;
	KB_R4_TRIS = 0;
}
//----------------------------------------------------------------------------
unsigned char KB_COL_RD(void)
{
	unsigned char a = 0;
	
	KB_C0_TRIS = 1;
	KB_C1_TRIS = 1;
	KB_C2_TRIS = 1;
	KB_C3_TRIS = 1;
	
	a = (a<<1) | (KB_C3_PORT & 0x01);
	a = (a<<1) | (KB_C2_PORT & 0x01);
	a = (a<<1) | (KB_C1_PORT & 0x01);
	a = (a<<1) | (KB_C0_PORT & 0x01);
	
	return a;
}
//============================================================================
void main (void)
{
	ADCON1 = 0B00001111;	// A/D端口設為Digital I/O
	CMCON = 0B00000111 ;	// 比較器關閉
	
	LCM_RS_LAT = 0;
	LCM_RW_LAT = 0;
	LCM_E_LAT = 0;
	LCM_RS_TRIS = 0;
	LCM_RW_TRIS = 0;
	LCM_E_TRIS = 0;
	
	LCM_DB70_LAT = 0x00;
	LCM_DB70_TRIS = 0x00;
	
	delay_ms(15);
	LCM_RS_LAT = 0;
	LCM_RW_LAT = 0;
	LCM_DB70_LAT = 0x38;
	LCM_E_LAT = 1;
	__delay_us(1);
	LCM_E_LAT = 0;

	delay_ms(5);
	LCM_RS_LAT = 0;
	LCM_RW_LAT = 0;
	LCM_DB70_LAT = 0x38;
	LCM_E_LAT = 1;
	__delay_us(1);
	LCM_E_LAT = 0;
	
	__delay_us(100);
	LCM_RS_LAT = 0;
	LCM_RW_LAT = 0;
	LCM_DB70_LAT = 0x38;
	LCM_E_LAT = 1;
	__delay_us(1);
	LCM_E_LAT = 0;
	
	Command(0x38) ;    // 採用8位元資料存取/雙列字/5*8點矩陣字型
	Command(0xe) ;     //顯示器ON/游標被顯示在位址計數器
	Command(0x1) ;     // 清除全螢幕 
	Command(0x3) ;     //
	delay_ms(2);
 	Command(0x80) ;    // 設定DD RAM位址為00H
	
	unsigned char *pp = mes;
	for(; *pp !=0 ;){
	    Data(*pp++);
	}
	
 	Command(0xc0) ;    // 設定DD RAM位址為40H
	
	while(1)
	{
		unsigned char row ;
		unsigned char keyflag ;
		unsigned char tROW ;
		unsigned char key ;
		unsigned char const *ptr ;
		do{
			for(KB_ROW_WR(0xfe) ,tROW=0xfe ,row=0 ,keyflag=0 ;(row<5);row++){
				if(KB_COL_RD() != 0xf){
					keyflag=1 ;
					break ;
				}
				tROW <<=1 ;
				tROW |=0x1 ;
				KB_ROW_WR(tROW) ;
			}
			if(keyflag== 1){
				delay_ms(20) ;
				if(KB_COL_RD() ==0xf){
					keyflag= 0 ;
				}
			}
		}
		while(keyflag==0) ;
		key=row*4+IdxCol[KB_COL_RD()];
		do{
			while((KB_COL_RD()) != 0xf) ;
			delay_ms(20) ;
		}
		while(KB_COL_RD() != 0xf) ;
		ptr= keycode[key] ;
		while(*ptr !=0){
			Data(*ptr++);
		}
	}
}
