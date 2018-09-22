//**********************************************************
//*		KB4x5_LCM1602.c
//*
//*		設定為 HS 16MHz
//*
//*
//*
//*
//*
//*
//**********************************************************
//#include	<xc.h>		//納入微控制器定義檔
//#include	<delays.h>	//納入時間延遲函式庫定義檔
//#include	<adc.h>		//納入類比訊號轉換函式庫定義檔
//#include	<pwm.h>		//納入PWM函式庫定義檔
//#include	<stdio.h>
//#include	<stdlib.h>
//#include	<timers.h>	//納入計時器函式庫定義檔
//#include	"config.h"      //設定微處理器相關結構位元
#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

#define _XTAL_FREQ 16000000

#define	SCAN_LAT	LATB
#define	SCAN_PORT	PORTB
#define	SCAN_TRIS	TRISB
#define	ROW_LAT	LATD
#define	ROW_PORT	PORTD
#define	ROW_TRIS	TRISD
//============================================================================
unsigned char const TABLE[88] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x81,0xff,0x91,0x91,0x91,0x6e,0x00,
				0x00,0x81,0xff,0x91,0x91,0xb9,0xc3,0x00,
				0x10,0x10,0x10,0x00,
				0x00,0x81,0xff,0x91,0x90,0xb8,0xc0,0x00,
				0x81,0xff,0x91,0x98,0x94,0x63,0x01,0x00,
				0x00,0x81,0xff,0x81,0x00,
				0x00,0x81,0xff,0x91,0x91,0xb9,0xc3,0x00,
				0x81,0xff,0x61,0x18,0x86,0xff,0x80,0x00,
				0x81,0xff,0x81,0x81,0x81,0x42,0x3c,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

unsigned char const SCAN_TABLE[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
//============================================================================
unsigned char reverse_DATA(unsigned char a)
{
	unsigned char b = 0;
	for(unsigned char i=0;i<8;i++)
	{
		if(((a>>i) & 0x01) == 0x01)
		b = (b<<1) +1;
		else b = (b<<1);
	}	
	return b;
}

void delay_ms(int a)       //  延遲1ms副程式
{
	int i;
	for(i=0;i<a;i++) __delay_us(996);
}

void main (void)
{
	unsigned char DELAY_COUNT,INDEX,SCAN_COUNT;
	
	ADCON1 = 0B00001111;	// A/D端口設為Digital I/O
	CMCON = 0B00000111 ;	// 比較器關閉
	ROW_LAT = 0xff;		//
	ROW_TRIS = 0; 		// 將ROW_TRIS設為0，ROW設定為輸出
	SCAN_LAT = 0xff;	//
	SCAN_TRIS = 0; 		// 將SCAN_TRIS設為0，SCAN設定為輸出
	
	DELAY_COUNT = 0;
	INDEX = 0;
	SCAN_COUNT = 0;
	
	delay_ms(100);

	while(1)
	{
		SCAN_LAT = 0xff;
		ROW_LAT = reverse_DATA(~TABLE[SCAN_COUNT+INDEX]);
		//ROW_LAT = TABLE[SCAN_COUNT+INDEX];
		SCAN_LAT = SCAN_TABLE[SCAN_COUNT];
		SCAN_COUNT++;
		if(SCAN_COUNT >= 8) SCAN_COUNT = 0;
		
		if(DELAY_COUNT++ >= 50)		//Speed Control
		{
			DELAY_COUNT = 0;
			if(INDEX++ >= 79)
			{
				INDEX = 0;
			}
		}
		delay_ms(1);
	}
}
