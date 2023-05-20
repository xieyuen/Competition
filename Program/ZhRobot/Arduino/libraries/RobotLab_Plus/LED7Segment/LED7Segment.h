#ifndef LED7Segment_h
#define LED7Segment_h
extern unsigned int TM1650_Init_flag;
class LED7Segment{
public:
	LED7Segment(int sdaport=-1,int sclport=-1) ;
	void init(int sdaport=-1,int sclport=-1) ;
	void write(unsigned char addr,unsigned char data) ;
	void setBrightness(unsigned char pos,unsigned int aValue) ;
	void displayState (unsigned char pos,bool aState);
	void clear ();
	void	setDot(unsigned int pos, bool aState);
	void displayChar(char pos,unsigned char c);
	void displayString(char *aString);
	void displayNumber4D(int num);
	
private:	
	int scl_pin=-1;
	int sda_pin=-1;
};

#endif//LED7Segment