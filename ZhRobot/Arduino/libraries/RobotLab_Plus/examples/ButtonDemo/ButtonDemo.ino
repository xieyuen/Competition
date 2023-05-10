#include <RobotLab_Plus.h>


void setup() 
{
}

void loop() 
{
  if(GetButton(1)) SetLed(10,HIGH);
  else SetLed(10,LOW);
}
