#include <Arduino.h>
#include "RobotLab_Plus.h"
#include "./IRremote/RLIRremote.h"
//#include "./print/print.h"
// Provides ISR
#include <avr/interrupt.h>
#include "wiring_private.h"
#include "pins_arduino.h"
//#include "./TM1650/TM1650.h"
#include "./DHT11/DHT11.h"
#include "./LEDMatrix/LEDMatrix.h"
#include "./SoftwareI2C/SoftwareI2C.h"
#include "./LED7Segment/LED7Segment.h"
#include "./LCD1602/LCD1602.h"
#include "./OLED_I2C/OLED_I2C.h"
#include "./RLSerial/RLSerial.h"
//ģ��˿�ת�����ֶ˿�
//ģ��˿�ת�����ֶ˿�
//pin:0-5=A0-A5 PC0-PC5
void pinModeA(uint8_t pin, uint8_t mode)//A0-A5��ʼ��IOģʽ
{
	if (pin >5) return;
	if (mode == INPUT) { 	DDRC&=~(1<<pin);	} 
	else if (mode == INPUT_PULLUP) 
	{
		DDRC&=~(1<<pin);
		PORTC|=(1<<pin);
	} 
	else 	{		DDRC|=(1<<pin);	}
}

void digitalWriteA(uint8_t pin, uint8_t val)//����A0-A5���״̬
{
	if (pin >5) return;
	if (val == LOW) 	{		PORTC&=~(1<<pin);	} 
	else {		PORTC|=(1<<pin);	}

}
int digitalReadA(uint8_t pin)//��ȡA0-A5����������ź�״̬
{
	if (pin >5) return LOW;
	if (PINC&(1<<pin)) return HIGH;
	return LOW;
}
unsigned long pulseInA(uint8_t pin, uint8_t state, unsigned long timeout)
{
	uint8_t bit=1<<pin;
	uint8_t stateMask = (state ?  bit: 0);
  if (pin >5) return 0;
	
	unsigned long maxloops = microsecondsToClockCycles(timeout)/16;

	unsigned long width = countPulseASM(portInputRegister(3), bit, stateMask, maxloops);

	// prevent clockCyclesToMicroseconds to return bogus values if countPulseASM timed out
	if (width)		return clockCyclesToMicroseconds(width * 16 + 16);
	else		return 0;
}
//ģ���
void _delay(float t){delay(t*1000);}//�뼶��ʱ
void delays(float t){delay(t*1000);}//�뼶��ʱ
void delayms(unsigned long t){delay(t);}//���뼶��ʱ
void delayus(unsigned int t){delayMicroseconds(t);}//���뼶��ʱ
int GetIO(uint8_t pin)//��ȡIO״̬ ������
{
	/*if(pin>=_DA0_&&pin<=_DA5_)
	{
		pinModeA(pin-_DA0_,INPUT);
		return digitalReadA(pin-_DA0_);
	}
	if(pin>13||pin<2)return 0;//*/
	pinMode(pin,INPUT);
	return digitalRead(pin);
}
int GetButton(uint8_t pin)//��ȡ����״̬ ������ pin=1~3 ��ӦB1~B3
{
	switch(pin)
	{
		case 1:return GetIO(_B1_);break;
		case 2:return GetIO(_B2_);break;
		case 3:return GetIO(_B3_);break;
	}
	return GetIO(pin);
}
int GetInMic(void)//��ȡ�������������� ģ���� ���ط�Χ0~1023	
{
	return analogRead(_MIC_);
}
int GetMic(uint8_t pin)//��ȡ���������� ģ����
{
	//if(pin>15)return 0;
	return analogRead(pin);
}
int GetSound(uint8_t pin)//��ȡ���������� ģ����
{
	//if(pin>7)return 0;
	return analogRead(pin);
}
int GetAdc(uint8_t pin)//��ȡ��紫����/ģ��ֵ ģ����
{
	//if(pin>7)return 0;
	return analogRead(pin);
}
int GetLightSensor(uint8_t pin)//��ȡ��紫����/ģ��ֵ ģ����
{
	//if(pin>7)return 0;
	return analogRead(pin);
}
float GetUltrasonicSensorD(uint8_t EchoPin,uint8_t TrigPin)//��ȡ������������ ������
{
	float distance;
	pinMode(TrigPin, OUTPUT);
	pinMode(EchoPin, INPUT); 
	// ����һ��10us�ĸ�����ȥ����TrigPin
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  // ��������ȣ������������
  distance = pulseIn(EchoPin, HIGH) / 58.00;
	if(distance>300)distance=300;
	return distance;
}
float GetUltrasonicSensor(uint8_t EchoPin,uint8_t TrigPin)//��ȡ������������ ������
{
	float distance;
	pinModeA(TrigPin, OUTPUT);
	pinMode(EchoPin, INPUT); 
	// ����һ��10us�ĸ�����ȥ����TrigPin
  digitalWriteA(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWriteA(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWriteA(TrigPin, LOW);
  // ��������ȣ������������
  distance = pulseIn(EchoPin, HIGH) / 58.00;
	if(distance>300)distance=300;
	return distance;
}
float GetUltrasonicA4A5(void)//��ȡ������������ ������
{
	uint8_t EchoPin=5;
	uint8_t TrigPin=4;
	float distance;
	pinModeA(TrigPin, OUTPUT);
	pinModeA(EchoPin, INPUT); 
	// ����һ��10us�ĸ�����ȥ����TrigPin
  digitalWriteA(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWriteA(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWriteA(TrigPin, LOW);
  // ��������ȣ������������
  distance = pulseInA(EchoPin, HIGH,1000000L) / 58.00;
	if(distance>300)distance=300;
	return distance;
}
float GetUltrasonic(uint16_t pin)//��ȡ������������ ������
{
	uint8_t EchoPin=11;
	uint8_t TrigPin=13;
	switch(pin)
	{
		case _D9A0_:EchoPin=9;TrigPin=0;break;
		case _D10A1_:EchoPin=10;TrigPin=1;break;
		case _D11A2_:EchoPin=11;TrigPin=2;break;
		case _D13A3_:EchoPin=13;TrigPin=3;break;
		case _D11D13_:EchoPin=11;TrigPin=13;
		      return GetUltrasonicSensorD(EchoPin,TrigPin);break;
		case _A4A5_:return GetUltrasonicA4A5();break;
		default :return 0;break;
	}
	return GetUltrasonicSensor(EchoPin,TrigPin);
}//*/
/*9
float GetUltrasonic(uint16_t pin)//��ȡ������������ ������
{
	uint8_t EchoPin=5;
	uint8_t TrigPin=4;
	float distance;
	pinModeA(TrigPin, OUTPUT);
	pinModeA(EchoPin, INPUT); 
	// ����һ��10us�ĸ�����ȥ����TrigPin
  digitalWriteA(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWriteA(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWriteA(TrigPin, LOW);
  // ��������ȣ������������
  distance = pulseInA(EchoPin, HIGH,1000000L) / 58.00;
	if(distance>300)distance=300;
	return distance;
}//*/
IRrecv _irrecv;
unsigned long _IRRecvData=0;
unsigned long _IRRecvData2=0;
uint32_t _IRRecvDataClearTime=0;
int _ir_decode_type=-1;
uint32_t GetIRReceiver(int pin)//��ȡ38k�������ģ�� ������
{
	if(pin>13||pin<2)return 0;
	unsigned long value=_IRRecvData;
	decode_results _ir_results;
	if(IRrecv_Init(pin)==1)
	{
		IRrecv();
		_irrecv.enableIRIn(); // Start the receiver
	}
	if (_irrecv.decode(&_ir_results)) 
	{
    //Serial.println(_ir_results.value, HEX);
		//value=_ir_results.value;
		_ir_decode_type=_ir_results.decode_type;
		if(_ir_results.value==REPEAT)//�ظ�ָ��
		{
			value=_IRRecvData2;//_IRRecvData;
		}
		else
		{
			value=_ir_results.value;
			_IRRecvData=_ir_results.value;
			_IRRecvData2=_ir_results.value;
			
		}
		_IRRecvDataClearTime=millis()+30;//25
		//Serial.print("REV:0X");
		//Serial.println(value, HEX);
		//Printf("data=%X, %X\r\n",value,_IRRecvData);
    _irrecv.resume(); // Receive the next value
  }
		//	Serial.print("REV:0X");
		//if(value)Serial.println(value, HEX);
	return value;
}
uint32_t GetIRReceiverData(int pin)
{
	unsigned long value=GetIRReceiver(pin);
	if(value>0)
	{
		_IRRecvData=0;
	}
	return value;
}
/*
#define TYPE_NEC 					1//*
#define TYPE_SONY 				2//*
#define TYPE_RC5 					3//*
#define TYPE_RC6 					4//*
#define TYPE_PANASONIC 		5//7//*
#define TYPE_JVC 					6//8//*
#define TYPE_SANYO 				7//9//*
#define TYPE_MITSUBISHI 	8//10//*
#define TYPE_UNKNOWN 		 -1
*/
#define IR_DEBUG //Serial.println
int GetIRDecodeType(int pin)//���һ��ң�ؽ�������
{
	switch(_ir_decode_type)
	{
		case	NEC:		IR_DEBUG("decode_type:NEC");		return TYPE_NEC;		break;
		case	SONY:		IR_DEBUG("decode_type:SONY");		return TYPE_SONY;		break;
		case	RC5:		IR_DEBUG("decode_type:RC5");		return TYPE_RC5;		break;
		case	RC6:		IR_DEBUG("decode_type:RC6");		return TYPE_RC6;		break;
		case	PANASONIC:		IR_DEBUG("decode_type:PANASONIC");		return TYPE_PANASONIC;		break;
		case	JVC:		IR_DEBUG("decode_type:JVC");		return TYPE_JVC;		break;
		case	SANYO:		IR_DEBUG("decode_type:SANYO");		return TYPE_SANYO;		break;
		case	MITSUBISHI:		IR_DEBUG("decode_type:MITSUBISHI");		return TYPE_MITSUBISHI;		break;
		case	UNKNOWN:		IR_DEBUG("decode_type:UNKNOWN");		return TYPE_UNKNOWN;		break;
		default:return _ir_decode_type; break;
	}
}
/*
ң��1
FFA25D : CH-  10
FF629D : CH   11
FFE21D : CH+  12
FF22DD : <<   13
FF02FD : >>   14
FFC23D : >||  15
FFE01F : -    16
FFA857 : +    17
FF906F : EQ   18
FF6897 : 0    0
FF9867 : 100+ 19
FFB04F : 200+ 20
FF30CF : 1    1
FF18E7 : 2    2
FF7A85 : 3    3
FF10EF : 4    4
FF38C7 : 5    5
FF5AA5 : 6    6
FF42BD : 7    7
FF4AB5 : 8    8
FF52AD : 9    9
ң��2
FFA25D : 1    1
FF629D : 2    2
FFE21D : 3    3
FF22DD : 4    4
FF02FD : 5    5
FFC23D : 6    6
FFE01F : 7    7
FFA857 : 8    8
FF906F : 9    9
FF6897 : *    11
FF9867 : 0    0
FFB04F : #    12
FF18E7 : /\   13
FF4AB5 : \/   14
FF10EF : <    15 
FF5AA5 : >    16
FF38C7 : OK   17
*/
int readIRRemoteKey(uint32_t num)
{
	switch(num)
	{
		/*case 0xFFA25D:return 10;break;//CH-
		case 0xFF629D:return 11;break;//CH
		case 0xFFE21D:return 12;break;//CH+
		case 0xFF22DD:return 13;break;//|<<
		case 0xFF02FD:return 14;break;//>>|
		case 0xFFC23D:return 15;break;//>||
		case 0xFFE01F:return 16;break;//-
		case 0xFFA857:return 17;break;//+
		case 0xFF906F:return 18;break;//EQ
		case 0xFF6897:return 0; break;//0
		case 0xFF9867:return 19;break;//100+
		case 0xFFB04F:return 20;break;//200+
		case 0xFF30CF:return 1; break;//1
		case 0xFF18E7:return 2; break;//2
		case 0xFF7A85:return 3; break;//3
		case 0xFF10EF:return 4; break;//4
		case 0xFF38C7:return 5; break;//5
		case 0xFF5AA5:return 6; break;//6
		case 0xFF42BD:return 7; break;//7
		case 0xFF4AB5:return 8; break;//8
		case 0xFF52AD:return 9; break;//9//*/
		case 0XFFA25D : return  1;break;//1    1
		case 0XFF629D : return  2;break;//2    2
		case 0XFFE21D : return  3;break;//3    3
		case 0XFF22DD : return  4;break;//4    4
		case 0XFF02FD : return  5;break;//5    5
		case 0XFFC23D : return  6;break;//6    6
		case 0XFFE01F : return  7;break;//7    7
		case 0XFFA857 : return  8;break;//8    8
		case 0XFF906F : return  9;break;//9    9
		case 0XFF6897 : return 11;break;//*    11
		case 0XFF9867 : return  0;break;//0    0
		case 0XFFB04F : return 12;break;//#    12
		case 0XFF18E7 : return 13;break;///\   13
		case 0XFF4AB5 : return 14;break;//\/   14
		case 0XFF10EF : return 15;break;//<    15 
		case 0XFF5AA5 : return 16;break;//>    16
		case 0XFF38C7 : return 17;break;//OK   17
	}
	return -1;
}
int GetIRKey(int pin,unsigned long num)
{
	if(pin>13||pin<2)return 0;
	if(_IRRecvDataClearTime>0&&_IRRecvDataClearTime<=millis())
	{
		_IRRecvDataClearTime=0;
		_IRRecvData=0;
	}
	if(readIRRemoteKey(GetIRReceiver(pin))==num)
	{
		//Serial.println(_IRRecvData, HEX);
		//Printf("num=%03d, %X\r\n",num,_IRRecvData);
		//_IRRecvData=0;
		return 1;
	}
	else
	{

	}
	return 0;
}
volatile long _encoder[2]={0,0};
volatile uint8_t Encoder_Init_Flag=0;
volatile long _encode_target[2]={0,0};
volatile int _encode_mode[2]={0,0};
void _Motor(uint8_t pin,int speed);
#define ecmotor_speed_min  5
#define ecmotor_speed_max  30
#define ecmotor_tcoder_max 50//30
#define ecmotor_tcoder_min 9
void Encoder_RunAngle(uint8_t port)
{
	uint8_t p=port-1;
	int speed;
 if(_encode_mode[p]!=1)return ;
 	if((_encoder[p]>=(_encode_target[p]-ecmotor_tcoder_min))&&(_encoder[p]<=(_encode_target[p]+ecmotor_tcoder_min)))
 	{
 		_Motor(port,0);
 	}
  else if((_encoder[p]<(_encode_target[p]-ecmotor_tcoder_min))&&(_encoder[p]>(_encode_target[p]-ecmotor_tcoder_max)))
	{
		speed=ABS(_encode_target[p]-_encoder[p])*(ecmotor_speed_max-ecmotor_speed_min)/(ecmotor_tcoder_max-ecmotor_tcoder_min)+ecmotor_speed_min;
		//if(speed<ecmotor_speed_min)speed=ecmotor_speed_min;
		_Motor(port,speed);
		//_Motor(port,38);
	}
	else if((_encoder[p]>(_encode_target[p]+ecmotor_tcoder_min))&&(_encoder[p]<(_encode_target[p]+ecmotor_tcoder_max)))
	{
		speed=ABS(_encode_target[p]-_encoder[p])*(ecmotor_speed_max-ecmotor_speed_min)/(ecmotor_tcoder_max-ecmotor_tcoder_min)+ecmotor_speed_min;
		//if(speed<ecmotor_speed_min)speed=ecmotor_speed_min;
		_Motor(port,-speed);
		//_Motor(port,-38);
	}
}
#define GETPINSTATE(PINX,BIT)	(PINX>>BIT)&0x01//((PINX&BIT)?1:0)//
#define GETENCODERCOUNT(EC1,EC2,port)	if(((EC1)^(EC2))){_encoder[port]--;}else{_encoder[port]++;}
void Encoder_ISR0 (void)//PD2/PD4
{
	//if((GETPINSTATE(PIND,0b00000100)^GETPINSTATE(PIND,0b00010000))){_encoder[0]++;}else{_encoder[0]--;}
	//if((PIND&0b00000100^PIND&0b00010000)){_encoder[0]++;}else{_encoder[0]--;}
	if((GETPINSTATE(PIND,2)^GETPINSTATE(PIND,4))){_encoder[0]++;}else{_encoder[0]--;}
	//GETENCODERCOUNT(GETPINSTATE(PIND,2),GETPINSTATE(PIND,4),0);
	Encoder_RunAngle(1);
}
void Encoder_ISR1 (void)
{
	//if((GETPINSTATE(PIND,0b00001000)^GETPINSTATE(PIND,0b10000000))){_encoder[1]++;}else{_encoder[1]--;}
	//if((PIND&0b00001000^PIND&0b10000000)){_encoder[1]++;}else{_encoder[1]--;}
	if((GETPINSTATE(PIND,3)^GETPINSTATE(PIND,7))){_encoder[1]--;}else{_encoder[1]++;}
	//GETENCODERCOUNT(GETPINSTATE(PIND,3),GETPINSTATE(PIND,7),1);
	Encoder_RunAngle(2);
}
void Encoder_Init(void)
{
	if(Encoder_Init_Flag)return;Encoder_Init_Flag=1;
	pinMode(2, INPUT_PULLUP);pinMode(3, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(2), Encoder_ISR0, CHANGE);//��int.0��ƽ�ı�ʱ,�����жϺ���Encoder_ISR0
	attachInterrupt(digitalPinToInterrupt(3), Encoder_ISR1, CHANGE);//��int.1��ƽ�ı�ʱ,�����жϺ���Encoder_ISR1
}
long GetEncoder(uint8_t pin)//��ȡ����ֵ
{
	Encoder_Init();
	if(pin==0||pin>2)return 0;
	return _encoder[pin-1];
}
void SetEncoder(uint8_t pin)//����ֵ����
{
	Encoder_Init();
	if(pin==0||pin>2)return ;
	_encoder[pin-1]=0;	
}

void setEncoderMotorAngle(char port,int speed,long code)
{
	char p;
	if(port==0||port>2)return ;
	p=port-1;
	Encoder_Init();
	if(_encode_mode[p]>1)_encode_mode[p]=1;
	if(_encode_mode[p]!=1)
	{
		_encode_target[p] = 0;
		_encoder[p] = 0;
		_encode_mode[p]=1;
	}

	if((int)speed>=0)
	{
		_encode_target[p] +=(long)code;
	}
	else
	{
		_encode_target[p] -=(long)code;
	}
	_Motor(port,(int)speed);
}
IRsend _irsend;
void SetIRData(uint8_t pin,unsigned long data)
{
	IRsend_Init(pin);
	_irsend.sendNEC(data, 32);
}//*/

#define SPEED_NPULSE 0//255
int _motorspeedlast[3]={0,0,0};
void _Motor(uint8_t pin,int speed)//��������ٶ� pin=1~3 ;����1,2Ϊ�������,3ΪС�������,�ٶ�Speed=-100~100
{
  unsigned int pulse=speed; 
  if(speed>=100)speed=99;if(speed<=-100)speed=-99;
  pulse=ABS(speed)*255/100;
  switch(pin)
  {
	  case 1://M1 ��������
    { 
      if(_motorspeedlast[0]==speed)return ;_motorspeedlast[0]=speed;
      if(speed>=0)
      {
        analogWrite(M2_PORT_P,pulse);
        analogWrite(M2_PORT_N,SPEED_NPULSE);
      }
      else
      {
        analogWrite(M2_PORT_N,pulse);
        analogWrite(M2_PORT_P,SPEED_NPULSE);
      }
    }break;
    case 2://M2 ��������
    {
      if(_motorspeedlast[1]==speed)return ;_motorspeedlast[1]=speed;
      if(speed>=0)
      {
        analogWrite(M1_PORT_P,pulse);
        analogWrite(M1_PORT_N,SPEED_NPULSE);
      }
      else
      {
        analogWrite(M1_PORT_N,pulse);
        analogWrite(M1_PORT_P,SPEED_NPULSE);
      }
     
    }break;
    case 3://M3 С��������
    {
      if(_motorspeedlast[2]==speed)return ;_motorspeedlast[2]=speed;
      if(speed>=0)
      {
        analogWrite(M3_PORT_P,pulse);
        analogWrite(M3_PORT_N,SPEED_NPULSE);//SPEED_NPULSE
      }
      else
      {
        analogWrite(M3_PORT_N,pulse);
        analogWrite(M3_PORT_P,SPEED_NPULSE);//SPEED_NPULSE
      }
    }break;
  }
}
void SetMotor(uint8_t pin,int speed)
{
	if(pin==0||pin>3)return ;
	if(pin<=2)_encode_mode[pin-1]=0;
	_Motor(pin,speed);
}
//D2 D3 D4 D5 D10 D11 D12 D13 D44 D45 D46 
void SetPWM(uint8_t pin,int speed)
{
	analogWrite(pin,speed);//0~255
}
void SetIO(uint8_t pin,char state)//����IO״̬
{
	pinMode(pin,OUTPUT);
	digitalWrite(pin, state);
}
void SetLed(uint8_t pin,char state)//����LED״̬
{
	SetIO(pin,state);
}

void SetBuzzer(uint8_t pin, uint16_t frequency, uint32_t duration)//���÷�����
{
  int period = 1000000L / frequency;
  int pulse = period / 2;
	if(pin>13||pin<2)return ;
  pinMode(pin, OUTPUT);
	if(frequency==0)
	{
		digitalWrite(pin, LOW);
		return ;
	}
  for (long i = 0; i < duration * 1000L; i += period) 
  {
    digitalWrite(pin, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(pin, LOW);
    delayMicroseconds(pulse);
   
  }
}
void SetInBuzzer(uint16_t frequency, uint32_t duration)//�������÷�����
{
	SetBuzzer(_BUZZER_,  frequency,  duration);
}
int _servoanglelast[4]={-1,-1,-1,-1};
Servo _myservo[4];  // create servo object to control a servo
void SetServo(uint8_t pin,int angle)//���ö���Ƕ�
{
	if(pin<9||pin>13)return ;
	if(angle==-2){_myservo[pin-9].detach();_servoanglelast[pin-9]=-1;return;}
	if(_servoanglelast[pin-9]==angle)return ;
	if(_servoanglelast[pin-9]==-1)
	{	
		_myservo[pin-9].attach(pin);  // attaches the servo on pin 9 to the servo object
	}
	_myservo[pin-9].write(angle);                  // sets the servo position according to the scaled value
	_servoanglelast[pin-9]=angle;
}
int Servo_Deinit(uint8_t pin)
{
	if(pin<9||pin>13)return 0;
	if(_servoanglelast[pin-9]!=-1){_servoanglelast[pin-9]=-1;_myservo[pin-9].detach();return 1;}
	return 0;
}
unsigned long _LastTime = 0;
unsigned long GetTime(void)//ϵͳʱ��/1ms
{
  return (millis() - _LastTime);
}
void SetTimeZero(void)
{
  _LastTime = millis();
}
float GetTimeSec(void)//ϵͳʱ��/1s
{
  return (float)GetTime()/1000;
}

//8*8LED��ʾ�� ԭ�������½�
//LED������ʾһ��
void SetSoftwareI2CPort(int port,int *scl,int *sda)
{
	//if(port==_A4A5_){*scl=-1;*sda=-1;}
	//else {*scl=11;*sda=13;}
	switch(port)
	{
		case _D0D1_:  {*scl=0;*sda=1;}  break;
		case _D11D13_:{*scl=11;*sda=13;}break;
		case _A4A5_:
		default:     {*scl=-1;*sda=-1;}break;
	}
}
void SetLEDMatrixXY(int port,uint8_t x,uint8_t y,uint8_t state)
{
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LM_init(sda,scl);
	LM_writeDisplayDot(sda,scl,x,y,state);
}
//LED��������
void SetLEDMatrixClear(int port)
{
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LM_init(sda,scl);
	LM_clear(sda,scl);
}
//LED����ȫ����ʾ
unsigned char *GetLEDMatrixBuffer(uint8_t *displaybuffer)
{
  static unsigned char drawBuffer[16];
  memcpy(drawBuffer,displaybuffer,16);
  free(displaybuffer);
  return drawBuffer;
}
void SetLEDMatrix(int port,uint8_t x,uint8_t y,uint8_t *displaybuffer)
{
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LM_init(sda,scl);
	LM_writeDisplay(sda,scl,x,y,displaybuffer);
}
//LED������ʾ1��
void SetLEDMatrixCom(int port,uint8_t x,uint8_t data)
{
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LM_init(sda,scl);
	LM_writeDisplayCom(sda,scl,x,data) ;
}
//LED������ʾ����
void SetLEDMatrixBrightness(int port,uint8_t b)
{
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LM_init(sda,scl);
	LM_setBrightness(sda,scl,b); //LED���� 0~15 0�,15����;
}
//LED������˸Ƶ��
void SetLEDMatrixBlink(int port,uint8_t b)
{
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LM_init(sda,scl);
	LM_blinkRate(sda,scl,b); //led��˸Ƶ�� 0~3 0�ر���˸,1 2HZ,2 1HZ,3 0.5HZ
}
//4λ�����
//TM1650 _LED7Seg4D;
unsigned int TM1650_Init_flag=0;
void TM1650_Init(int port)
{
	if(TM1650_Init_flag)return ;TM1650_Init_flag=1;
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	//_LED7Seg4D.init(sda,scl);
	//delayms(300);
	LED7Segment led7seg;
	led7seg.init(sda,scl);
}
//��ʾ4λ����
void Set7SegDisplayNum(int port,int num)
{
	TM1650_Init(port);
	//_LED7Seg4D.displayNumber4D(num);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LED7Segment led7seg(sda,scl);
	led7seg.displayNumber4D(num);
}
//�����ʾ������
void Set7SegDisplayClear(int port)
{
	TM1650_Init(port);
	//_LED7Seg4D.clear();
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LED7Segment led7seg(sda,scl);
	led7seg.clear();
}
//��ʾ��
void Set7SegDisplayDot(int port,bool aState)
{
	TM1650_Init(port);
	//_LED7Seg4D.setDot(1,  aState);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LED7Segment led7seg(sda,scl);
	led7seg.setDot(2,  aState);
}
//��ʾ1λ
void Set7SegDisplayChar(int port,char pos,unsigned char c)//��ʾһ������,pos=1-4������,c=0~9����
{
	if(c>9)return ;
	if(pos==0||pos>4)return ;
	TM1650_Init(port);
	//_LED7Seg4D.displayChar(pos-1,  c+'0');
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LED7Segment led7seg(sda,scl);
	led7seg.displayChar(pos-1,  c+'0');
}
//���ȿ���
void Set7SegDisplayBrightness(int port,uint8_t b)//b=0~7 7����
{
	TM1650_Init(port);
	//_LED7Seg4D.setBrightness(b);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LED7Segment led7seg(sda,scl);
	led7seg.setBrightness(0,b);
}
/*/��ʾʱ��
void Set7SegDisplayTick(int port,int tick1,int tick2)
{
	TM1650_Init(port);
	_LED7Seg4D.setDot(1,  1);
	_LED7Seg4D.displayChar(0,  ((tick1/10)%10)+'0');
	_LED7Seg4D.displayChar(1,  (tick1%10)+'0');
	_LED7Seg4D.displayChar(2,  ((tick2/10)%10)+'0');
	_LED7Seg4D.displayChar(3,  (tick2%10)+'0');
}
void Set7SegDisplayString(int port,char *aString)
{
	TM1650_Init(port);
	_LED7Seg4D.displayString(aString);
}//*/
//DHT11
HTS_dht11 _DHT;
#define _DEBUG Serial.print
float GetDHT(char port,int type)//type 1ʪ��,2�¶� ,
{
  int chk;
	float data=999.9;
  //_DEBUG("DHT11, \t");
	int s=18;//18
	while(s--)
	{
		chk = _DHT.read(port);    // READ DATA
		//chk= _DHT.readdata(port,type,&data);
		switch (chk)
		{
			case DHTLIB_OK:  
				 //_DEBUG("OK,\t"); 
				 if(type==1)data= _DHT.humidity;
				 else data= _DHT.temperature;
				 break;
			break;
			case DHTLIB_ERROR_CHECKSUM: 
				//_DEBUG("Checksum error,\t"); 
			break;
			case DHTLIB_ERROR_TIMEOUT: 
				//_DEBUG("Time out error,\t"); 
			break;
			default: 
				//_DEBUG("Unknown error,\t"); 
			break;
		}//*/
	}
	//pinMode(port, OUTPUT);
	//digitalWrite(port, HIGH);
 // DISPLAT DATA
 // _DEBUG(DHT.humidity,1);
 // _DEBUG(",\t");
 // _DEBUG(DHT.temperature,1);	
	return data;
}
//��ʼ��
void LCD1602_Init(int port)
{
	if(LCD1602_Init_flag)return ;LCD1602_Init_flag=1;
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LCD1602 lcd;
	lcd.init(sda,scl);
}
//����
void SetLcdClear(int port)
{
	LCD1602_Init(port);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LCD1602 lcd(sda,scl);
	lcd.clear();
}
//��ʾ�ַ���
//x:������,��Χ1-16
//y:������,��Χ1-2
//s:��ʾ���ַ���,ΪӢ���ַ�
void SetLcdString(int port,unsigned char x,unsigned char y,unsigned char *s)
{
	if(x==0||y==0||x>16||y>2)return ;x--;y--;
	LCD1602_Init(port);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LCD1602 lcd(sda,scl);
	lcd.displayString(x,y,s);
}
//Һ������ƿ���
//b=0 ��,1��
void SetLcdsetBacklight(int port,uint8_t b)
{
	LCD1602_Init(port);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LCD1602 lcd(sda,scl);
	lcd.setBacklight(b);
}
//��ʾ10λʮ�����з�������
//��ʾ�ַ���
//x:������,��Χ1-16
//y:������,��Χ1-2
//num:��ʾ������
void SetLcdNum(int port,unsigned char x,unsigned char y,long num)
{
	if(x==0||y==0||x>16||y>2)return ;x--;y--;
	LCD1602_Init(port);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LCD1602 lcd(sda,scl);
	lcd.displayNumber(x, y,num,8);
}
//��ʾʮ�������֣�ָ����ʾλ��
//��ʾ�ַ���
//x:������,��Χ1-16
//y:������,��Χ1-2
//num:��ʾ������
//bits:��ʾ�з���λ������Χ1~10
void SetLcdNumBits(int port,unsigned char x,unsigned char y,long num,char bits)
{
	if(x==0||y==0||x>16||y>2)return ;x--;y--;
	LCD1602_Init(port);
	int scl=-1,sda=-1;
	SetSoftwareI2CPort(port,&scl,&sda);
	LCD1602 lcd(sda,scl);
	lcd.displayNumber(x, y,num,bits);
}

char Oled_Init_flag=0;
//��ʼ��
void Oled_Init(void)
{
	if(Oled_Init_flag)return ;Oled_Init_flag=1;
	OLED_I2C oled(31,30);
	oled.init();
}
//����
void SetOledClear(void)
{
	Oled_Init();
	OLED_I2C oled(31,30);
	oled.clear();
}
//��ʾ�ַ���
//x:������,��Χ1-128
//y:������,��Χ1-8
//s:��ʾ���ַ���,ΪӢ���ַ�
void SetOledString(unsigned char x,unsigned char y,unsigned char *s)
{
	if(x==0||y==0||x>128||y>8)return ;x--;y--;
	Oled_Init();
	OLED_I2C oled(31,30);
	oled.displayString(x,y,s,0);
}

//��ʾ10λʮ�����з�������
//��ʾ�ַ���
//x:������,��Χ1-128
//y:������,��Χ1-8
//num:��ʾ������
void SetOledNum(unsigned char x,unsigned char y,long num)
{
	if(x==0||y==0||x>128||y>8)return ;x--;y--;
	Oled_Init();
	OLED_I2C oled(31,30);
	oled.displayNumber(x, y,num,10|0x80);
}
//��ʾʮ�������֣�ָ����ʾλ��
//��ʾ�ַ���
//x:������,��Χ1-128
//y:������,��Χ1-8
//num:��ʾ������
//bits:��ʾ�з���λ������Χ1~10
void SetOledNumBits(unsigned char x,unsigned char y,long num,char bits)
{
	if(x==0||y==0||x>128||y>8)return ;x--;y--;
	Oled_Init();
	OLED_I2C oled(31,30);
	oled.displayNumber(x, y,num,bits);
}
//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
//--------------------------------------------------------------
void SetOledImage(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char *BMP)
{
	if(x0==0||y0==0||x0>128||y0>8||x1==0||y1==0||x1>128||y1>8)return ;x0--;y0--;x1--;y1--;
	Oled_Init();
	OLED_I2C oled(31,30);
	oled.Image(x0,y0,x1,y1,BMP);
}

unsigned long _RLSerial_Baud[3]={0,0,0};

void RLSerial_Init(unsigned char port,unsigned long baud)
{
	if(_RLSerial_Baud[port]==baud)return ;_RLSerial_Baud[port]=baud;
	RLSerial mserial(port);
	mserial.begin(port,baud);
	_RLSerial_ClearTime[port]=0;
	
}
//��ȡָ����Ƕ�
//port=1-3;�Ӵ���1-3
int GetCompass(unsigned char port)
{
	if(port==0||port>RLSERIAL_PORT_SIZE)return 9999;
#if RLSERIAL_PORT_SIZE==3
	port-=1;
#endif//#if RLSERIAL_PORT_SIZE==3
	int data=9999;
	RLSerial_Init(port,115200);
	RLSerial mserial(port);
	mserial.rxmode(port,1);
	Compass_Protocol_Typedef *protocol_t=(Compass_Protocol_Typedef *)mserial.getprotocolhandle(port);
	while(mserial.writebufferstate(port));//�ȴ�д�뻺�����
	//if(mserial.readprotocolflag(port)&&mserial.readprotocol(port,3)==_TYPE_COMPASS)
	if(mserial.readprotocolflag(port)&&protocol_t->type==_TYPE_COMPASS)
	{
		data=protocol_t->angle;
	}
	if(mserial.getleartime(port))
	{//��ʱ�建��
		mserial.setcleartime(port);
		mserial.clearprotocol(port);
	}
	return data;
}
//��ȡ����
//port=1-3:�Ӵ���1-3
//cmd=0~9:0=����ģʽmode[0����ģʽ,1��ͨģʽ],1=���ͨ��ֵ,2=��Сͨ��ֵ,3-9=ͨ��1-7ģ��ֵ
int GetEye(unsigned char port,unsigned char cmd)//cmd=0~9; 0=mode,1=max,2=min,3-9=adc1-7
{//PHEAD1+PHEAD2+LEN+type+mode+MAXCH+MINCH+ADC_CH1+ADC_CH2+ADC_CH3+ADC_CH4+ADC_CH5+ADC_CH6+ADC_CH7+SUM
	if(port==0||port>RLSERIAL_PORT_SIZE||cmd>9)return 9999;
#if RLSERIAL_PORT_SIZE==3
	port-=1;
#endif//#if RLSERIAL_PORT_SIZE==3
  int data=9999;
	RLSerial_Init(port,115200);
	RLSerial mserial(port);
	mserial.rxmode(port,1);
	Eye_Protocol_Typedef *protocol_t=(Eye_Protocol_Typedef *)mserial.getprotocolhandle(port);
	while(mserial.writebufferstate(port));//�ȴ�д�뻺�����
	//if(mserial.readprotocolflag(port)&&mserial.readprotocol(port,3)==_TYPE_EYE)
	if(mserial.readprotocolflag(port)&&protocol_t->type==_TYPE_EYE)
	{
		data=mserial.readprotocol(port,cmd+4);
	}
	if(mserial.getleartime(port))
	{//��ʱ�建��
		mserial.setcleartime(port);
		mserial.clearprotocol(port);
	}
	return data;
}
//���ø��۹���ģʽ
//mode=0~1:0����ģʽ,1��ͨģʽ
int SetEye(unsigned char port,unsigned char mode)
{
	if(port==0||port>RLSERIAL_PORT_SIZE)return 9999;
#if RLSERIAL_PORT_SIZE==3
	port-=1;
#endif//#if RLSERIAL_PORT_SIZE==3
  int data=9999;
	RLSerial_Init(port,115200);
	RLSerial mserial(port);
	unsigned char sum=__RLSerial_Protocol_Head1+__RLSerial_Protocol_Head2+7+_TYPE_EYE+1+mode;
	////PHEAD1+PHEAD2+LEN+type+cmd+mode+SUM
	mserial.write(port,__RLSerial_Protocol_Head1);
	mserial.write(port,__RLSerial_Protocol_Head1);
	mserial.write(port,7);
	mserial.write(port,_TYPE_EYE);
	mserial.write(port,1);
	mserial.write(port,mode);
	mserial.write(port,sum);
	
}
//��ŷ�����
//pulse=0���ɿ���ŷ�,1~100�򿪵�ŷ����൱�ڵ�ŷ����ȣ����ݲ�ͬ��ŷ�����
void SetSolenoidValve(unsigned char pulse)//pulse =0~100
{
	if(pulse>=100)pulse =100;
	pulse=pulse*255/100;if(pulse>=255)pulse=254;
	analogWrite(_ELECSWITCH_,pulse);
}
int GetInPower(void)//��ص�ѹ��λ0.1V
{
	unsigned long vol=analogRead(_BAT_);
	//Vad=Vin*R2/(R1+R2) =>vadc=Vin*47/247
	//5V/Vadc=1023/adc=>Vadc=5*adc/1023=Vin*47/247
	//vin=5*adc*247/1023/47
	vol=(vol*5000*247/1023/47+266)/100;
	return vol;
}