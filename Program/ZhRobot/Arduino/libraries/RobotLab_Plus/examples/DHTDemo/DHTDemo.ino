#include <RobotLab.h>


void setup()
{
  Serial.begin(9600);
}
float humidity=0,temperature=0;
void loop() {
  humidity=GetDHT(9,1);
  temperature=GetDHT(9,2);
  Serial.print("humidity:\t");
  Serial.print(humidity,1);
  Serial.print(" ; temperature:\t");
  Serial.println(temperature,1);
  //delay(2000);
}
