#include <RobotLab.h>


void setup() 
{
}

void loop() 
{
 SetServo(9,0);
  SetServo(10,0);
 delay(1000);
  SetServo(9,90);
  SetServo(10,180);
 delay(1000);
  SetServo(9,180);
  SetServo(10,90);
 delay(1000);
}
