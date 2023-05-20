#include <RobotLab.h>
void setup()
{
}

void loop() 
{
  setEncoderMotorAngle(1,50,1170);
  delay(1000);
  setEncoderMotorAngle(1,-50,1170);
  delay(1000);
}
