#include <RobotLab.h>
#include "PID.h"
void setup()
{
  Serial.begin(9600);
  SetEncoder(1);SetEncoder(2);
}
long ec1=0,ec2=0;
long lec1=0,lec2=0;
void loop() 
{
  /*ec1=GetEncoder(1);ec2=GetEncoder(2);
  if(lec1!=ec1)
  {
    lec1=ec1;
     Serial.print("EC1:");
     Serial.println(ec1);
  }
  if(lec2!=ec2)
  {lec2=ec2;
     Serial.print("EC2:");
     Serial.println(ec2);
  }//***/
  //setWaiteforAnlge(80,1130);
  SetMotor(1,50);
  delay(1000);
  //setWaiteforAnlge(-80,1130);
  SetMotor(1,-50);
  delay(1000);
}
