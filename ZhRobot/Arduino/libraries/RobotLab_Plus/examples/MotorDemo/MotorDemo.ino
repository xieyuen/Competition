#include <RobotLab.h>


void setup() 
{
}
int speed=0,dir=0;
void loop() 
{
  SetMotor(1,speed);
   SetMotor(2,speed);
  delay(10);
  if(dir==0)
  {
      speed=speed+1;
      if(speed>=100)
      {
        speed=99;
        dir=1;
      }
  }
  else
  {
      speed=speed-1;
      if(speed<=-100)
      {
        speed=-99;
        dir=0;
      }
  }//*/
}
