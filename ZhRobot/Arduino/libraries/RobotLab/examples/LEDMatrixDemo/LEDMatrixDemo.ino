#include"RobotLab.h"

const unsigned char f[16]={120,132,130,65,65,130,132,120,0,0,0,0,0,0,0,0};
const uint8_t f1[16]={24,60,126,153,24,24,24,24,0,0,0,0,0,0,0,0};
const uint8_t f2[16] = {0,56,68,34,34,68,56,0,0,0,0,0,0,0,0,0};//{0,0,126,80,80,80,0,0,0,0,0,0,0,0,0,0};
const uint8_t f3[16] ={32,64,36,14,14,36,64,32,0,0,0,0,0,0,0,0};
const uint8_t f4[16] ={32,16,36,8,8,36,16,32,0,0,0,0,0,0,0,0};
void setup() {
}

void loop() {
  uint8_t i,j; 
  SetLEDMatrixBrightness(15);
  SetLEDMatrix(f);
  delayms(1000);
  SetLEDMatrixClear();
  SetLEDMatrixBrightness(0);
  SetLEDMatrix(f2);
  delayms(1000);
  SetLEDMatrixClear();
  SetLEDMatrixBrightness(5);
  SetLEDMatrix(f3);
  delayms(1000);
  SetLEDMatrixClear();
   for(i=0;i<8;i++)
  {
   SetLEDMatrixCom(i,0xff) ;
   delayms(50);
   SetLEDMatrixCom(i,0x00) ;
  }
  delayms(1000);
  SetLEDMatrixClear();

  for(i=0;i<8;i++)
  {
    for(j=0;j<8;j++)
    {
      SetLEDMatrixXY(i,j,1);
      delayms(50);
      SetLEDMatrixXY(i,j,0);
    }
  }
  SetLEDMatrixClear();
  delayms(1000);

 
}
