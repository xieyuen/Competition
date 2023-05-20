/************************************************************************************
* Description:128*64�����OLED��ʾ�������ļ����������ڻ����Զ���(heltec.taobao.com)��SD1306����IICͨ�ŷ�ʽ��ʾ��
*
* Others: none;
*
* Function List:
*	1. void LCD_Init(void) -- ����CPU��Ӳ��I2C
* 2. void write(unsigned char addr,unsigned char data) -- ��Ĵ�����ַдһ��byte������
* 3. void write_command(unsigned char I2C_Command) -- д����
* 4. void write_data(unsigned char I2C_Data) -- д����
* 5. void OLED_Init(void) -- OLED����ʼ��
* 6. void SetCursor(unsigned char x, unsigned char y) -- ������ʼ������
* 7. void FillScreen(unsigned char fill_Data) -- ȫ�����
* 8. void clear(void) -- ����
* 9. void WakeUp(void) -- ����
* 10. void Sleep(void) -- ˯��
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ��ʾ�ַ���(�����С��6*8��8*16����)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
*
* History: none;
*
*************************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "wiring_private.h"
#include "pins_arduino.h"
#include "./OLED_I2C/OLED_I2C.h"
#include "./OLED_I2C/asciilib.h"
#include "./SoftwareI2C/SoftwareI2C.h"

#define OLED_WIGTH 128
#define OLED_HIGH  8//(64/8)
unsigned char OLED_Init_flag=0;
OLED_I2C::OLED_I2C(int sdaport,int sclport) 
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
void OLED_I2C::Power(char state)
{
	char pin=39;
	pinMode(pin,OUTPUT);
	if(state)
	{
		digitalWrite(pin, LOW);
	}
	else 
	{
		digitalWrite(pin, HIGH);
	}
}
void OLED_I2C::init(int sdaport,int sclport) 
{
	Power(1);
#ifdef SoftwareI2C_H
  if(sdaport>=0&&sclport>=0)
	{
		sda_pin=sdaport;scl_pin=sclport;
	}
	else
#endif
	{
		Wire.begin(); //Join the bus as master
	}
	
  OLED_Init();
  FillScreen(0x00);
}
void OLED_I2C::write(unsigned char addr,unsigned char data)
{
#ifdef SoftwareI2C_H
  if(sda_pin>=0&&scl_pin>=0)
	{
		SoftwareI2C iic(sda_pin,scl_pin);
		char ack=0;
		ack=iic.beginTransmission(OLED_ADDRESS);
		iic.write(addr);
		iic.write(data);
		iic.endTransmission();
		if(ack!= 0) OLED_Init_flag=0;
	}
	else
#endif
	{
		Wire.beginTransmission(OLED_ADDRESS);
		Wire.write(addr);
		Wire.write(data);
		if(Wire.endTransmission() != 0) OLED_Init_flag=0;
	}	
}
void OLED_I2C::write_command(u8 I2C_Command)//д����
{
	write(0x00, I2C_Command);
}
void OLED_I2C::write_data(u8 I2C_Data)//д����
{
	write(0x40, I2C_Data);
}
void OLED_I2C::OLED_Init(void)
{
	delay(100); //�������ʱ����Ҫ
	
	write_command(0xAE); //display off
	//�����ڴ��ַģʽ
	write_command(0x20);	//Set Memory Addressing Mode	
	write_command(0x10);	//00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	write_command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	write_command(0xc8);	//Set COM Output Scan Direction
	write_command(0x00); //---set low column address
	write_command(0x10); //---set high column address
	write_command(0x40); //--set start line address

	write_command(0x81); //--set contrast control register ���öԱȶ�
	write_command(0xff); //���ȵ��� 0x00~0xff

	write_command(0xa1); //--set segment re-map 0 to 127 0xA1: ���ҷ��ã�  0xA0: ������ʾ��Ĭ��0xA0��
	write_command(0xa6); //--set normal display
	write_command(0xa8); //--set multiplex ratio(1 to 64)
	write_command(0x3F); //
	write_command(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	write_command(0xd3); //-set display offset
	write_command(0x00); //-not offset
	write_command(0xd5); //--set display clock divide ratio/oscillator frequency
	write_command(0xf0); //--set divide ratio
	write_command(0xd9); //--set pre-charge period
	write_command(0x22); //
	write_command(0xda); //--set com pins hardware configuration
	write_command(0x12);
	write_command(0xdb); //--set vcomh
	write_command(0x20); //0x20,0.77xVcc
	write_command(0x8d); //--set DC-DC enable
	write_command(0x14); ///
	write_command(0xaf); //--turn on oled panel*/
	delay(100);
}

void OLED_I2C::SetCursorPage(unsigned char y)
{
	write_command(0xb0+y);
}
void OLED_I2C::SetCursorColumn(unsigned char x)
{
	write_command(((x&0xf0)>>4)|0x10);//0X10-0X1F
	write_command((x&0x0f)|0x01);//0X00-0X0F
}
void OLED_I2C::SetCursor(unsigned char x, unsigned char y) //������ʼ������ x=0~127,y=0~7(ҳ)
{ //ҳ��ַģʽ
	write_command(0xb0+y);
	write_command(((x&0xf0)>>4)|0x10);//0X10-0X1F
	write_command((x&0x0f)|0x01);//0X00-0X0F
}

void OLED_I2C::FillScreen(unsigned char fill_Data)//ȫ�����
{
	unsigned char m,n;
	for(m=0;m<OLED_HIGH;m++)
	{
		write_command(0xb0+m);		//page0-page1
		write_command(0x00);		//low column start address
		write_command(0x10);		//high column start address
		for(n=0;n<OLED_WIGTH;n++)
		{
			write_data(fill_Data);
		}
	}
}

void OLED_I2C::clear(void)//����
{
	FillScreen(0x00);
}

//--------------------------------------------------------------
// Prototype      : void WakeUp(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED�������л���     >=100ms
//--------------------------------------------------------------
void OLED_I2C::WakeUp(void)
{
	write_command(0X8D);  //���õ�ɱ�
	write_command(0X14);  //������ɱ�
	write_command(0XAF);  //OLED����
}

//--------------------------------------------------------------
// Prototype      : void Sleep(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA   >=100ms
//--------------------------------------------------------------
void OLED_I2C::Sleep(void)
{
	write_command(0X8D);  //���õ�ɱ�
	write_command(0X10);  //�رյ�ɱ�
	write_command(0XAE);  //OLED����
}
void OLED_I2C::displayChar(unsigned int x, unsigned int y, unsigned char Ascii, unsigned char TextSize)
{
  unsigned int i,j,in=8,jn=2;
  unsigned int addr=Ascii-32;
	unsigned char *p;
	switch(TextSize)
	{
		case FONT_SIZE_6X8:
		{
		
			//p=Ascii6X8Addr(addr);//(unsigned char *)Ascii6X8[addr];
			in=6,jn=1;
			for(j=0;j<jn;j++)
			{
				SetCursor(x,y+j);
				if(x>(OLED_WIGTH-1))break;
				for(i=0;i<in;i++)
				{
					if((x+i)>=(OLED_WIGTH-1))break;
					write_data(Ascii6X8Addr(addr,i+j*8));
//#ifndef MEM_USER_DEFINE
//					write_data(Ascii6X8[addr][i+j*8]);
//#else
//					write_data(pgm_read_byte_near(&Ascii6X8[addr][i+j*8]) );
//#endif
				}
			}
      //SetCursor(x,y);
			//for(i=0;i<6;i++)
			//{
      //   write_data(p[i]);
      //}
    }break;
    case FONT_SIZE_8X16:
		default:
		{
			//p=Ascii8X16Addr(addr);//(unsigned char *)Ascii8X16[addr];
			in=8,jn=2;
			for(j=0;j<jn;j++)
			{
				SetCursor(x,y+j);
				if(x>(OLED_WIGTH-1))break;
				for(i=0;i<in;i++)
				{
					if((x+i)>=(OLED_WIGTH-1))break;
					write_data(Ascii8X16Addr(addr,i+j*8));
//#ifndef MEM_USER_DEFINE
//					write_data(Ascii8X16[addr][i+j*8]);
//#else
//					write_data(pgm_read_byte_near(&Ascii8X16[addr][i+j*8]) );
//#endif
				}
			}
      //for(j=0;j<2;j++)
      //{
      //  SetCursor(x,y+j);
      //  for(i=0;i<8;i++)
      //  {
      //     write_data(p[i+j*8]);
      //  }
      //}
    }break;
  }
	/*for(j=0;j<jn;j++)
  {
    SetCursor(x,y+j);
    if(x>127)break;
    for(i=0;i<in;i++)
    {
    	if((x+i)>=127)break;
      write_data(p[i+j*8]);
    }
  }//*/
}
//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//--------------------------------------------------------------
void OLED_I2C::displayString(unsigned char x, unsigned char y, unsigned char *ch, unsigned char TextSize)
{
  while(*ch!='\0')
  {
    if(*ch<0x80)
    {
      displayChar(x, y, *ch, TextSize);
    }
		x+=8;
    ch++;
  }
}


//*******************************************************************************
// Function Name  : LCD_DisplayNum
// Description    : �Զ���ʮ��������ʾ����
// Input          : x,y:������ʼ����;
//					        num:��ʾ������
//					        ch_color:������ɫ
//					        bg_color:������ɫ
//					        size:��ʾ�����ָ���;
//                  BIT0~BIT3:��ʾ1~8λ����λ
//					        BIT7:1:��ʾ����(0x80),ʮ������Ϊ����,,0:����ʾ����
//                  BIT6:�ֺ�,0С�ֺ�,1���ֺ�
//                  BIT5:ǰ���0Ϊ�ո�,1ǰ���0����
// Output         : None
// Return         : None
//*******************************************************************************
void OLED_I2C::displayNumber(unsigned int x,unsigned int y,long num,unsigned int size)//-0X7FFF FFFF ~ 0X7FFF FFFF
{
	unsigned char temp[18];
	char i=0;
	unsigned char NumSign=0;	//0��������1���Ǹ�
	unsigned char fsize=(size>>6)&0x01;
	unsigned char zeroflag=(size>>5)&0x01;
	unsigned char ii=0,flag=0,ns=size&0x80,nn,n=10;
	long m=num;
	size&=0x0f;
	ii=size;
//ʮ����
	if(num<0)
	{
		num=-num;
		NumSign= 1;
	}
	if(!ns){ii=size-1;NumSign=0;}

	for(i=0;i<size;i++)
	{
		nn=num%10;
		if(nn<10)temp[i]='0'+nn;
		num/=10;
	}
	temp[size]=' ';
	for(i=size;i>-1;i--)
	{
		if((temp[i-1]!=48)&&(flag==0))
		{
			if(NumSign==1&&m!=0)temp[i]='-';
			flag=1;
		}
		else
		{
			if((flag==0)&&(i>1))
			{
				if(zeroflag==0)temp[i-1]=' ';
			}
		}
	}
	if(temp[0]==48){temp[0]='0';}
	i=ii;//�Ҷ���
	n=ii+1;
	while(n--)
	{
		displayChar(x,y,temp[i],fsize);
		if(fsize==FONT_SIZE_6X8)x+=6;
		else x+=8;
		if(i>0)i--;	else i=ii;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
//--------------------------------------------------------------
void OLED_I2C::Image(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char *BMP)
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)		y = y1>>3;//y1/8;                                  
  else		y = y1>>3+1;//y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		SetCursor(x0,y);
    for(x=x0;x<x1;x++)
		{
			write_data(BMP[j++]);
		}
	}
}