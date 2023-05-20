#include <RobotLab.h>

void setup() 
{
  Serial.begin(9600);
 Serial.println("Ultrasonic sensor:");
}
float distance;
void loop() 
{
    distance = GetUltrasonic(_D10A1_);
    Serial.print(distance);
    Serial.print("cm");
    Serial.println();
    delay(100);
}
