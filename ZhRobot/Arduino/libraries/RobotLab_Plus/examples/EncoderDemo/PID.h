#define speed_min_th	30//最小转速限制
#define set_motor(s) SetMotor(1,s)
#define _Encoder     GetEncoder(1)

#define T_Adjust_Range 	50//26//50//距离目标脉冲数的脉冲数
#define T_Adjust_Stop	  6
#define T_Adjust_Speed 	40//35//距离目标很近时调整速度，避免进入共振

#define MOTOR_NORMAL_MODE				0//普通模式,马达直接PWM控制
#define MOTOR_SERVO_MODE				1//伺服模式
#define MOTOR_RPM_MODE					2//转速模式,即以单位时间编码值作为度量
#define MOTOR_SPEED_TEST_MODE	  3//转速测试模式

#define BASIC_PULSE				52//一圈脉冲数;22/26对极
char _motor_mode=0;
#define SetMotorMode(m)		(_motor_mode=m)
#define CheckMotorMode(m)	(_motor_mode==m)
long _motor_tencode=0;//目标脉冲
//long _motor_setpulse=0;//伺服模式下距离目标的剩余脉冲
/******************PID参数定义*******************/
typedef struct
{
	long SetPoint;// 设定目标 Desired Value
	long Proportion;// 比例常数 ProportionalConst
	long Integral;// 积分常数 Integral Const
	long Derivative;// 微分常数 Derivative Const
	long LastError;// Error[-1]
	long PrevError;// Error[-2]
	long SumError;// Sums of Errors
	//long Delta;//PID调节后输出量
	//int Max;//PID调节上限值
	//int Min;//pid调节下限值
	//int CurVar;//当前值，即转到指定角度的速度
	//long ThError; //开始PID调节的误差阈值,只有误差小于这个值时开始调节
	
}PID_Tcb;
PID_Tcb pid_t;									//PID Control Structure
PID_Tcb *spid_t = &pid_t;				//PID 地址指针
/*******************PID初始化********************/

void PID_Init(void)
{
  spid_t->SetPoint   = 0;
	//spid_t->Delta = 0;						//输出值

	spid_t->LastError  = 0;				//Error[-1]
	spid_t->PrevError  = 0;				//Error[-2]
	spid_t->SumError   = 0;

	spid_t->Proportion = 20;//17;//				//比例常数 Proportional Const
	spid_t->Integral   = 200;//100;//					//积分常数 Integral Const
	spid_t->Derivative = 1;//0;					//微分常数 Derivative Const
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
		_motor_tencode = _Encoder;//记录起始脉冲
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

#define filter 30//0.3 //速度互补滤波系数
//long speeds=0,speed_add=0,position=0;
//long spid_t->PrevError=0;
#define Delta_Error	51//最后一圈进行调节+-3度
#define SPEED_MAX	100
#define CurSpeed 100
long MotorServoPidCalc(long NextPoint)
{
	long iError;
	long var;
	int speeds;
	iError = spid_t->SetPoint - NextPoint;
	if(abs(iError)>Delta_Error)//不在指定范围
	{
		//spid_t->Delta = 0;						//输出值
		spid_t->LastError  = 0;				//Error[-1]
		spid_t->PrevError  = 0;				//Error[-2]
		spid_t->SumError   = 0;
		if(iError>Delta_Error)return abs(CurSpeed); //大于指定范围，并且为正时,应该反转
		else if(iError<-Delta_Error)return 0-abs(CurSpeed); //大于指定范围,并且为-时，应该正转
		return CurSpeed;
	}
	speeds=iError*100/Delta_Error; //计算出当前应该给的速度
	spid_t->SumError=(100-filter)*spid_t->SumError/100+filter*speeds/100;//互补滤波 计算出当前速度
	spid_t->LastError=spid_t->LastError+spid_t->SumError; //通过速度积分 计算出位置累积量
	
	//if(spid_t->LastError>spid_t->Max) spid_t->LastError=spid_t->Max;  //+100限定最大最小值
	//if(spid_t->LastError<-spid_t->Max) spid_t->LastError=-spid_t->Max;//-100
	if(spid_t->LastError> SPEED_MAX) spid_t->LastError = SPEED_MAX;  //+100限定最大最小值
	if(spid_t->LastError<-SPEED_MAX) spid_t->LastError =-SPEED_MAX;//-100
	//kp=147 ki=100,kd=0
	var=spid_t->Proportion * spid_t->SumError/100 
		 +spid_t->Integral	 * spid_t->LastError/1000
		 +spid_t->Derivative * spid_t->PrevError/100;  //PID控制器  速度环
	spid_t->PrevError=spid_t->LastError;//记录上一次位置
	//if(var> spid_t->Max) var = spid_t->Max;	//+100上限控制
	//if(var< -spid_t->Max)var = -spid_t->Max;	//-100下限控制
	if(var> SPEED_MAX) var = SPEED_MAX;	//+100上限控制
	if(var<-SPEED_MAX) var =-SPEED_MAX;	//-100下限控制
	return(var);
}
void MotorServoPID_Init(int speed,long pulse)
{
		     Serial.print("MotorServoPID_Init: ");
     Serial.println(speed);
	Motor_Angle(speed,pulse);
	
	spid_t->SetPoint   = _motor_tencode;//目标角度
  //spid_t->Delta = 0;						//输出值
	spid_t->LastError  = 0;				//Error[-1]
	spid_t->PrevError  = 0;				//Error[-2]
	spid_t->SumError   = 0;

	//spid_t->Proportion = 20;//17;				//比例常数 Proportional Const
	//spid_t->Integral   = 200;//100;				//积分常数 Integral Const
	//spid_t->Derivative = 1;//0;					//微分常数 Derivative Const
  //spid_t->Max=100;
  //spid_t->Min=-100;
}
int MotorServoPID(void)
{
	int cur_speed=0;
	if(!CheckMotorMode(MOTOR_SERVO_MODE))return ;
	//到达指定停车范围，则停止
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