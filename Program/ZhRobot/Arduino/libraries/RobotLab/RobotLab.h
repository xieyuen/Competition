#ifndef RobotLab_h
#define RobotLab_h

#include <Arduino.h>
#include <Servo.h>
//#include "./LEDMatrix/LEDMatrix.h"
//#include"./SoftwareI2C/SoftwareI2C.h"

#define _A0_ 0
#define _A1_ 1
#define _A2_ 2
#define _A3_ 3
#define _A4_ 4
#define _A5_ 5
#define _A6_ 6
#define _A7_ 7

#define _D0_ 0
#define _D1_ 1
#define _D2_ 2
#define _D3_ 3
#define _D4_ 4
#define _D5_ 5
#define _D6_ 6
#define _D7_ 7
#define _D8_ 8
#define _D9_ 9
#define _D10_ 10
#define _D11_ 11
#define _D12_ 12
#define _D13_ 13

#define _M1_ 		1
#define _M2_ 		2

#define _D9A0_ 		1
#define _D10A1_ 	2
#define _D11A2_ 	3
#define _D13A3_ 	4
#define _A4A5_ 		45
#define _D0D1_ 		46
#define _D11D13_ 	44//0x2c

#define _DA0_ 		100
#define _DA1_ 		101
#define _DA2_ 		102
#define _DA3_ 		103
#define _DA4_ 		104
#define _DA5_ 		105

#define _IIC0_    _A4A5_
#define _IIC1_    _D11D13_
#define _UART_    _D0D1_
//红外
// Values for decode_type
#define TYPE_HASH 			  0//任意解码值，32bit
#define TYPE_NEC 					1//*
#define TYPE_SONY 				2//*
#define TYPE_RC5 					3//*
#define TYPE_RC6 					4//*
#define TYPE_PANASONIC 		5//7//*
#define TYPE_JVC 					6//8//*
#define TYPE_SANYO 				7//9//*
#define TYPE_MITSUBISHI 	8//10//*
#define TYPE_JVC_RE 			9
#define TYPE_UNKNOWN 		 -1

#define ABS(X) ((X)<0?-(X):(X))

extern unsigned char SoftwareI2CFlag;

float GetUltrasonicSensor(uint8_t EchoPin,uint8_t TrigPin);
float GetUltrasonic(uint16_t pin);
int GetIRKey(int pin,unsigned long num);
void SetBuzzer(uint8_t pin, uint16_t frequency, uint32_t duration);
void SetLed(uint8_t pin,char state);
int GetButton(uint8_t pin);
uint32_t GetIRReceiverData(int pin);
void pinModeA(uint8_t pin, uint8_t mode);
void digitalWriteA(uint8_t pin, uint8_t val);
int digitalReadA(uint8_t pin);
int GetButton(uint8_t pin);
int GetMic(uint8_t pin);
int GetAdc(uint8_t pin);
int GetLightSensor(uint8_t pin);
float GetUltrasonicSensor(uint8_t EchoPin,uint8_t TrigPin);
uint32_t GetIRReceiver(int pin);
void SetMotor(uint8_t pin,int speed);
void SetLed(uint8_t pin,char state);
void SetBuzzer(uint8_t pin, uint16_t frequency, uint32_t duration);
void SetServo(uint8_t pin,int angle);
int Servo_Deinit(uint8_t pin);
void IRrecv_DeInit(int recvpin);
void _delay(float t);
void delayms(unsigned long t);
void delayus(unsigned int t);
void delays(float t);
unsigned long GetTime(void);
void SetIO(uint8_t pin,char state);
void SetTimeZero(void);
int GetIO(uint8_t pin);
void SetIRData(uint8_t pin,unsigned long data);
int GetIRDecodeType(int pin);
long GetEncoder(uint8_t pin);
void SetEncoder(uint8_t pin);
void setEncoderMotorAngle(char port,int speed,long code);
float GetTimeSec(void);
float GetDHT(char port,int type);
//void Printf(const char* fmt, ...);
unsigned char *GetLEDMatrixBuffer(uint8_t *displaybuffer);
//LED点阵显示一点
void SetLEDMatrixXY(int port,uint8_t x,uint8_t y,uint8_t state);
//LED点阵全屏显示
void SetLEDMatrix(int port,uint8_t x,uint8_t y,uint8_t *displaybuffer);
//LED点阵清屏
void SetLEDMatrixClear(int port);
//LED点阵显示亮度
void SetLEDMatrixBrightness(int port,uint8_t b);
//LED点阵闪烁频率
void SetLEDMatrixBlink(int port,uint8_t b);
//LED点阵显示1列
void SetLEDMatrixCom(int port,uint8_t x,uint8_t data);
extern const unsigned char Ascii[95][8];
//4位数码管
void Set7SegDisplayNum(int port,int num);//显示4位数字
void Set7SegDisplayClear(int port);//全部清除
void Set7SegDisplayDot(int port,bool aState);//显示点
void Set7SegDisplayChar(int port,char pos,unsigned char c);//显示一个数字
void Set7SegDisplayBrightness(int port,uint8_t b);//亮度控制
void Set7SegDisplayTick(int port,int tick1,int tick2);//时钟模块


void SetLcdClear(int port);
void SetLcdString(int port,unsigned char x,unsigned char y,unsigned char *s);
void SetLcdsetBacklight(int port,uint8_t b);
void SetLcdNum(int port,unsigned char x,unsigned char y,long num);
void SetLcdNumBits(int port,unsigned char x,unsigned char y,long num,char bits);

void SetFan(uint8_t pin,int speed);//风扇模块
float GetAhrs(int port,int cmd);
int Get24GByte(void);
void Set24GByte(int data);
void Set7SegDisplayAscii(int port,char pos,unsigned char *c);
#endif
