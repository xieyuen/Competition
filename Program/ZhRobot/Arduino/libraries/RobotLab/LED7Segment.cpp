#include <Arduino.h>
#include <Wire.h>
#include "wiring_private.h"
#include "pins_arduino.h"
#include "./LED7Segment/LED7Segment.h"
#include "./SoftwareI2C/SoftwareI2C.h"

#define ON  1
#define OFF 0

#define TM1650_DISPLAY_BASE 0x34//(0X68>>1) 0X68\0X6A\0X6C\0X6E//显示命令设置
#define TM1650_DCTRL_BASE   0x24//(0X48>>1)//数据命令设置 

#define TM1650_ONOFF_SHIFT		0
#define TM1650_MSK_ONOFF			0b11111110
#define TM1650_DOT_SHIFT			3
#define TM1650_MSK_DOT				0b11110111
#define TM1650_BRIGHT_SHIFT	4
#define TM1650_MSK_BRIGHT		0b10001111

#define TM1650_MIN_BRIGHT	0
#define TM1650_MAX_BRIGHT	7

#define TM1650_DISPLAY_SZIE 4
#define TM1650_DCTRL_SIZE   4
unsigned int _tm1650_display[TM1650_DISPLAY_SZIE]={0,0,0,0};
#define setDisplayDot(p,b)     _tm1650_display[p]=(_tm1650_display[p]&0x7f)|(((b)&0x01)<<7)
#define getDisplayDot(p)       _tm1650_display[p]&0x80
#define getDisplay7Seg(p)      _tm1650_display[p]&0x7f
#define setDisplay7Seg(p,b)    _tm1650_display[p]=(_tm1650_display[p]&80)|((b)&0x7f)

unsigned char _tm1650_dctrl[TM1650_DCTRL_SIZE]={0,0,0,0};//0bbbd00x   0xxx 0000
#define setBrightnessBits(p,b) _tm1650_dctrl[p]=(_tm1650_dctrl[p]&TM1650_MSK_BRIGHT)|(((b)&0x07)<<TM1650_BRIGHT_SHIFT)
//#define setDotBits(p,b)        _tm1650_dctrl[p]=(_tm1650_dctrl[p]&TM1650_MSK_DOT)|(((b)&0x01)<<TM1650_DOT_SHIFT)
#define setONOFFBits(p,b)      _tm1650_dctrl[p]=(_tm1650_dctrl[p]&TM1650_MSK_ONOFF)|(((b)&0x01)<<TM1650_ONOFF_SHIFT)

#define TM1650_USE_PROGMEM

#ifdef TM1650_USE_PROGMEM
#include <avr/pgmspace.h>
#endif

#ifndef TM1650_USE_PROGMEM
const byte _Ascii_7Seg[128] {
#else
const PROGMEM byte _Ascii_7Seg[128] {
#endif
//0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10
  0x00, 0x82, 0x21, 0x00, 0x00, 0x00, 0x00, 0x02, 0x39, 0x0F, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, // 0x20
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7f, 0x6f, 0x00, 0x00, 0x00, 0x48, 0x00, 0x53, // 0x30
  0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x06, 0x1E, 0x00, 0x38, 0x00, 0x54, 0x3F, // 0x40
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x00, 0x0F, 0x00, 0x08, // 0x50 
  0x63, 0x5F, 0x7C, 0x58, 0x5E, 0x7B, 0x71, 0x6F, 0x74, 0x02, 0x1E, 0x00, 0x06, 0x00, 0x54, 0x5C, // 0x60
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x1C, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x30, 0x0F, 0x00, 0x00  // 0x70
};

LED7Segment::LED7Segment(int sdaport,int sclport) 
{
#ifdef SoftwareI2C_H
  if(sdaport>=0&&sclport>=0)
	{
		sda_pin=sdaport;scl_pin=sclport;
	}
	else
#endif
	{
		sda_pin=-1;scl_pin=-1;
	}
}

void LED7Segment::init(int sdaport,int sclport) 
{
#ifdef SoftwareI2C_H
  if(sdaport>=0&&sclport>=0)
	{
		sda_pin=sdaport;scl_pin=sclport;
		//SoftwareI2C iic(sda_pin,scl_pin);
		//char ack=0;
		//ack=iic.beginTransmission(TM1650_DISPLAY_BASE);
		//iic.endTransmission();
		//if(ack!= 0) TM1650_Init_flag=0;
	}
	else
#endif
	{
		Wire.begin(); //Join the bus as master
	}
	clear();
	displayState(0,ON);
	setBrightness(0,3);
}

void LED7Segment::write(unsigned char addr,unsigned char data) 
{
#ifdef SoftwareI2C_H
  if(sda_pin>=0&&scl_pin>=0)
	{
		SoftwareI2C iic(sda_pin,scl_pin);
		char ack=0;
		ack=iic.beginTransmission(addr);
		iic.write(data);
		iic.endTransmission();
		if(ack!= 0) TM1650_Init_flag=0;
  //Serial.print("ack:\t");
  //Serial.println(ack,1);
	}
	else
#endif
	{
		Wire.beginTransmission(addr);
		Wire.write(data);
		if(Wire.endTransmission() != 0) TM1650_Init_flag=0;
	}	
}
#define WRITE_DCTRL(pos,c) write(TM1650_DCTRL_BASE+pos,c) 
void LED7Segment::setBrightness(unsigned char pos,unsigned int aValue) 
{
	if(aValue>TM1650_MAX_BRIGHT||pos>TM1650_DCTRL_SIZE)return ;
	int p=0,n=TM1650_DCTRL_SIZE;
	if(pos){p=pos-1;n=pos;}
	for(p;p<n;p++)
	{
		setBrightnessBits(p,aValue);
		//write(TM1650_DCTRL_BASE+p,_tm1650_dctrl[p]);
		WRITE_DCTRL(p,_tm1650_dctrl[p]) ;
	} 
}

void LED7Segment::displayState (unsigned char pos,bool aState)
{
	if(pos>TM1650_DCTRL_SIZE)return ;
	int p=0,n=TM1650_DCTRL_SIZE;
	if(pos){p=pos-1;n=pos;}
	for(p;p<n;p++)
	{
		setONOFFBits(p,aState);
		//write(TM1650_DCTRL_BASE+p,_tm1650_dctrl[p]);
		WRITE_DCTRL(p,_tm1650_dctrl[p]) ;
	} 	
}

#define WRITE_DISPLAY(p,c) _tm1650_display[p]=c;write(TM1650_DISPLAY_BASE+p,c);
void LED7Segment::clear ()
{
	for(int p;p<TM1650_DCTRL_SIZE;p++)
	{
		//write(TM1650_DISPLAY_BASE+p,0);
		WRITE_DISPLAY(p,0);
	} 
}

void	LED7Segment::setDot(unsigned int pos, bool aState)
{
	if(pos>TM1650_DISPLAY_SZIE)return ;
	int p=0,n=TM1650_DISPLAY_SZIE;
	if(pos){p=pos-1;n=pos;}
	for(p;p<n;p++)
	{
		setDisplayDot(p,aState);
		WRITE_DISPLAY(p,_tm1650_display[p]);
	} 		
}

void LED7Segment::displayChar(char pos,unsigned char c)
{
	unsigned char ch,dot,data;
	if(pos>=TM1650_DISPLAY_SZIE)return ;
	ch=c&0x7f;
	dot=getDisplayDot(pos);
#ifndef TM1650_USE_PROGMEM	  
	data = _Ascii_7Seg[ch]&0x7f;
#else
	data = pgm_read_byte_near(_Ascii_7Seg + ch)&0x7f;
#endif
	//if(data)
	{
		//write(TM1650_DISPLAY_BASE+pos,_tm1650_display[pos]) ;
		WRITE_DISPLAY(pos,((data&0x7f)|(dot)));
	}
}

void LED7Segment::displayString(char *aString)
{
	for(int p=0;p<TM1650_DISPLAY_SZIE&&*aString;p++,aString++)
	{
		displayChar(p,*aString);
	}
}

void LED7Segment::displayNumber4D(int num)
{
	char a;
	int i;
	for (i=0; i<4; i++) 
	{
		a=num%10;
		if(num||i==0)a=a+'0';
		else if(i&&num==0)a=' ';
		num/=10;
		displayChar(3-i,a);
	}
}