#include <RobotLab.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("IR Receiver Demo");
}
uint32_t results;
void loop() 
{
  results=GetIRReceiverData(10);
  if(results>0)
  {
      switch(GetIRDecodeType(10))
      {
        case  1:    Serial.println("decode_type:NEC");      break;
        case  2:   Serial.println("decode_type:SONY");    break;
        case  3:    Serial.println("decode_type:RC5");      break;
        case  4:    Serial.println("decode_type:RC6");       break;
        case  5:    Serial.println("decode_type:PANASONIC");        break;
        case  6:    Serial.println("decode_type:JVC");       break;
        case  7:    Serial.println("decode_type:SANYO");        break;
        case  8:   Serial.println("decode_type:MITSUBISHI");     break;
        case  -1:    Serial.println("decode_type:UNKNOWN");       break;
      }
    Serial.println(results,HEX);
  }
}
