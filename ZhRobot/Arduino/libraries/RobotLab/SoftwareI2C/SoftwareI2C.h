#ifndef SoftwareI2C_H
#define SoftwareI2C_H

#include <inttypes.h>

#define HARDWAREI2C_MODE	0
#define SOFTWAREI2C_MODE	1

 extern unsigned char SoftwareI2CFlag;
class SoftwareI2C{

private:
  // per object data
  uint8_t _sdaPin;
  uint8_t _sclPin;
  uint8_t _sdaBitMask;
  uint8_t _sclBitMask;
  volatile uint8_t *_sdaPortReg;
  volatile uint8_t *_sclPortReg;
  volatile uint8_t *_sdaDirReg;
  volatile uint8_t *_sclDirReg;

  uint8_t usePullups;
  
  // private methods
  void setPins(uint8_t sdaPin, uint8_t sclPin, uint8_t usePullups);
  void i2c_writebit( uint8_t c );
  uint8_t i2c_readbit(void);
  void i2c_init(void);
  void i2c_start(void);
  void i2c_repstart(void);
  void i2c_stop(void);
  uint8_t i2c_write( uint8_t c );
  uint8_t i2c_read( uint8_t ack );
  
public:
  // public methods
  SoftwareI2C(uint8_t sdaPin=13, uint8_t sclPin=11);
  SoftwareI2C(uint8_t sdaPin, uint8_t sclPin, uint8_t usePullups);

  uint8_t beginTransmission(uint8_t address);
  uint8_t beginTransmission(int address);
  uint8_t endTransmission(void);
  uint8_t send(uint8_t);
  void send(uint8_t*, uint8_t);
  void send(int);
  void send(char*);
  void write(int data);
	
  uint8_t requestFrom(int address);
  uint8_t requestFrom(uint8_t address);
  uint8_t receive( uint8_t ack );
  uint8_t receive();
  uint8_t receiveLast();
};
#endif//SoftwareI2C_H