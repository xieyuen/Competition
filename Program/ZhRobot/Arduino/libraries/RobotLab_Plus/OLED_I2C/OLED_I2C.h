#ifndef __OLED_I2C_H
#define	__OLED_I2C_H


#define _I2C_HARDWARE    0
#define OLED_ADDRESS	   0x3c//(0x78 >>1)

#define FONT_SIZE_8X16  0
#define FONT_SIZE_6X8   1


extern unsigned char OLED_I2C_Init_flag;
class OLED_I2C{
public:
	OLED_I2C(int sdaport=-1,int sclport=-1) ;
	void init(int sdaport=-1,int sclport=-1) ;
	void write(unsigned char addr,unsigned char data) ;
	void OLED_Init(void);
	void Power(char state);
	void clear();
	void write_command(unsigned char command) ;
	void write_data(unsigned char value) ;
	void SetCursorPage(unsigned char y);
	void SetCursorColumn(unsigned char x);
	void SetCursor(unsigned char x, unsigned char y) ;
	void FillScreen(unsigned char fill_Data);
	void WakeUp(void);
	void Sleep(void);
	void Image(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char *BMP);
	void displayChar(unsigned int x, unsigned int y, unsigned char Ascii, unsigned char TextSize);
	void displayString(unsigned char x, unsigned char y, unsigned char *ch, unsigned char TextSize);
	void displayNumber(unsigned int x,unsigned int y,long num,unsigned int size);
	
private:	
	int scl_pin=-1;
	int sda_pin=-1;
};


#endif
