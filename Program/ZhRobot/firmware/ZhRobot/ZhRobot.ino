#include <RobotLab.h>
#include <avr/wdt.h>

#define HEAD1 0xFF
#define HEAD2 0x55
//1 byte 2 float 3 short 4 len+string 5 double
#define DATA_TYPE_BYTE    1
#define DATA_TYPE_FLOAT   2
#define DATA_TYPE_SHORT   3
#define DATA_TYPE_STRING  4
#define DATA_TYPE_DOUBLE  5
#define DATA_TYPE_LONG    6

#define GETCMD   0X01
#define SETCMD   0X02
#define STOPCMD  0X04

typedef enum
{
  RESET_ARDUINO = 0x00, 
    
  MOTOR = 0x01,
  SETENCODER,
  SETMOTORANGLE,
	BUZZER,
  LED,
  SERVO,
	IRESEND,
	LEDMBLIGHT,
	LEDMPIC,
	LEDMDOT,
	LED7DBLIGHT,
	LED7DCLEAR,
	LED7DNUM,
	LED7DDOT,
	LED7DNUM2,
	FAN,

  ADC_SENSOR = 0x65,
  ENCODER_SENSOR,
  //SOUND_SENSOR=0x65,
	ULTRASONIC_SENSOR=0x67 ,
	IRDATA_SENSOR ,
  DHT11_SENSOR ,
	//ADC1_SENSOR =0x65,
	//ADC2_SENSOR =0x65,
	//ADC3_SENSOR =0x65,
	//ADC4_SENSOR =0x65,

  BUTTON_SENSOR= 200,  
	IRKEY_SENSOR ,
	//IO1_SENSOR= 200,
	//IO2_SENSOR= 200,
	//IO3_SENSOR= 200,

} Sensor_Tyep_Defined;
union
{
    byte val_byte[4];
    float val_float;
}Float_t;
union
{
  byte val_byte[8];
  double val_double;
}Double_t;
union
{
  byte val_byte[2];
  short val_int;
}Int_t;
union
{
  byte val_byte[4];
  long val_long;
}Long_t;

#define protocol_minlen 1
#define BUFFER_SIZE       64
char packet_flag = 0;
int is_protocol=0;
int blen = 0;
unsigned char buffer[BUFFER_SIZE];
extern unsigned char TM1650_Init_flag;
extern uint8_t LM_init_flag;
#define IIC_PORT_UNKNOW 0
#define IIC_PORT_LED7D  1
#define IIC_PORT_LEDM   2
#define IIC_PORT_ULTRA  3
uint8_t iic_port_flag=IIC_PORT_UNKNOW;

char CkeckProtocol(void);
void RunProtocol(void);
void Ack_OK(void);
void Ack_Data(int idx,void *data,int datatype);
void Ack_Type(int type);
#define ACK(t) Ack_Type(t)
#define Reset_Arduino()  wdt_enable(WDTO_30MS); while(1);

void setup()
{
  Serial.begin(115200);
  Serial.println("Version:1.0-20180626");
}
void loop()
{
  if(CkeckProtocol()==1)
  {
    RunProtocol();
    blen=0;
    is_protocol=0;
    //memset(buffer,0,BUFFER_SIZE);
  }
}
char CkeckProtocol(void)
{
  int data;
  unsigned char *buf=buffer;
  if(Serial.available()>0)
  {
    data = Serial.read()&0xFF;
    //Serial.print(data,HEX);
    //Serial.write(data);
  }
  else return 0;
  switch(packet_flag)
  {
    case 0:
    {
      if(data==HEAD1)
      {
        packet_flag=1;
      }
    }break;
    case 1:
    {
       if(data==HEAD2)
      {
        packet_flag=2;
      }
    }break;
    case 2:
    {
      if(data<=protocol_minlen||data>BUFFER_SIZE)
      {
        packet_flag=0;
      }
      else
      {
        packet_flag=3;
        buf[0]=data;
        blen=1;
      }
    }break;
    case 3:
    {
      buf[blen++]=data;
      if(blen>buf[0])
      {
        is_protocol=1;
        packet_flag=0;
      }
      else if(blen>=BUFFER_SIZE)
      {
        is_protocol=0;
        packet_flag=0;
      }
    }break;
  }
  return is_protocol;
}
void ResetPin(int port)
{
  IRrecv_DeInit(port);
  Servo_Deinit(port);
}
void ResetIICPort(uint8_t type)
{
  if(iic_port_flag==type)return ;
  iic_port_flag=type;
  TM1650_Init_flag=0;
  LM_init_flag=0;
}
/**************************************************
  H1   H2    len  idx  action device port slot data a
  0    1     2     3    4      5      6    7    8
 HEAD1 HEAD2 0x04 0x03 0x01   0x01   0x01 0x0A
返回的数据
  H1   H2    idx  datatype  data 
  0    1     2     3        4      
 HEAD1 HEAD2 0x00 0x01    0x01   0x0a 0x0d
***************************************************/
void RunProtocol(void)
{
  int idx=0,action=0,device=0,port=0,val;
  unsigned char *buf=buffer;
  idx=buf[1]&0XFF;
  action=buf[2]&0XFF;
  device=buf[3]&0XFF;
  port=buf[4]&0XFF;
  val=buf[5]&0XFF;
  switch(action)
  {
    case STOPCMD:
    {
      TM1650_Init_flag=0;LM_init_flag=0;
      ResetIICPort(IIC_PORT_UNKNOW);
      ResetPin(9);ResetPin(10);ResetPin(11);ResetPin(13);
      SetMotor(1,0);SetMotor(2,0);
      Ack_OK();
      return ;
    }break;
    case GETCMD:
    case SETCMD:
    break;
  }
  
  switch(device)
  {
    case RESET_ARDUINO:
    {
      Reset_Arduino();
    }break;
    case ADC_SENSOR:
    {
      if(port==4||port==5){ResetIICPort(IIC_PORT_UNKNOW);}
      unsigned int data=GetAdc(port);
      Ack_Data(idx,&data,DATA_TYPE_SHORT);
    }break;
		case ENCODER_SENSOR:
    {
      double data=GetEncoder(port);
      Ack_Data(idx,&data,DATA_TYPE_DOUBLE);
    }break;
    case ULTRASONIC_SENSOR:
    {
      ResetIICPort(IIC_PORT_ULTRA);
      ResetPin(port);
      unsigned int data=GetUltrasonic(port);
      Ack_Data(idx,&data,DATA_TYPE_SHORT);
    }break;
		case IRDATA_SENSOR:
    { 
      double data=GetIRReceiver(port);
      Ack_Data(idx,&data,DATA_TYPE_DOUBLE);
    }break;
		
		case BUTTON_SENSOR:
    {
      ResetPin(port);
      char data=GetIO(port);
      Ack_Data(idx,&data,DATA_TYPE_BYTE);
    }break;
    case IRKEY_SENSOR:
    {
      char data=GetIRKey(port,val);
      Ack_Data(idx,&data,DATA_TYPE_BYTE);
    }break;
		case DHT11_SENSOR:
    {
      ResetPin(port);
      double data=GetDHT(port,val);
      Ack_Data(idx,&data,DATA_TYPE_DOUBLE);      
    }break;
    case MOTOR:
    {
      int speed=(int)(buf[5]&0xff)|((buf[6]&0xff)<<8);
      SetMotor(port,speed);
      ACK(MOTOR);
    }break;
		case SETENCODER:
    {
      SetEncoder(port);
      ACK(SETENCODER);
    }break;
		case SETMOTORANGLE:
    {
			int speed=(int)(buf[5]&0xff)|((buf[6]&0xff)<<8);
			long data=(buf[7]&0xff)|((buf[8]&0xff)<<8)|((buf[9]&0xff)<<16)|((buf[10]&0xff)<<24);
      setEncoderMotorAngle(port,speed,data);
      ACK(SETMOTORANGLE);
    }break;
    case LED:
    {
      ResetPin(port);
      SetLed(port,val);
      ACK(LED);
    }break;
    case BUZZER:
    {
      unsigned int freq=(buf[5]&0xff)|((unsigned int)(buf[6]&0xff)<<8);
      uint32_t t=(buf[7]&0xff)|((unsigned int)(buf[8]&0xff)<<8);
      ResetPin(port);
      SetBuzzer(port,freq,t);
      ACK(BUZZER);
    }break;
    case SERVO:
    {
      int angle=(buf[5]&0xff)|((unsigned int)buf[6]&0xff)<<8;
      SetServo(port,(int)angle);
      ACK(SERVO);
    }break;
		case IRESEND:
    {
      ResetPin(port);
      long data=(buf[5]&0xff)|((buf[6]&0xff)<<8)|((buf[7]&0xff)<<16)|((buf[8]&0xff)<<24);
			SetIRData(port,data);
      ACK(IRESEND);
    }break;
		case LEDMBLIGHT:
    {
      ResetIICPort(IIC_PORT_LEDM);
			SetLEDMatrixBrightness(port,val);
      ACK(LEDMBLIGHT);
    }break;
		case LEDMPIC:
    {
			uint8_t x=buf[5]&0XFF;
			uint8_t y=buf[6]&0XFF;
			uint8_t *displaybuffer=&buf[7];
     ResetIICPort(IIC_PORT_LEDM);
			SetLEDMatrix(port, x, y,displaybuffer);
      ACK(LEDMPIC);
    }break;
		case LEDMDOT:
    {
			uint8_t x=buf[5]&0XFF;
			uint8_t y=buf[6]&0XFF;
			uint8_t data=buf[7]&0XFF;
     ResetIICPort(IIC_PORT_LEDM);
			SetLEDMatrixXY(port, x, y, data);
      ACK(LEDMDOT);
    }break;
		
		case LED7DBLIGHT:
    {
      ResetIICPort(IIC_PORT_LED7D);
			Set7SegDisplayBrightness(port,val);
      ACK(LED7DBLIGHT);
    }break;
		case LED7DCLEAR:
    {
      ResetIICPort(IIC_PORT_LED7D);
			Set7SegDisplayClear(port);
      ACK(LED7DCLEAR);
    }break;
		case LED7DNUM:
    {
			int num=(buf[5]&0xff)|((int)(buf[6]&0xff)<<8);
      ResetIICPort(IIC_PORT_LED7D);
			Set7SegDisplayNum(port,num);
      ACK(LED7DNUM);
    }break;
		case LED7DDOT:
    {
      ResetIICPort(IIC_PORT_LED7D);
			Set7SegDisplayDot(port,val);
      ACK(LED7DDOT);
    }break;
		case LED7DNUM2:
    {
      ResetIICPort(IIC_PORT_LED7D);
			Set7SegDisplayChar(port,buf[5]&0xff,buf[6]&0xff);
      ACK(LED7DNUM2);
    }break;
		case FAN:
    {
			SetIO(port,val);
      ACK(FAN);
    }break;
    default:
    {
      ACK(device);
    }break;
  }
}
void Ack_OK(void)
{
    Serial.write(HEAD1);
    Serial.write(HEAD2);
    Serial.println(); 
}
void Ack_Type(int type)
{
    Serial.write(HEAD1);
    Serial.write(HEAD2);
    Serial.write(type);
    Serial.println(); 
}
void Ack_Data(int idx,void* data,int datatype)
{
  
    Serial.write(HEAD1);
    Serial.write(HEAD2);
    Serial.write(idx);
    Serial.write(datatype);
    switch(datatype)
    {
      case DATA_TYPE_BYTE:
      {
        unsigned char *dat=(unsigned char *)data;
        Serial.write(*dat);
      }break;
      case DATA_TYPE_FLOAT:
      {
        float *dat=(float *)data;
        Float_t.val_float = (float)*dat;
        Serial.write(Float_t.val_byte[0]);
        Serial.write(Float_t.val_byte[1]);
        Serial.write(Float_t.val_byte[2]);
        Serial.write(Float_t.val_byte[3]);
      }break;
      case DATA_TYPE_SHORT:
      {
        short *dat=(short *)data;
        Int_t.val_int = (short)*dat;
        Serial.write(Int_t.val_byte[0]);
        Serial.write(Int_t.val_byte[1]);
      }break;
      case DATA_TYPE_DOUBLE:
      {
        double *dat=(double *)data;
        Double_t.val_double = (double)*dat;
        Serial.write(Double_t.val_byte[0]);
        Serial.write(Double_t.val_byte[1]);
        Serial.write(Double_t.val_byte[2]);
        Serial.write(Double_t.val_byte[3]);
        Serial.write(Double_t.val_byte[4]);
        Serial.write(Double_t.val_byte[5]);
        Serial.write(Double_t.val_byte[6]);
        Serial.write(Double_t.val_byte[7]);
      }break;
			case DATA_TYPE_LONG:
      {
        long *dat=(long *)data;
        Long_t.val_long = (long)*dat;
        Serial.write(Long_t.val_byte[0]);
        Serial.write(Long_t.val_byte[1]);
        Serial.write(Long_t.val_byte[2]);
        Serial.write(Long_t.val_byte[3]);
      }break;
      default:
      {
        
      }break;
    }
    Serial.println(); 
}
