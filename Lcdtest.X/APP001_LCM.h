void LCM_Init(void);
void LCM_SetCursor(unsigned char X, unsigned char Y);
void LCM_PutASCII(unsigned char Ascii);
void LCM_PutHex(unsigned char Hex);
void LCM_PutROMString(const unsigned char * String);
void LCM_PutRAMString(unsigned char *String);
void LCM_PutNumber(unsigned int Number, unsigned char Digit);

// LCD Module Commands
#define Disp_2Line_4Bit_5x8Dots		0x28
#define Disp_2Line_8Bit_5x8Dots		0x38

#define Disp_Off			0x08		// Display off
#define Disp_On				0x0c		// Display on
#define Disp_On_Cursor                  0x0e		// Display on, Cursor on
#define Disp_On_Cursor_Blink		0x0f		// Display on, Cursor on, Blink cursor

#define Disp_Clear			0x01		// Clear the Display

#define Disp_Entry_Inc			0x06		//
#define Disp_Entry_Dec			0x04		//
