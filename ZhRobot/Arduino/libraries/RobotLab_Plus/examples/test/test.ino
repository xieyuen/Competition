void pinModeA(uint8_t pin, uint8_t mode)//0-3=A0-A3 PC0-PC3
{
  if (pin >5) return;
  if (mode == INPUT) {  DDRC&=~(1<<pin);  } 
  else if (mode == INPUT_PULLUP) 
  {
    DDRC&=~(1<<pin);
    PORTC|=(1<<pin);
  } 
  else  {   DDRC|=(1<<pin); }
}

void digitalWriteA(uint8_t pin, uint8_t val)
{
  if (pin >5) return;
  if (val == LOW)   {   PORTC&=~(1<<pin); } 
  else {    PORTC|=(1<<pin);  }

}
int digitalReadA(uint8_t pin)
{
  if (pin >5) return LOW;
  if (PINC&(1<<pin)) return HIGH;
  return LOW;
}
const int TrigPin = 3 ;//A0
const int EchoPin = 13;//D9
void setup() 
{
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
 pinMode(EchoPin, INPUT); // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
 Serial.println("Ultrasonic sensor:");
}
float distance;
void loop() 
{
    // 产生一个10us的高脉冲去触发TrigPin
    digitalWriteA(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWriteA(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWriteA(TrigPin, LOW);
    // 检测脉冲宽度，并计算出距离
    distance = pulseIn(EchoPin, HIGH) / 58.00;
    Serial.print(distance);
    Serial.print("cm");
    Serial.println();
    delay(100);
}

