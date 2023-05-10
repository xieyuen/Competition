//atmega2560
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "./RLSerial/RLSerial.h"
//UCSRA

#define RXC    7
#define TXC    6
#define UDRE   5
#define FE     4
#define DOR    3
#define PE  		 2
#define U2X0   1
#define MPCM   0
//UCSRB
#define RXCIE  7
#define TXCIE  6
#define UDRIE  5
#define RXEN   4
#define TXEN   3
#define UCSZ2  2
#define RXB8   1
#define TXB8   0
//UCSRC
#define URSEL  7
#define UMSEL  6
#define UPM1   5
#define UPM0   4
#define USBS   3
#define UCSZ01 2
#define UCSZ00 1
#define UCPOL  0

//#define RLSERIAL_PORT_SIZE 3
volatile bool _is_protocol[RLSERIAL_PORT_SIZE];
volatile bool _protocol_enable[RLSERIAL_PORT_SIZE];
volatile uint8_t _protocol_sum[RLSERIAL_PORT_SIZE];
volatile uint8_t _protocol_flag[RLSERIAL_PORT_SIZE];
volatile uint16_t _rx_len[RLSERIAL_PORT_SIZE];
volatile uint16_t _resd_rx_len[RLSERIAL_PORT_SIZE];
volatile unsigned char _rx_buffer[RLSERIAL_PORT_SIZE][RLSERIAL_RX_BUFFER_SIZE];
volatile unsigned char _rxprotocol_buffer[RLSERIAL_PORT_SIZE][RLSERIAL_RX_BUFFER_SIZE];//用于缓存接收到的符合协议的数据包
volatile bool _is_write_buf[RLSERIAL_PORT_SIZE]={0,0,0};
volatile unsigned long _RLSerial_ClearTime[RLSERIAL_PORT_SIZE]={0,0,0,};

#define _RLSerial_CLEAR_TIME_MS 20
typedef enum
{
#if RLSERIAL_PORT_SIZE==4
	_RLSerial0=0,
	_RLSerial1,
#else
	_RLSerial1=0,
#endif
	_RLSerial2,
	_RLSerial3,
}RLSerial_Port_Defined;


RLSerial::RLSerial(uint8_t port) 
{
	if(port<=(RLSERIAL_PORT_SIZE-1))
	{

	}
}		
void RLSerial::rxmode(uint8_t port,uint8_t mode)
{
	if(_protocol_enable[port]!=mode)_protocol_enable[port]=mode;
}
bool RLSerial::readprotocolflag(uint8_t port)
{
	if(_rxprotocol_buffer[port][0]==__RLSerial_Protocol_Head1&&
	    _rxprotocol_buffer[port][1]==__RLSerial_Protocol_Head2)return 1;
	return 0;
	//return _is_protocol[port];
}
void RLSerial::clearprotocolflag(uint8_t port)
{
	_is_protocol[port]=0;
}
void RLSerial::clearprotocol(uint8_t port)
{
	_rxprotocol_buffer[port][0]=0;
	_rxprotocol_buffer[port][1]=0;
}
uint8_t RLSerial::readprotocol(uint8_t port,uint8_t n)
{
	return _rxprotocol_buffer[port][n];
}
unsigned char* RLSerial::getprotocolhandle(uint8_t port)
{
	return _rxprotocol_buffer[port];
}
unsigned char RLSerial::getleartime(uint8_t port)
{
	if((_RLSerial_ClearTime[port]&&_RLSerial_ClearTime[port]<millis()))//超过一定时间没有收到数据
	{
		return 1;
	}
	return 0;
}
void RLSerial::setcleartime(uint8_t port)
{
	 _RLSerial_ClearTime[port]=0;
}
uint8_t RLSerial::writebufferstate(uint8_t port)
{
	return _is_write_buf[port];
}
void RLSerial::begin(uint8_t port,unsigned long baud)
{
	uint16_t ubrr=(F_CPU/baud/16 -1);//异步正常模式 (U2X = 0)
	
	uint16_t ucsrax=0;
	_rx_len[port]=0;
	_rx_len[port]=0;
	_protocol_flag[port]=0;
	_is_protocol[port]=0;
	_is_write_buf[port]=0;
	_RLSerial_ClearTime[port]=0;
	memset(_rx_buffer[port],0,RLSERIAL_RX_BUFFER_SIZE);
	
	if((baud == 57600)||(baud == 115200)||(baud == 230400))
	{
		ucsrax|=1<<U2X0;
		ubrr=(F_CPU/baud/8 -1);
	}
	switch(port)
	{	
#if RLSERIAL_PORT_SIZE==4
		case _RLSerial0:
		{
			UCSR0A = ucsrax;	
			UCSR0B = 0x00;	
			UCSR0C = 0X00;
			UCSR0C =  (1<<UCSZ01)|(1<<UCSZ00);// 8位数据位  无校验位 1停止位
			UBRR0L =  (ubrr)&0XFF; 
			UBRR0H=(ubrr>>8)&0XFF;
			UCSR0B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
		}break;
#endif
		case _RLSerial1:
		{
			UCSR1A = ucsrax;	
			UCSR1B = 0x00;	
			UCSR1C = 0X00;
			UCSR1C = (1<<UCSZ01)|(1<<UCSZ00);// 8位数据位  无校验位 1停止位
			UBRR1L = (ubrr)&0XFF; 
			UBRR1H = (ubrr>>8)&0XFF;
			UCSR1B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
		}break;
		case _RLSerial2:
		{
			UCSR2A = ucsrax;
			UCSR2B = 0x00;	
			UCSR2C = 0X00;
			UCSR2C =  (1<<UCSZ01)|(1<<UCSZ00);// 8位数据位  无校验位 1停止位
			UBRR2L =  (ubrr)&0XFF; 
			UBRR2H=(ubrr>>8)&0XFF;
			UCSR2B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
		}break;
		case _RLSerial3:
		{
			UCSR3A = ucsrax;	
			UCSR3B = 0x00;	
			UCSR3C = 0X00;
			UCSR3C =  (1<<UCSZ01)|(1<<UCSZ00);// 8位数据位  无校验位 1停止位
			UBRR3L =  (ubrr)&0XFF; 
			UBRR3H=(ubrr>>8)&0XFF;
			UCSR3B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
		}break;
	}
}
void RLSerial::receivestate(uint8_t port,uint8_t state)
{
	switch(port)
	{	
#if RLSERIAL_PORT_SIZE==4
		case _RLSerial0:
		{
			if(state)UCSR0B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
			else UCSR0B &= UCSR0B &= ~((1<<RXEN)|(1<<RXCIE));
		}break;
#endif
		case _RLSerial1:
		{
			if(state)UCSR1B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
			else UCSR1B &= ~((1<<RXEN)|(1<<RXCIE));
		}break;
		case _RLSerial2:
		{
			if(state)UCSR2B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
			else UCSR2B &= ~((1<<RXEN)|(1<<RXCIE));
		}break;
		case _RLSerial3:
		{
			if(state)UCSR3B |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
			else UCSR3B &= ~((1<<RXEN)|(1<<RXCIE));
		}break;
	}	
}
int RLSerial::read(uint8_t port)
{
	uint8_t m_temp;
	if(_resd_rx_len[port]!=_rx_len[port])
	{
		m_temp = _rx_buffer[port][_resd_rx_len[port]];
		if(++_resd_rx_len[port]>=RLSERIAL_RX_BUFFER_SIZE)_resd_rx_len[port]=0;
		return m_temp;
	}
	return -1;
}

void RLSerial::write(uint8_t port,uint8_t data)
{
	switch(port)
	{	
#if RLSERIAL_PORT_SIZE==4
		case _RLSerial0:
		{
			while(!(UCSR0A & (1<<UDRE)));//0x20
			UDR0 = data;
		}break;
#endif
		case _RLSerial1:
		{
			while(!(UCSR1A & (1<<UDRE)));//0x20
			UDR1 = data;
		}break;
		case _RLSerial2:
		{
			while(!(UCSR2A & (1<<UDRE)));//0x20
			UDR2 = data;
		}break;
		case _RLSerial3:
		{
			while(!(UCSR3A & (1<<UDRE)));//0x20
			UDR3 = data;
		}break;
	}
}
void RLSerial::sendstring(uint8_t port,uint8_t *str)
{
	while(*str)
	{
		write(port,*str++);
	}
}
void _USARTX_RX_ISR(uint8_t port,uint8_t data)
{
	_rx_buffer[port][_rx_len[port]]=data;
	if(++_rx_len[port]>=RLSERIAL_RX_BUFFER_SIZE)_rx_len[port]=0;
	//Serial.write(data);
//__RLSerial_Protocol_Head1+__RLSerial_Protocol_Head2+LEN+type+D0+D1+...+Dn+SUM
	if(_protocol_enable[port]==0)return ;
  switch(_protocol_flag[port])
  {
    case 0:
    {
      if(data==__RLSerial_Protocol_Head1)
      {
        _protocol_flag[port]=1;
				_protocol_sum[port]=data;
      }
			else
			{
				_rx_len[port]=0;
			}
    }break;
    case 1:
    {
       if(data==__RLSerial_Protocol_Head2)
      {
        _protocol_flag[port]=2;
				_protocol_sum[port]+=data;
      }
			else
			{
				_rx_len[port]=0;
				_protocol_flag[port]=0;
			}
    }break;
    case 2:
    {
      if(data<=4||data>RLSERIAL_RX_BUFFER_SIZE)
      {
        _protocol_flag[port]=0;
				_rx_len[port]=0;
      }
      else
      {
				_rx_buffer[port][0]=__RLSerial_Protocol_Head1;
				_rx_buffer[port][1]=__RLSerial_Protocol_Head2;
				_rx_buffer[port][2]=data;
				_protocol_flag[port]=3;
				_protocol_sum[port]+=data;
      }
    }break;
    case 3:
    {
      if(_rx_buffer[port][2]<=(_rx_len[port]))
      {
				if( _protocol_sum[port]==_rx_buffer[port][_rx_len[port]-1])
				{
					_is_protocol[port]=1;
					_is_write_buf[port]=1;
					for(int i=0;i<_rx_buffer[port][2];i++)
					{
						_rxprotocol_buffer[port][i]=_rx_buffer[port][i];
					}
					_is_write_buf[port]=0;
					_RLSerial_ClearTime[port]=millis()+_RLSerial_CLEAR_TIME_MS;
				}
				_protocol_sum[port]=0;
        _protocol_flag[port]=0;
				_rx_len[port]=0;
      }
      else 
      {
        _protocol_sum[port]+=data;
      }
    }break;
  }
}
#if RLSERIAL_PORT_SIZE==4
ISR(USART0_RX_vect)
{
	uint8_t data=UDR0;
	_USARTX_RX_ISR(_RLSerial0,data);
}
#endif
ISR(USART1_RX_vect)
{
	uint8_t data=UDR1;
	_USARTX_RX_ISR(_RLSerial1,data);
}
ISR(USART2_RX_vect)
{
	uint8_t data=UDR2;
	_USARTX_RX_ISR(_RLSerial2,data);
}
ISR(USART3_RX_vect)
{
	uint8_t data=UDR3;
	_USARTX_RX_ISR(_RLSerial3,data);
}