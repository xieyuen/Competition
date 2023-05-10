#include <RobotLab.h>

void setup()
{
  Serial.begin(9600);
}

void loop() {

  if(GetIO(13))
  {
    SetIRData(10,0x8A20040A);
    delayms(50);
    SetIRData(10,0x8A20040E);
    while((GetIO(13)));
    delayms(300);
  }
}
