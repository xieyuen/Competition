#include <RobotLab.h>

void setup()
{
  Serial.begin(9600);
  SetEncoder(1);SetEncoder(2);
}
long ec1=0,ec2=0;
long lec1=0,lec2=0;
void loop() 
{
  ec1=GetEncoder(1);ec2=GetEncoder(2);
  if(lec1!=ec1||lec2!=ec2)
  {
    lec1=ec1;lec2=ec2;
     Serial.print("EC1:");
     Serial.print(ec1);
     Serial.print(" ; EC2:");
     Serial.println(ec2);
  }
}
