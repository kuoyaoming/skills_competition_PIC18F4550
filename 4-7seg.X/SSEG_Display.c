//**********************************************************
//*		SSEG_Display.c
//*
//*		設定為 HS 16MHz
//*
//*
//*
//*
//*
//*
//**********************************************************
#include	<xc.h>		//納入微控制器定義檔
#include	<delays.h>	//納入時間延遲函式庫定義檔
#include	<adc.h>		//納入類比訊號轉換函式庫定義檔
#include	<pwm.h>		//納入PWM函式庫定義檔
#include	<stdio.h>
#include	<stdlib.h>
#include	<timers.h>	//納入計時器函式庫定義檔
#include	"config.h"      //設定微處理器相關結構位元

#define _XTAL_FREQ 16000000

#define SSEG_SCAN0_LAT	LATB0
#define SSEG_SCAN1_LAT	LATB1
#define SSEG_SCAN2_LAT	LATB2
#define SSEG_SCAN3_LAT	LATB3
#define SSEG_SCAN0_TRIS	TRISB0
#define SSEG_SCAN1_TRIS	TRISB1
#define SSEG_SCAN2_TRIS	TRISB2
#define SSEG_SCAN3_TRIS	TRISB3

#define SSEG_A_LAT	LATD0
#define SSEG_B_LAT	LATD1
#define SSEG_C_LAT	LATD2
#define SSEG_D_LAT	LATD3
#define SSEG_E_LAT	LATD4
#define SSEG_F_LAT	LATD5
#define SSEG_G_LAT	LATD6
#define SSEG_DP_LAT	LATD7
#define SSEG_A_TRIS	TRISD0
#define SSEG_B_TRIS	TRISD1
#define SSEG_C_TRIS	TRISD2
#define SSEG_D_TRIS	TRISD3
#define SSEG_E_TRIS	TRISD4
#define SSEG_F_TRIS	TRISD5
#define SSEG_G_TRIS	TRISD6
#define SSEG_DP_TRIS	TRISD7
//============================================================================

//============================================================================
unsigned char reverse_DATA(unsigned char a)
{
	unsigned char b;
	for(unsigned char i=0;i<8;i++)
	{
		if(((a>>i) & 0x01) == 0x01)
		b = (b<<1) +1;
		else b = (b<<1);
	}	
	return b;
}
//----------------------------------------------------------------------------
void delay_ms(int a)	//  延遲1ms副程式
{
	int i;
	for(i=0;i<a;i++) __delay_us(996);
}
//============================================================================
void SSEG_OUT(unsigned char a, unsigned char dp)
{
unsigned char const SSEG_TABLE[] = {	0xc0,0xf9,0xa4,0xb0,	//0123
					0x99,0x92,0x82,0xd8,	//4567
					0x80,0x90,0x88,0x83,	//89Ab
					0xc6,0xa1,0x86,0x8e,	//CdEF
					0xbf,0xff};		//-
	if(a >= 17) a = 17;
	unsigned char b = SSEG_TABLE[a];
	
	SSEG_A_TRIS = 1; SSEG_B_TRIS = 1; SSEG_C_TRIS = 1; SSEG_D_TRIS = 1;
	SSEG_E_TRIS = 1; SSEG_F_TRIS = 1; SSEG_G_TRIS = 1; SSEG_DP_TRIS = 1;
	
	SSEG_A_LAT = b & 0x01;
	SSEG_B_LAT = (b >> 1) & 0x01;
	SSEG_C_LAT = (b >> 2) & 0x01;
	SSEG_D_LAT = (b >> 3) & 0x01;
	SSEG_E_LAT = (b >> 4) & 0x01;
	SSEG_F_LAT = (b >> 5) & 0x01;
	SSEG_G_LAT = (b >> 6) & 0x01;
	if(dp==0) SSEG_DP_LAT = 0;
	else SSEG_DP_LAT = 1;
	
	SSEG_A_TRIS = 0; SSEG_B_TRIS = 0; SSEG_C_TRIS = 0; SSEG_D_TRIS = 0;
	SSEG_E_TRIS = 0; SSEG_F_TRIS = 0; SSEG_G_TRIS = 0; SSEG_DP_TRIS = 0;
}
//----------------------------------------------------------------------------
void SCAN_OUT(unsigned char a)
{
	SSEG_SCAN0_TRIS = 1; SSEG_SCAN1_TRIS = 1; SSEG_SCAN2_TRIS = 1; SSEG_SCAN3_TRIS = 1;
	switch(a)
	{
		case 0:	SSEG_SCAN0_LAT = 0;
			SSEG_SCAN1_LAT = 1;
			SSEG_SCAN2_LAT = 1;
			SSEG_SCAN3_LAT = 1;
			break;
		case 1:	SSEG_SCAN0_LAT = 1;
			SSEG_SCAN1_LAT = 0;
			SSEG_SCAN2_LAT = 1;
			SSEG_SCAN3_LAT = 1;
			break;
		case 2:	SSEG_SCAN0_LAT = 1;
			SSEG_SCAN1_LAT = 1;
			SSEG_SCAN2_LAT = 0;
			SSEG_SCAN3_LAT = 1;
			break;
		case 3:	SSEG_SCAN0_LAT = 1;
			SSEG_SCAN1_LAT = 1;
			SSEG_SCAN2_LAT = 1;
			SSEG_SCAN3_LAT = 0;
			break;
		default:SSEG_SCAN0_LAT = 1;
			SSEG_SCAN1_LAT = 1;
			SSEG_SCAN2_LAT = 1;
			SSEG_SCAN3_LAT = 1;
	}
	SSEG_SCAN0_TRIS = 0; SSEG_SCAN1_TRIS = 0; SSEG_SCAN2_TRIS = 0; SSEG_SCAN3_TRIS = 0;
}
//============================================================================
void main (void)
{
	unsigned char DELAY_COUNT,SCAN_COUNT;
	unsigned char N1,N10,N100,N1000;
	
	ADCON1 = 0B00001111;	// A/D端口設為Digital I/O
	CMCON = 0B00000111 ;	// 比較器關閉
	
	DELAY_COUNT = 0;
	SCAN_COUNT = 0;
	
	N1 = 0;
	N10 = 0;
	N100 = 0;
	N1000 = 0;

	delay_ms(1);

	while(1)
	{
		if(DELAY_COUNT++ >= 10)		//Speed Control
		{
			DELAY_COUNT = 0;
			if(N1 >= 9)
			{
				N1 = 0;
				if(N10 >= 9)
				{
					N10 = 0;
					if(N100 >= 9)
					{
						N100 = 0;
						if(N1000 >= 9) N1000 = 0;
						else N1000++;
					}
					else N100++;
				}
				else N10++;
			}
			else N1++;
		}
		delay_ms(1);
		
		SCAN_OUT(0xff);
		switch(SCAN_COUNT)
		{
			case 0:	SSEG_OUT(N1,1);
				break;
			case 1:	SSEG_OUT(N10,1);
				break;
			case 2:	SSEG_OUT(N100,0);
				break;
			case 3:	SSEG_OUT(N1000,1);
				break;
			default:SSEG_OUT(0xff,1);
		}
		SCAN_OUT(SCAN_COUNT);
		SCAN_COUNT++;
		if(SCAN_COUNT >= 4) SCAN_COUNT = 0;
	}
}
//============================================================================