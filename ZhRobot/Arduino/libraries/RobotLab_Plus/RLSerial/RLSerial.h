#ifndef Serial_h
#define Serial_h

#include <inttypes.h>

#include "Stream.h"

#define RLSERIAL_PORT_SIZE 3//4
#define RLSERIAL_RX_BUFFER_SIZE 64

#define __RLSerial_Protocol_Head1 0xAA
#define __RLSerial_Protocol_Head2 0x55
class RLSerial
{
public:
	RLSerial(uint8_t port) ;
	void begin(uint8_t port,unsigned long baud);
	int read(uint8_t port);
  void write(uint8_t port,uint8_t data);
	void sendstring(uint8_t port,uint8_t *str);
	void rxmode(uint8_t port,uint8_t mode);
	bool readprotocolflag(uint8_t port);
	void clearprotocolflag(uint8_t port);
	uint8_t readprotocol(uint8_t port,uint8_t n);
	void clearprotocol(uint8_t port);
	void receivestate(uint8_t port,uint8_t state);
	uint8_t writebufferstate(uint8_t port);
	unsigned char *getprotocolhandle(uint8_t port);
	unsigned char getleartime(uint8_t port);
	void setcleartime(uint8_t port);
};

extern volatile unsigned char _rxprotocol_buffer[RLSERIAL_PORT_SIZE][RLSERIAL_RX_BUFFER_SIZE];
extern volatile unsigned long _RLSerial_ClearTime[RLSERIAL_PORT_SIZE];
#endif
