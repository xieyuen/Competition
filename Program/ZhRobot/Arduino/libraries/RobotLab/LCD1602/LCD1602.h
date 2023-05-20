#ifndef LCD1602_h
#define LCD1602_h

// flags for backlight control
#define LCD_BACKLIGHT   0x08
#define LCD_NOBACKLIGHT 0x00

extern unsigned int LCD1602_Init_flag;
class LCD1602{
public:
	LCD1602(int sdaport=-1,int sclport=-1) ;
	void init(int sdaport=-1,int sclport=-1) ;
	void write(unsigned char addr,unsigned char data) ;
	void clear();
	void	enable();
	void write_command(unsigned char command) ;
	void write_data(unsigned char value) ;
	void setBacklight(uint8_t new_val);
	void displayChar(unsigned char x,unsigned char y,unsigned char c) ;
	void displayString(unsigned char x,unsigned char y,unsigned char *s) ;
	void displayNumber(unsigned char x,unsigned char y,long num,unsigned char bits);
	
private:	
	int scl_pin=-1;
	int sda_pin=-1;
	unsigned char LCD_data=0x08;
	uint8_t _backlightval;
};

#endif//LED7Segment