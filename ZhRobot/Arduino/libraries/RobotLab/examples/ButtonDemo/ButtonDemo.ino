#include <RobotLab.h>


void setup() 
{
}

void loop() 
{
  if(GetButton(9)) SetLed(10,HIGH);
  else SetLed(10,LOW);
}
