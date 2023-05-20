#define speed_min_th	30//��Сת������
#define set_motor(s) SetMotor(1,s)
#define _Encoder     GetEncoder(1)

#define T_Adjust_Range 	50//26//50//����Ŀ����������������
#define T_Adjust_Stop	  6
#define T_Adjust_Speed 	40//35//����Ŀ��ܽ�ʱ�����ٶȣ�������빲��

#define MOTOR_NORMAL_MODE				0//��ͨģʽ,���ֱ��PWM����
#define MOTOR_SERVO_MODE				1//�ŷ�ģʽ
#define MOTOR_RPM_MODE					2//ת��ģʽ,���Ե�λʱ�����ֵ��Ϊ����
#define MOTOR_SPEED_TEST_MODE	  3//ת�ٲ���ģʽ

#define BASIC_PULSE				52//һȦ������;22/26�Լ�
char _motor_mode=0;
#define SetMotorMode(m)		(_motor_mode=m)
#define CheckMotorMode(m)	(_motor_mode==m)
long _motor_tencode=0;//Ŀ������
//long _motor_setpulse=0;//�ŷ�ģʽ�¾���Ŀ���ʣ������
/******************PID��������*******************/
typedef struct
{
	long SetPoint;// �趨Ŀ�� Desired Value
	long Proportion;// �������� ProportionalConst
	long Integral;// ���ֳ��� Integral Const
	long Derivative;// ΢�ֳ��� Derivative Const
	long LastError;// Error[-1]
	long PrevError;// Error[-2]
	long SumError;// Sums of Errors
	//long Delta;//PID���ں������
	//int Max;//PID��������ֵ
	//int Min;//pid��������ֵ
	//int CurVar;//��ǰֵ����ת��ָ���Ƕȵ��ٶ�
	//long ThError; //��ʼPID���ڵ������ֵ,ֻ�����С�����ֵʱ��ʼ����
	
}PID_Tcb;
PID_Tcb pid_t;									//PID Control Structure
PID_Tcb *spid_t = &pid_t;				//PID ��ַָ��
/*******************PID��ʼ��********************/

void PID_Init(void)
{
  spid_t->SetPoint   = 0;
	//spid_t->Delta = 0;						//���ֵ

	spid_t->LastError  = 0;				//Error[-1]
	spid_t->PrevError  = 0;				//Error[-2]
	spid_t->SumError   = 0;

	spid_t->Proportion = 20;//17;//				//�������� Proportional Const
	spid_t->Integral   = 200;//100;//					//���ֳ��� Integral Const
	spid_t->Derivative = 1;//0;					//΢�ֳ��� Derivative Const
}

void Motor_Angle(int speed,long code)
{
	     Serial.print("Motor_Angle speed: ");
     Serial.println(speed);
	if(!CheckMotorMode(MOTOR_SERVO_MODE))//(_motor_mode!=MOTOR_SERVO_MODE)
	{
		//_motor_tencode = 0;
		//_Encoder = 0;
		SetEncoder(1);
		_motor_tencode = _Encoder;//��¼��ʼ����
		SetMotorMode(MOTOR_SERVO_MODE);//
	}
	if(speed>=0)
	{
		_motor_tencode +=code;
	}
	else
	{
		_motor_tencode -=code;
	}
	//_motor_setpulse=code;
	set_motor(speed);
}

#define filter 30//0.3 //�ٶȻ����˲�ϵ��
//long speeds=0,speed_add=0,position=0;
//long spid_t->PrevError=0;
#define Delta_Error	51//���һȦ���е���+-3��
#define SPEED_MAX	100
#define CurSpeed 100
long MotorServoPidCalc(long NextPoint)
{
	long iError;
	long var;
	int speeds;
	iError = spid_t->SetPoint - NextPoint;
	if(abs(iError)>Delta_Error)//����ָ����Χ
	{
		//spid_t->Delta = 0;						//���ֵ
		spid_t->LastError  = 0;				//Error[-1]
		spid_t->PrevError  = 0;				//Error[-2]
		spid_t->SumError   = 0;
		if(iError>Delta_Error)return abs(CurSpeed); //����ָ����Χ������Ϊ��ʱ,Ӧ�÷�ת
		else if(iError<-Delta_Error)return 0-abs(CurSpeed); //����ָ����Χ,����Ϊ-ʱ��Ӧ����ת
		return CurSpeed;
	}
	speeds=iError*100/Delta_Error; //�������ǰӦ�ø����ٶ�
	spid_t->SumError=(100-filter)*spid_t->SumError/100+filter*speeds/100;//�����˲� �������ǰ�ٶ�
	spid_t->LastError=spid_t->LastError+spid_t->SumError; //ͨ���ٶȻ��� �����λ���ۻ���
	
	//if(spid_t->LastError>spid_t->Max) spid_t->LastError=spid_t->Max;  //+100�޶������Сֵ
	//if(spid_t->LastError<-spid_t->Max) spid_t->LastError=-spid_t->Max;//-100
	if(spid_t->LastError> SPEED_MAX) spid_t->LastError = SPEED_MAX;  //+100�޶������Сֵ
	if(spid_t->LastError<-SPEED_MAX) spid_t->LastError =-SPEED_MAX;//-100
	//kp=147 ki=100,kd=0
	var=spid_t->Proportion * spid_t->SumError/100 
		 +spid_t->Integral	 * spid_t->LastError/1000
		 +spid_t->Derivative * spid_t->PrevError/100;  //PID������  �ٶȻ�
	spid_t->PrevError=spid_t->LastError;//��¼��һ��λ��
	//if(var> spid_t->Max) var = spid_t->Max;	//+100���޿���
	//if(var< -spid_t->Max)var = -spid_t->Max;	//-100���޿���
	if(var> SPEED_MAX) var = SPEED_MAX;	//+100���޿���
	if(var<-SPEED_MAX) var =-SPEED_MAX;	//-100���޿���
	return(var);
}
void MotorServoPID_Init(int speed,long pulse)
{
		     Serial.print("MotorServoPID_Init: ");
     Serial.println(speed);
	Motor_Angle(speed,pulse);
	
	spid_t->SetPoint   = _motor_tencode;//Ŀ��Ƕ�
  //spid_t->Delta = 0;						//���ֵ
	spid_t->LastError  = 0;				//Error[-1]
	spid_t->PrevError  = 0;				//Error[-2]
	spid_t->SumError   = 0;

	//spid_t->Proportion = 20;//17;				//�������� Proportional Const
	//spid_t->Integral   = 200;//100;				//���ֳ��� Integral Const
	//spid_t->Derivative = 1;//0;					//΢�ֳ��� Derivative Const
  //spid_t->Max=100;
  //spid_t->Min=-100;
}
int MotorServoPID(void)
{
	int cur_speed=0;
	if(!CheckMotorMode(MOTOR_SERVO_MODE))return ;
	//����ָ��ͣ����Χ����ֹͣ
	if((_Encoder>=(_motor_tencode-T_Adjust_Stop))&&
     (_Encoder<=(_motor_tencode+T_Adjust_Stop)))
	{
		cur_speed=0;
		set_motor(cur_speed);	
		return 1;
	}
	else
	{
		cur_speed=(int)MotorServoPidCalc(_Encoder);
		if(cur_speed>0&&cur_speed<speed_min_th)cur_speed=speed_min_th;
		if(cur_speed<0&&cur_speed>-speed_min_th)cur_speed=-speed_min_th;
		set_motor(cur_speed);
	}
	return 0;
}

void setWaiteforAnlge(char speed,long pulse)
{
		     Serial.print("setWaiteforAnlge: ");
     Serial.println(speed);
	MotorServoPID_Init( speed, pulse);
	while(1)
	{
		if(MotorServoPID()==1)break;
	}
	
}