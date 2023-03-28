//display 128x64 (128/8 == 16 (Width))
//snake size 4x4 (64/8 == 8  (Height))

#include "SSD1306_minimal.h"
#include <avr/pgmspace.h>

SSD1306_Mini oled;

#define WIDTH 32
#define HEIGHT 16

uint8_t twoBlockSendPackage;

void fillScreen(bool color) {
  for (unsigned char x=0;x<64; x+=8) {
    for (unsigned char y=0;y<128; y++) {
      
      if (color) {twoBlockSendPackage = B11111111;}
      else       {twoBlockSendPackage = B00000000;}
  
      // send a bunch of data in one xmission
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);            // data mode
      Wire.send(twoBlockSendPackage);
      Wire.endTransmission();
    }
  }  
}


void setup() {
  pinMode(4, OUTPUT);
  oled.init(0x3c);
  oled.startScreen();  
}
 
void loop(){
  fillScreen(true);
  delay(1000);
  fillScreen(false);
  delay(1000); 
}
