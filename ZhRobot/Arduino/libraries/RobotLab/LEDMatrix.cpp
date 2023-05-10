#include "./LEDMatrix/LEDMatrix.h"
#include"./SoftwareI2C/SoftwareI2C.h"
#define i2c_addr 0x70

uint8_t LM_displaybuffer[8]={0,0,0,0,0,0,0,0};
uint8_t LM_init_flag=0;

void LM_iic_writereg(int sdaport,int sclport,uint8_t data)
{
#ifdef SoftwareI2C_H
  if(sdaport>=0&&sclport>=0)//if(SoftwareI2CFlag==SOFTWAREI2C_MODE)
	{
	SoftwareI2C iic(sdaport,sclport);
	uint8_t ack=0;
	ack=iic.beginTransmission(i2c_addr);
  iic.write(data);
  iic.endTransmission();
	if(ack != 0)  LM_init_flag=0;
	return ;
	}
	else
#endif//SoftwareI2C_H
	{
  Wire.beginTransmission(i2c_addr);
  Wire.write(data);
  //Wire.endTransmission();
	if(Wire.endTransmission() != 0)  LM_init_flag=0;
	}
}
void LM_iic_writedata(int sdaport,int sclport,uint8_t reg,uint8_t *data,int len)
{
#ifdef SoftwareI2C_H
  if(sdaport>=0&&sclport>=0)
	{
	SoftwareI2C iic(sdaport,sclport);
	uint8_t ack=0;
	ack=iic.beginTransmission(i2c_addr);
  iic.write(reg);
  for(int i=0;i<len;i++)
  {
    iic.write(*data++);
  }
  iic.endTransmission(); 
	if(ack != 0)  LM_init_flag=0;
	return ;
	}
	else
#endif//SoftwareI2C_H
	{
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg);
  for(int i=0;i<len;i++)
  {
    Wire.write(*data++);
  }
  //Wire.endTransmission(); 
	if(Wire.endTransmission() != 0)  LM_init_flag=0;
	}
}
void LM_setBrightness(int sdaport,int sclport,uint8_t b) //LED亮度 0~15 0最暗,15最亮
{
  if (b > 15) b = 15;  
  LM_iic_writereg(sdaport,sclport,HT16K33_CMD_BRIGHTNESS | b);
}

void LM_blinkRate(int sdaport,int sclport,uint8_t b) //led闪烁频率 0~3 0关闭闪烁,1 2HZ,2 1HZ,3 0.5HZ
{
  if (b > 3) b = 0; // turn off if not sure
  LM_iic_writereg(sdaport,sclport,HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1));
}
void LM_clear(int sdaport,int sclport) ;
void LM_init(int sdaport,int sclport) 
{
	if(LM_init_flag)return ;LM_init_flag=1;
#ifdef SoftwareI2C_H
  if(sdaport<0||sclport<0)
	{
		Wire.begin();
	}
#endif
	LM_iic_writereg(sdaport,sclport,0x21);// System setup:turn on oscillator
  LM_blinkRate(sdaport,sclport,HT16K33_BLINK_OFF);//led闪烁频率
  LM_setBrightness(sdaport,sclport,5); // LED亮度
	LM_clear(sdaport,sclport) ;
}
//设置一个点
void LM_writeDisplayDot(int sdaport,int sclport,uint8_t x,uint8_t y,uint8_t state) //x=com;y=row,state=0/1
{
  if(x>8||y>8)return ;
	if(x)x-=1;	if(y)y-=1;
  //if(x==7)x=6;else if(x==6)x=7;
  if(x==0)x=1;else if(x==1)x=0;
  if(state)LM_displaybuffer[x]|=(1<<y);  else LM_displaybuffer[x]&=~(1<<y);
  LM_iic_writedata(sdaport,sclport,HT16K33_DISPLAY_RAM+((7-x)<<1),(uint8_t *)&LM_displaybuffer[x],1);
}

//全屏显示,刷新全部点
void LM_writeDisplay(int sdaport,int sclport,int x,int y,uint8_t *displaybuffer) //displaybuffer 为8个字节显示数据
{
  uint8_t i,j,dir=0;
	if(x<0)dir|=0x01;if(y<0)dir|=0x02;
	x=abs(x);y=abs(y);
	if(x>8||y>8)return;
	if(x)x-=1;	if(y)y-=1;
	
  for(i=0;i<8;i++)
  {
    for(j=0;j<8;j++)
    {
			if(i<x||j<y)  LM_writeDisplayDot(sdaport,sclport,i+1,j+1,0);
      else LM_writeDisplayDot(sdaport,sclport,i+1,j+1,(displaybuffer[i-x]>>(j-y))&0x01);
    }
  }
}

//设置一列
void LM_writeDisplayCom(int sdaport,int sclport,uint8_t x,uint16_t data) 
{
  if(x>8)return ;
	if(x)x-=1;	
  //if(x==7)x=6; else if(x==6)x=7;
  if(x==0)x=1;else if(x==1)x=0;
  LM_displaybuffer[x]=data;
  //LM_iic_writedata(0x00+(x<<1),(uint8_t *)&data,1);
  LM_iic_writedata(sdaport,sclport,HT16K33_DISPLAY_RAM+((7-x)<<1),(uint8_t *)&data,1);
}
//清屏
void LM_clear(int sdaport,int sclport) 
{
#ifdef SoftwareI2C_H
  if(sdaport>=0&&sclport>=0)//if(SoftwareI2CFlag==SOFTWAREI2C_MODE)
	{
	SoftwareI2C iic(sdaport,sclport);
	uint8_t ack=0;
	ack=iic.beginTransmission(i2c_addr);
  iic.write((uint8_t)HT16K33_DISPLAY_RAM); // start at address $00

  for (uint8_t i=0; i<8; i++) //COM
  {
    iic.write(0);    //ROW
    iic.write(0);  
    LM_displaybuffer[i]=0;    
  }
  iic.endTransmission(); 
  if(ack != 0)  LM_init_flag=0; 
	}
	else
#endif
	{
  Wire.beginTransmission(i2c_addr);
  Wire.write((uint8_t)HT16K33_DISPLAY_RAM); // start at address $00

  for (uint8_t i=0; i<8; i++) //COM
  {
    Wire.write(0);    //ROW
    Wire.write(0);  
    LM_displaybuffer[i]=0;    
  }
  //Wire.endTransmission(); 
 if(Wire.endTransmission() != 0)  LM_init_flag=0; 
 }
}