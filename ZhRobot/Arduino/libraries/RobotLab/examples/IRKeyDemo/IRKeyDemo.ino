#include <RobotLab.h>

void setup()
{
  Serial.begin(9600);
  Serial.print("IR Test");
}
uint32_t results;
void loop() 
{
  results=GetIRKey(10,0);
  if(results>0)SetLed(9,1);else SetLed(9,0);
}
