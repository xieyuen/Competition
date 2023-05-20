#include <RobotLab.h>


void setup() 
{
}

void loop() 
{
  if(GetAdc(0)>100) 
  {
    SetBuzzer(9, 660, 100);
  } 
}
