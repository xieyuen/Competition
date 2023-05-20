#ifndef _LEDMatrix_H_
#define _LEDMatrix_H_

#include <Arduino.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include "wiring_private.h"
#include "pins_arduino.h"

#define HT16K33_BLINK_CMD 0x80//Display setup
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

#define HT16K33_CMD_BRIGHTNESS 0xE0//Dimming set
#define HT16K33_DISPLAY_RAM 		 (uint8_t)0X00

#define SEVENSEG_DIGITS 5

void LM_iic_writereg(int sdaport,int sclport,uint8_t data);
void LM_setBrightness(int sdaport,int sclport,uint8_t b) ;
void LM_blinkRate(int sdaport,int sclport,uint8_t b) ;
void LM_init(int sdaport,int sclport) ;
void LM_writeDisplay(int sdaport,int sclport,int x,int y,uint8_t *displaybuffer) ;
void LM_clear(int sdaport,int sclport) ;
void LM_writeDisplayDot(int sdaport,int sclport,uint8_t x,uint8_t y,uint8_t state);
void LM_writeDisplayCom(int sdaport,int sclport,uint8_t x,uint16_t data) ;

#endif