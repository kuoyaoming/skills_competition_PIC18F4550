#include <p18cxxx.h>
#include "APP001_LCM.h"

#define SystemFrequency			16000000L / 4

#define IntLCM_Delay1ns			( SystemFrequency * 1 ) / 1000000000L / 10
#define IntLCM_Delay10ns		( SystemFrequency * 10 ) / 1000000000L / 10
#define IntLCM_Delay100ns		( SystemFrequency * 100 ) / 1000000000L / 10

#define IntLCM_Delay1us			( SystemFrequency * 1 ) / 1000000L / 10
#define IntLCM_Delay10us		( SystemFrequency * 10 ) / 1000000L / 10
#define IntLCM_Delay100us		( SystemFrequency * 100 ) / 1000000L / 10

#define IntLCM_Delay1ms			( SystemFrequency * 1 ) / 1000L / 10
#define IntLCM_Delay10ms		( SystemFrequency * 10 ) / 1000L / 10
#define IntLCM_Delay100ms		( SystemFrequency * 100 ) / 1000L / 10

#define LCM_RS					RB5 
#define LCM_RW					RB4
#define LCM_E					RB3 

#define LCM_D4					RD4
#define LCM_D5					RD5
#define LCM_D6					RD6
#define LCM_D7					RD7 

#define LCM_TRIS_RS				TRISBbits.TRISB5
#define LCM_TRIS_RW				TRISBbits.TRISB4
#define LCM_TRIS_E				TRISBbits.TRISB3

#define LCM_TRIS_D4				TRISDbits.TRISD4 
#define LCM_TRIS_D5				TRISDbits.TRISD5 
#define LCM_TRIS_D6				TRISDbits.TRISD6 
#define LCM_TRIS_D7				TRISDbits.TRISD7 

// " / 10 " <- Fix C Code Function

void LCM_IOSetup(void)
{
    ADCON1bits.PCFG = 0x0d;

    LCM_TRIS_RS = 0;
    LCM_TRIS_RW = 0;
    LCM_TRIS_E = 0;

    LCM_TRIS_D4 = 0;
    LCM_TRIS_D5 = 0;
    LCM_TRIS_D6 = 0;
    LCM_TRIS_D7 = 0;

    LCM_RS = 0;
    LCM_RW = 0;
    LCM_E = 0;

    LCM_D7 = 0;
    LCM_D6 = 0;
    LCM_D5 = 0;
    LCM_D4 = 0;
}

void LCM_Delay(unsigned long Count)
{
    unsigned int i = 0;

    while (Count--)
    {
        i = 100;
        while (i--);
    }
}

void IntLCM_Communication(unsigned char NibbleByte)
{
    unsigned char Temporary;
    //Temporary = NibbleByte & 0x0f;

    LCM_Delay(IntLCM_Delay10ns * 4); // tAS

    LCM_D7 = 0;
    LCM_D6 = 0;
    LCM_D5 = 0;
    LCM_D4 = 0;

    Temporary = NibbleByte & 0x0f;
    if ((Temporary & 0x08) == 0x08) // D7
        LCM_D7 = 1;

    Temporary = NibbleByte & 0x0f;
    if ((Temporary & 0x04) == 0x04) // D6
        LCM_D6 = 1;

    Temporary = NibbleByte & 0x0f;
    if ((Temporary & 0x02) == 0x02) // D5
        LCM_D5 = 1;

    Temporary = NibbleByte & 0x0f;
    if ((Temporary & 0x01) == 0x01) // D4
        LCM_D4 = 1;

    LCM_E = 1;
    LCM_Delay(IntLCM_Delay100ns * 2.3); //PWEH

    LCM_E = 0;
    LCM_Delay(IntLCM_Delay10ns); //tAH
}

unsigned char LCM_IsBusy(void)
{

    /*
            unsigned char Temporary;

            PMPSetAddress( LCM_INSTRUCTION );
            Temporary = mPMPMasterReadByte( );		// Dummy Read for trigger PMP active.

            while( !INTGetFlag( INT_PMP ) );		// Wait PMP Complete.
            INTClearFlag( INT_PMP );

            Temporary = mPMPMasterReadByte( );

            return ( ( Temporary >> 7 ) & 0x01 );
     */

    unsigned int i;

    for (i = 0; i < 1000; i++);
    return 0;
}

void IntLCM_WriteData(unsigned char Data)
{
    while (LCM_IsBusy());
    LCM_RS = 1; // RS = 1 , Data
    LCM_RW = 0;

    IntLCM_Communication((Data >> 4) & 0x0f);
    IntLCM_Communication(Data & 0x0f);
}

void IntLCM_WriteInstruction(unsigned char Instruction)
{
    while (LCM_IsBusy());
    LCM_RS = 0; // RS = 0 , Instruction
    LCM_RW = 0;

    IntLCM_Communication((Instruction >> 4) & 0x0f);
    IntLCM_Communication(Instruction & 0x0f);
}

void LCM_Init(void)
{
    LCM_IOSetup();

    LCM_RS = 0; // RS = 0 , Instruction
    LCM_RW = 0;

    IntLCM_Communication(0x03); // Reset
    LCM_Delay(IntLCM_Delay1ms * 4.1); // Wait 4.1 mSecs

    IntLCM_Communication(0x03); // Reset
    LCM_Delay(IntLCM_Delay100us); // Wait 100 uSecs

    IntLCM_Communication(0x03); // Reset
    LCM_Delay(IntLCM_Delay100us); // Wait 100 uSecs

    IntLCM_Communication(0x02);
    LCM_Delay(IntLCM_Delay100us); // Wait 100 uSecs

    IntLCM_Communication((0x28 >> 4) & 0x0f);
    IntLCM_Communication(0x28 & 0xf);
    LCM_Delay(IntLCM_Delay10us * 3.7);

    IntLCM_Communication((Disp_Off >> 4) & 0x0f); // Dsplay Off
    IntLCM_Communication(Disp_Off & 0xf);
    LCM_Delay(IntLCM_Delay10us * 3.7);

    IntLCM_Communication((Disp_Clear >> 4) & 0x0f); // Dsplay Clear
    IntLCM_Communication(Disp_Clear & 0xf);
    LCM_Delay(IntLCM_Delay1ms * 1.52);

    IntLCM_Communication((Disp_Entry_Inc >> 4) & 0x0f); // Entry Mode
    IntLCM_Communication(Disp_Entry_Inc & 0xf);
    LCM_Delay(IntLCM_Delay10us * 3.7);

    IntLCM_Communication((Disp_On >> 4) & 0x0f); // Display On
    IntLCM_Communication(Disp_On & 0xf);
    LCM_Delay(IntLCM_Delay10us * 3.7);
}

void LCM_SetCursor(unsigned char X, unsigned char Y)
{
    IntLCM_WriteInstruction((0x80 + (Y * 0x40) + X));
}

void LCM_PutASCII(unsigned char Ascii)
{
    IntLCM_WriteData(Ascii);
}

void LCM_PutHex(unsigned char Hex)
{
    unsigned char Temporary;

    Temporary = (Hex >> 4) & 0x0f;

    if (Temporary > 9)
        Temporary += 0x37;
    else
        Temporary += '0';

    IntLCM_WriteData(Temporary);

    Temporary = Hex & 0x0f;
    if (Temporary > 9)
        Temporary += 0x37;
    else Temporary += '0';
    IntLCM_WriteData(Temporary);
}

void LCM_PutROMString(const unsigned char *String)
{
    while (*String != 0x00)
        IntLCM_WriteData(*String++);
}

void LCM_PutRAMString(unsigned char *String)
{
    while (*String != 0x00)
        IntLCM_WriteData(*String++);
}

static unsigned char Disable_Zero = 1;

unsigned char LCM_BCD_Regulate(unsigned char BCD)
{
    if (BCD == 0)
    {
        if (Disable_Zero == 1)
            return ' ';
        else
            return '0';
    }

    else
    {
        Disable_Zero = 0;
        return ( BCD += '0');
    }
}

void LCM_PutNumber(unsigned int Number, unsigned char Digit)
{
    unsigned char Temporary;
    Disable_Zero = 1;

    switch (Digit)
    {
    default:
    case 4:
        Temporary = LCM_BCD_Regulate((Number % 10000) / 1000);
        IntLCM_WriteData(Temporary);

    case 3:
        Temporary = LCM_BCD_Regulate((Number % 1000) / 100);
        IntLCM_WriteData(Temporary);

    case 2:
        Temporary = LCM_BCD_Regulate((Number % 100) / 10);
        IntLCM_WriteData(Temporary);

    case 1:
        Disable_Zero = 0;
        Temporary = LCM_BCD_Regulate(Number % 10);
        IntLCM_WriteData(Temporary);
    }
}
