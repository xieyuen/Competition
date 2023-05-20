#include "./DHT11/DHT11.h"

// Return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
//#define OVER_TIME	20000
#define __DEBUG	//Serial.print
int HTS_dht11::read(int pin)
{
        // BUFFER TO RECEIVE
        uint8_t bits[5]={0,0,0,0,0};
        uint8_t cnt = 7;
        uint8_t idx = 0;
				unsigned int loopCnt;

        // EMPTY BUFFER
       // for (int i=0; i< 5; i++) bits[i] = 0;
				//主机开始信号
        // REQUEST SAMPLE
				//主机拉低>=18ms.拉高20~40us
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        delay(20);//>=18ms
        digitalWrite(pin, HIGH);
        delayMicroseconds(40);
				pinMode(pin, INPUT);
				
        // ACKNOWLEDGE or TIMEOUT
        loopCnt = 10000;
        while(digitalRead(pin) == LOW)         if (loopCnt-- == 0)  {__DEBUG("error=-1 \t"); return DHTLIB_ERROR_TIMEOUT;}
				
				loopCnt = 10000;
        while(digitalRead(pin) == HIGH)        if (loopCnt-- == 0)  {__DEBUG("error=-2 \t"); return DHTLIB_ERROR_TIMEOUT;}
				
				//unsigned long tt = micros()+OVER_TIME;
        //while(digitalRead(pin) == LOW)         if (tt<micros())  {__DEBUG("error=-1 \t"); return DHTLIB_ERROR_TIMEOUT;}
				//tt = micros()+OVER_TIME;
        //while(digitalRead(pin) == HIGH)        if (tt<micros())  {__DEBUG("error=-2 \t"); return DHTLIB_ERROR_TIMEOUT;}
				
        // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
        for (int i=0; i<40; i++)
        {
                loopCnt = 10000;
                while(digitalRead(pin) == LOW) if (loopCnt-- == 0)  {__DEBUG("error=-3 \t"); return DHTLIB_ERROR_TIMEOUT;}
								//tt = micros()+OVER_TIME;
								//while(digitalRead(pin) == LOW)         if (tt<micros())  {__DEBUG("error=-3 \t"); return DHTLIB_ERROR_TIMEOUT;}
								
                unsigned long t = micros();
                loopCnt = 10000;
                while(digitalRead(pin) == HIGH)if (loopCnt-- == 0)  {__DEBUG("error=-4 \t"); return DHTLIB_ERROR_TIMEOUT;}
								//tt = micros()+OVER_TIME;
								//while(digitalRead(pin) == HIGH)         if (tt<micros())  {__DEBUG("error=-4 \t"); return DHTLIB_ERROR_TIMEOUT;}
								
                if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
                if (cnt == 0)   // next byte?
                {
                        cnt = 7;    // restart at MSB
                        idx++;      // next byte!
                }
                else cnt--;
        }

        // WRITE TO RIGHT VARS
		
			humidity=bits[0]*10+bits[1];
	
			if(bits[3]&0X80)	//negative temperature
			{
				temperature = 0-(bits[2]*10+((bits[3]&0x7F)));
			}
			else   //positive temperature
			{
				temperature = bits[2]*10+bits[3];
			}
			//temperature range：-20℃~60℃，humidity range:5％RH~95％RH
			if(humidity>950) 
			{
			  humidity=950;
			}
			if(humidity<50)
			{
				humidity=50;
			}
			if(temperature>600)
			{
			  temperature=600;
			}
			if(temperature<-200)
			{
				temperature = -200;
			}
			temperature = temperature/10;//convert to the real temperature value
			humidity = humidity/10; //convert to the real humidity value

        uint8_t sum = bits[0]+bits[1]+bits[2]+bits[3];  

        if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
        return DHTLIB_OK;
}
//
// END OF FILE