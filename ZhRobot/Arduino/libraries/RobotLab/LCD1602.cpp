#include <Arduino.h>
#include <Wire.h>
#include "wiring_private.h"
#include "pins_arduino.h"
#include "./LCD1602/LCD1602.h"
#include "./SoftwareI2C/SoftwareI2C.h"

#define ON  1
#define OFF 0

#define LCD1602_I2C_ADDR 0X20
unsigned int LCD1602_Init_flag=0;

LCD1602::LCD1602(int sdaport,int sclport) 
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
	LCD_data=LCD_BACKLIGHT;
	_backlightval = LCD_NOBACKLIGHT;
}

void LCD1602::init(int sdaport,int sclport) 
{
#ifdef SoftwareI2C_H
  if(sdaport>=0&&sclport>=0)
	{
		sda_pin=sdaport;scl_pin=sclport;
		//SoftwareI2C iic(sda_pin,scl_pin);
		//char ack=0;
		//ack=iic.beginTransmission(TM1650_DISPLAY_BASE);
		//iic.endTransmission();
		//if(ack!= 0) LCD1602_Init_flag=0;
	}
	else
#endif
	{
		Wire.begin(); //Join the bus as master
	}
	write_command(0x02);
  delayMicroseconds(40);
  write_command(0x28); //4位显示
  write_command(0x0c); //显示开
  write_command(0x01); //清屏
  delay(10);
	//clear();
}

void LCD1602::write(unsigned char addr,unsigned char data) 
{
#ifdef SoftwareI2C_H
  if(sda_pin>=0&&scl_pin>=0)
	{
		SoftwareI2C iic(sda_pin,scl_pin);
		char ack=0;
		ack=iic.beginTransmission(addr);
		iic.write(data);
		iic.endTransmission();
		if(ack!= 0) LCD1602_Init_flag=0;
	}
	else
#endif
	{
		Wire.beginTransmission(addr);
		Wire.write(data);
		if(Wire.endTransmission() != 0) LCD1602_Init_flag=0;
	}	
}
//********************液晶屏使能*********************
void LCD1602::enable()
{
	//unsigned char LCD_data=0x08;
  LCD_data|=(1<<(3-1));//E=1;
  write(LCD1602_I2C_ADDR,LCD_data);  //
  delayMicroseconds(2);
  LCD_data&=~(1<<(3-1));//E=0;
  write(LCD1602_I2C_ADDR,LCD_data);
}
void LCD1602::setBacklight(uint8_t new_val)
{
	//unsigned char LCD_data=0x08;
	if(new_val)_backlightval=LCD_BACKLIGHT;
	else _backlightval=LCD_NOBACKLIGHT;
  LCD_data=(LCD_data&(~LCD_BACKLIGHT))|_backlightval;
  write(LCD1602_I2C_ADDR,LCD_data);  
}
//写入命令函数
void LCD1602::write_command(unsigned char command) 
{ 
	//unsigned char LCD_data=0x08;
	delayMicroseconds(16);
  LCD_data&=~(1<<(1-1));//RS=0;
  LCD_data&=~(1<<(2-1));//RW=0;
  //LCD_data&=~(1<<(4-1));
  write(LCD1602_I2C_ADDR,LCD_data);
	
  LCD_data&=0X0f; //清高四位
  LCD_data|=command & 0xf0; //写高四位
  write(LCD1602_I2C_ADDR,LCD_data);
  enable();

  command=command<<4; //低四位移到高四位
  LCD_data&=0x0f; //清高四位
  LCD_data|=command&0xf0; //写低四位
  write(LCD1602_I2C_ADDR,LCD_data);
  enable();
}
//写入数据函数
void LCD1602::write_data(unsigned char value) 
{ 
	//unsigned char LCD_data=0x08;
  delayMicroseconds(16);
  LCD_data|=(1<<(1-1));//RS=1;
  LCD_data&=~(1<<(2-1));//RW=0;
  write(LCD1602_I2C_ADDR,LCD_data);

  LCD_data&=0X0f; //清高四位
  LCD_data|=value&0xf0; //写高四位
  write(LCD1602_I2C_ADDR,LCD_data);
  enable();         

  value=value<<4; //低四位移到高四位
  LCD_data&=0x0f; //清高四位
  LCD_data|=value&0xf0; //写低四位
  write(LCD1602_I2C_ADDR,LCD_data);
  enable();
}
//清屏函数
void LCD1602::clear(void) 
{ 
	write_command(0x01); 
	delay(2);
}
//显示字符
void LCD1602::displayChar(unsigned char x,unsigned char y,unsigned char c) 
{ 
	if(x>=16||y>=2)return;  
	write_command(0x80|(y<<6) + x);
  write_data(c);        
}
//显示字符串函数
void LCD1602::displayString(unsigned char x,unsigned char y,unsigned char *s) 
{ 
	if(x>=16||y>=2)return;
  
	write_command(0x80|(y<<6) + x);
  while (*s) 
  {     
    write_data(*s++);
    if(++x>=16)break; 
  }//*/
	/*while (*s) 
  {     
    displayChar(x,y,*s++);
    if(++x>=16)break; 
  }//*/
}

void LCD1602::displayNumber(unsigned char x,unsigned char y,long num,unsigned char bits)
{
	char a, s=0,i;
	char buf[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	
	if(bits==0||bits>10)return ;
	if(num<0){s=1;num=-num;}
	bits+=1;
	for(i=0; i<bits; i++) 
	{
		a=num%10;
		if(num||i==0){	a=a+'0';}
		else if(i&&num==0)
		{
			if(s==1){s=2;	a='-';}
			else {a=' ';}
		}
		num/=10;
		buf[bits-1-i]=a;
	}
	displayString(x,y,buf);
}