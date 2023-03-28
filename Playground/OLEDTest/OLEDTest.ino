//display 128x64 (128/8 == 16 (Width))
//snake size 4x4 (64/8 == 8  (Height))

#include "SSD1306_minimal.h"
#include <avr/pgmspace.h>

SSD1306_Mini oled;

#define WIDTH 32
#define HEIGHT 16

// there are different wall types
unsigned char wall[5][4]= { 
  0x0, 0x0, 0x0, 0x0,
  0xf, 0xf, 0xf, 0xf,
  0xf, 0x9, 0x9, 0xf,
  0x9, 0x9, 0x9, 0x9,
  0x9, 0x6, 0x6, 0x9,
};

// the ball shape
unsigned char ball[4]= { 
  0x6, 0x9, 0x9, 0x6 
};

unsigned char posX= 7; 
unsigned char posY= 10; 
char velX= -1;
char velY= +1;

// this is the room shape
const uint8_t room[] PROGMEM ={
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};
  
void displayRoom(){
  oled.startScreen();

  uint8_t upperY;
  uint8_t lowerY;
  uint8_t data[4];
  
  for (char y=0;y<HEIGHT; y+=2 ){
    for (char x=0;x<WIDTH; x++){
      // room
      upperY = pgm_read_byte(&room[ y   *WIDTH + x]);
      lowerY = pgm_read_byte(&room[(y+1)*WIDTH + x]);

      for (int i = 0; i<4; i++) {
        data[i] = 0x0;
        
        
        if (lowerY) {data[i]|= wall[lowerY][i] << 4;}
        if (upperY) {data[i]|= wall[upperY][i] << 0;}
        
        if ((y == posY) && (x == posX)) {data[i]|= ball[i] << 0;}
        else if ((y+1 == posY) && (x == posX)) {data[i]|= ball[i] << 4;}
      }
  
      // send a bunch of data in one xmission
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);            // data mode
      for (int i = 0; i<4; i++) {Wire.send(data[i]);}
      Wire.endTransmission();
    }
  }  
}

void setup() {
  pinMode(4, OUTPUT);   
  oled.init(0x3c);
  oled.clear();
}
 
void loop(){
  if (pgm_read_byte(&(room[(posY+velY)*WIDTH + posX]))) {velY *= -1;}
  if (pgm_read_byte(&(room[posY*WIDTH + posX+velX]))) {velX *= -1;}
  
  posY = posY + velY;
  posX = posX + velX;

  displayRoom();
  delay(5); 
}
