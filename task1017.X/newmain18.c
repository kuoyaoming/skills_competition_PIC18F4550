#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include <xc.h>
#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

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

#define IO12_LAT	LATB4
#define IO12_TRIS	TRISB4
#define IO13_LAT	LATB5
#define IO13_TRIS	TRISB5

#define S1_3_PORT	PORTE
#define S1_3_TRIS	TRISE
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
	SSEG_SCAN0_LAT = 1; SSEG_SCAN1_LAT = 1; SSEG_SCAN2_LAT = 1; SSEG_SCAN3_LAT = 1;
	switch(a)
	{
		case 0:	SSEG_SCAN0_LAT = 0;
			break;
		case 1:	SSEG_SCAN1_LAT = 0;
			break;
		case 2:	SSEG_SCAN2_LAT = 0;
			break;
		case 3:	SSEG_SCAN3_LAT = 0;
			break;
		default:break;
	}
	SSEG_SCAN0_TRIS = 0; SSEG_SCAN1_TRIS = 0; SSEG_SCAN2_TRIS = 0; SSEG_SCAN3_TRIS = 0;
}
//============================================================================
void main (void)
{
	unsigned char DELAY_COUNT,SCAN_COUNT;
	
	unsigned int ADC_10B = 0;
	unsigned char N1 = 0,N10 = 0,N100 = 0,N1000 = 0;
	
	IO12_LAT = 0;
	IO13_LAT = 0;
	IO12_TRIS = 0;
	IO13_TRIS = 0;
	
	ADCON0=0x01;		// 選擇AN0通道轉換，開啟ADC模組
	ADCON1=0x0e;		// 使用VDD，VSS為參考電壓，設定AN0為類比輸入
	ADCON2=0xb9;		// 結果向右靠齊並設定轉換時間為Fosc/8，採樣時間為20TAD
	CMCON = 0B00000111 ;	// 比較器關閉
	
	DELAY_COUNT = 0;
	SCAN_COUNT = 0;
	
	delay_ms(1);
	
	S1_3_TRIS |= 0x07;	//SET S1_3_PORT As Inputs
	
	while(1)
	{
	
	switch((S1_3_PORT & 0x07))
	{
	case 0x02:	IO12_LAT = 1;
			IO13_LAT = 0;
			break;
	case 0x04:	IO12_LAT = 0;
			IO13_LAT = 1;
			break;
	default:	IO12_LAT = 0;
			IO13_LAT = 0;
	}
	
	Delay10TCYx(5);	//時間延遲以完成採樣
	ConvertADC() ;		//進行訊號轉換
	while(BusyADC());	//等待轉換完成
	DELAY_COUNT++;
	if(DELAY_COUNT < 64)
	{
	ADC_10B += ADRESL;
	ADC_10B += ((0x0000+ADRESH) << 8);
	}
	else
	{
	ADC_10B = ADC_10B / 64;
	N1000 = ADC_10B/1000;
	N100 = (ADC_10B/100)%10;
	N10 = (ADC_10B%100)/10;
	N1 = ADC_10B%10;
	ADC_10B = 0;
	DELAY_COUNT = 0;
	}
	
	switch((S1_3_PORT & 0x07))
	{
	case 0x01:	SCAN_OUT(0xff);
			switch(SCAN_COUNT)
			{
			case 0:	SSEG_OUT(0x00,0);
				break;
			case 1:	SSEG_OUT(0x01,0);
				break;
			case 2:	SSEG_OUT(0x02,0);
				break;
			case 3:	SSEG_OUT(0x03,0);
				break;
			default:SSEG_OUT(0xff,1);
			}
			SCAN_OUT(SCAN_COUNT);
			delay_ms(245);
			SCAN_COUNT++;
			if(SCAN_COUNT >= 4) SCAN_COUNT = 0;
			break;
			
	case 0x02:	SCAN_OUT(0xff);
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
			delay_ms(1);
			break;
			
	case 0x04:	SCAN_OUT(0xff);
			switch(SCAN_COUNT)
			{
			case 0:	SSEG_OUT(N1,1);
				break;
			case 1:	SSEG_OUT(N10,1);
				break;
			case 2:	SSEG_OUT(N100,1);
				break;
			case 3:	SSEG_OUT(N1000,0);
				break;
			default:SSEG_OUT(0xff,1);
			}
			SCAN_OUT(SCAN_COUNT);
			SCAN_COUNT++;
			if(SCAN_COUNT >= 4) SCAN_COUNT = 0;
			delay_ms(1);
			break;
			
	default:	SCAN_OUT(0xff);
			SSEG_OUT(0xff,1);
			SCAN_COUNT = 0;
	}
	}
}
//============================================================================