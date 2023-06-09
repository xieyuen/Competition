#ifndef RobotLab_Plus_h
#define RobotLab_Plus_h

#include <Arduino.h>
#include <Servo.h>
//#include "./LEDMatrix/LEDMatrix.h"
//#include"./SoftwareI2C/SoftwareI2C.h"

#define _TYPE_EYE     1
#define _TYPE_COMPASS 2

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
#define _M3_ 		3
#define _M4_ 		4

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


//内置端口定义
#define _MIC_		8//A8 音量检测
#define _BAT_		9//A9 电压检测
//蜂鸣器
#define _BUZZER_  13//d13
//电磁阀开关
#define _ELECSWITCH_  44//D44电磁阀
//按键
#define _B1_  40//D40
#define _B2_  41//D41
#define _B3_  38//D38
//马达
#define M1_PORT_P 2//D2
#define M1_PORT_N 3//D3
#define M2_PORT_P 4//D4
#define M2_PORT_N 5//D5
#define M3_PORT_P 45//D45
#define M3_PORT_N 46//D46

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

typedef struct
{
	uint8_t head1;
	uint8_t head2;
	uint8_t len;
	uint8_t type;
	uint16_t angle;
	int16_t xraw;
	int16_t yraw;
	uint8_t sum;
}Compass_Protocol_Typedef;

typedef struct
{//PHEAD1+PHEAD2+LEN+type+mode+MAXCH+MINCH+ADC_CH1+ADC_CH2+ADC_CH3+ADC_CH4+ADC_CH5+ADC_CH6+ADC_CH7+SUM
	uint8_t head1;
	uint8_t head2;
	uint8_t len;
	uint8_t type;
	uint8_t mode;
	uint8_t chmax;
	uint8_t chmin;
	uint8_t chadc[7];
	uint8_t sum;
}Eye_Protocol_Typedef;

extern unsigned char SoftwareI2CFlag;

float GetUltrasonicSensor(uint8_t EchoPin,uint8_t TrigPin);
float GetUltrasonic(uint16_t pin);
int GetIRKey(int pin,unsigned long num);
void SetBuzzer(uint8_t pin, uint16_t frequency, uint32_t duration);
void SetLed(uint8_t pin,char state);
uint32_t GetIRReceiverData(int pin);
void pinModeA(uint8_t pin, uint8_t mode);
void digitalWriteA(uint8_t pin, uint8_t val);
int digitalReadA(uint8_t pin);
int GetMic(uint8_t pin);
int GetAdc(uint8_t pin);//读取模拟值 模拟量
int GetLightSensor(uint8_t pin);
float GetUltrasonicSensor(uint8_t EchoPin,uint8_t TrigPin);
uint32_t GetIRReceiver(int pin);
void SetLed(uint8_t pin,char state);
void SetBuzzer(uint8_t pin, uint16_t frequency, uint32_t duration);//设置无源蜂鸣器, frequency蜂鸣器频率 ,duration 蜂鸣器响的时间,单位1ms
void SetServo(uint8_t pin,int angle);
int Servo_Deinit(uint8_t pin);
void IRrecv_DeInit(int recvpin);
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

//LCD1602
void SetLcdClear(int port);
void SetLcdString(int port,unsigned char x,unsigned char y,unsigned char *s);
void SetLcdsetBacklight(int port,uint8_t b);
void SetLcdNum(int port,unsigned char x,unsigned char y,long num);
void SetLcdNumBits(int port,unsigned char x,unsigned char y,long num,char bits);

//竞赛板
//12C oled
void Oled_Init(void);
void SetOledClear(void);
void SetOledString(unsigned char x,unsigned char y,unsigned char *s);
void SetOledNum(unsigned char x,unsigned char y,long num);
void SetOledNumBits(unsigned char x,unsigned char y,long num,char bits);
void SetOledImage(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char *BMP);

void _delay(float t);//秒级延时 ,t最小单位为0.001
void delayms(unsigned long t);//毫秒级延时 ,单位1ms
void delayus(unsigned int t);//微秒级延时 ，单位1us
void delays(float t);//秒级延时 ,t最小单位为0.001
unsigned long GetTime(void);//读取系统时间,单位1ms
void SetIO(uint8_t pin,char state);//设置IO状态,state=0 输出低电平,1输出高电平
void SetTimeZero(void);//系统时间清零
int GetIO(uint8_t pin);//读取IO状态 数字量
void SetMotor(uint8_t pin,int speed);//设置马达速度 pin=1~3 ;其中1,2为大功率输出,3为小功率输出,速度Speed=-100~100
void SetInBuzzer(uint16_t frequency, uint32_t duration);//设置内置蜂鸣器, frequency蜂鸣器频率 ,duration 蜂鸣器响的时间,单位1ms
int GetCompass(unsigned char port);//读取指南针角度 port=1-3;接串口1-3
void RLSerial_Init(unsigned char port,unsigned long baud);
//读取复眼
//port=1-3:接串口1-3
//cmd=0~9:0=工作模式mode[0调制模式,1普通模式],1=最大通道值,2=最小通道值,3-9=通道1-7模拟值
int GetEye(unsigned char port,unsigned char cmd);
//设置复眼工作模式
//mode=0~1:0调制模式,1普通模式
int SetEye(unsigned char port,unsigned char mode);
void SetSolenoidValve(unsigned char pulse);//电磁阀控制 pulse=0，松开电磁阀,1~100打开电磁阀，相当于电磁阀力度，根据不同电磁阀调节
int GetInPower(void);//读取电池电压
int GetInMic(void);//读取内置声音传感器 模拟量 返回范围0~1023	
int GetButton(uint8_t pin);//读取按键状态 数字量 pin=1~3 对应B1~B3
#endif
