#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON


#define _XTAL_FREQ 16000000

#define	SCAN_LAT	LATB
#define	SCAN_PORT	PORTB
#define	SCAN_TRIS	TRISB
#define	ROW_LAT		LATD
#define	ROW_PORT	PORTD
#define	ROW_TRIS	TRISD

#define PAE_LAT_0	LATA0
#define PAE_LAT_1	LATA1
#define PAE_LAT_2	LATA2
#define PAE_LAT_3	LATA3
#define PAE_LAT_4	LATA5
#define PAE_LAT_5	LATE0
#define PAE_LAT_6	LATE1
#define PAE_LAT_7	LATE2
#define PAE_PORT_0	RA0
#define PAE_PORT_1	RA1
#define PAE_PORT_2	RA2
#define PAE_PORT_3	RA3
#define PAE_PORT_4	RA5
#define PAE_PORT_5	RE0
#define PAE_PORT_6	RE1
#define PAE_PORT_7	RE2
#define PAE_TRIS_0	TRISA0
#define PAE_TRIS_1	TRISA1
#define PAE_TRIS_2	TRISA2
#define PAE_TRIS_3	TRISA3
#define PAE_TRIS_4	TRISA5
#define PAE_TRIS_5	TRISE0
#define PAE_TRIS_6	TRISE1
#define PAE_TRIS_7	TRISE2
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
//----------------------------------------------------------------------------
void delay_ms(int a)       //  延遲1ms副程式
{
	int i;
	for(i=0;i<a;i++) __delay_us(996);
}
//============================================================================
unsigned char PAE_RD(void)
{
	unsigned char data;
	PAE_TRIS_7 = 1; PAE_TRIS_6 = 1; PAE_TRIS_5 = 1; PAE_TRIS_4 = 1;
	PAE_TRIS_3 = 1; PAE_TRIS_2 = 1; PAE_TRIS_1 = 1; PAE_TRIS_0 = 1;

	data = (data<<1) | (PAE_PORT_7 & 0x01);
	data = (data<<1) | (PAE_PORT_6 & 0x01);
	data = (data<<1) | (PAE_PORT_5 & 0x01);
	data = (data<<1) | (PAE_PORT_4 & 0x01);
	data = (data<<1) | (PAE_PORT_3 & 0x01);
	data = (data<<1) | (PAE_PORT_2 & 0x01);
	data = (data<<1) | (PAE_PORT_1 & 0x01);
	data = (data<<1) | (PAE_PORT_0 & 0x01);

	return data;
}
//----------------------------------------------------------------------------
void PAE_WR(unsigned char data)
{
	PAE_LAT_0 = data & 0x01;
	PAE_LAT_1 = (data>>1) & 0x01;
	PAE_LAT_2 = (data>>2) & 0x01;
	PAE_LAT_3 = (data>>3) & 0x01;
	PAE_LAT_4 = (data>>4) & 0x01;
	PAE_LAT_5 = (data>>5) & 0x01;
	PAE_LAT_6 = (data>>6) & 0x01;
	PAE_LAT_7 = (data>>7) & 0x01;

	PAE_TRIS_0 = 0; PAE_TRIS_1 = 0; PAE_TRIS_2 = 0; PAE_TRIS_3 = 0;
	PAE_TRIS_4 = 0; PAE_TRIS_5 = 0; PAE_TRIS_6 = 0; PAE_TRIS_7 = 0;
}
//============================================================================
void main (void)
{
	unsigned char COLOR_SELECT,DELAY_COUNT,INDEX,SCAN_COUNT;
	
	ADCON1 = 0B00001111;	// A/D端口設為Digital I/O
	CMCON = 0B00000111 ;	// 比較器關閉
	ROW_LAT = 0xff;		//
	ROW_TRIS = 0; 		// 將ROW_TRIS設為0，ROW設定為輸出
	SCAN_LAT = 0xff;	//
	SCAN_TRIS = 0; 		// 將SCAN_TRIS設為0，SCAN設定為輸出
	
	COLOR_SELECT = 0;
	DELAY_COUNT = 0;
	INDEX = 0;
	SCAN_COUNT = 0;
	
	delay_ms(100);

	while(1)
	{
		SCAN_LAT = 0xff;
		
		unsigned char ROW_DATA = reverse_DATA(~TABLE[SCAN_COUNT+INDEX]);
		switch(COLOR_SELECT)
		{
			case 0:	ROW_LAT = ROW_DATA;
				PAE_WR(0xff);
				break;
			case 1:	ROW_LAT = 0xff;
				PAE_WR(ROW_DATA);
				break;
			case 2:	ROW_LAT = ROW_DATA;
				PAE_WR(ROW_DATA);
				break;
				
			case 3:	ROW_LAT = ~ROW_DATA;
				PAE_WR(0xff);
				break;
			case 4:	ROW_LAT = 0x00;
				PAE_WR(ROW_DATA);
				break;
			case 5:	ROW_LAT = ~ROW_DATA;
				PAE_WR(ROW_DATA);
				break;
				
			case 6:	ROW_LAT = 0x00;
				PAE_WR(~ROW_DATA);
				break;
			case 7:	ROW_LAT = ~ROW_DATA;
				PAE_WR(~ROW_DATA);
				break;
			case 8:	ROW_LAT = ~ROW_DATA;
				PAE_WR(0x00);
				break;
				
			case 9:	ROW_LAT = ROW_DATA;
				PAE_WR(~ROW_DATA);
				break;
			case 10:ROW_LAT = ROW_DATA;
				PAE_WR(0x00);
				break;
			case 11:ROW_LAT = 0xff;
				PAE_WR(~ROW_DATA);
				break;
			
			default:ROW_LAT = 0xff;
				PAE_WR(0xff);
		}
		
		SCAN_LAT = SCAN_TABLE[SCAN_COUNT];
		SCAN_COUNT++;
		if(SCAN_COUNT >= 8) SCAN_COUNT = 0;
		
		if(DELAY_COUNT++ >= 50)		//Speed Control
		{
			DELAY_COUNT = 0;
			if(INDEX++ >= 79)
			{
				INDEX = 0;
				if(COLOR_SELECT++ == 11) COLOR_SELECT = 0;
			}
		}
		delay_ms(1);
	}
}
//============================================================================
