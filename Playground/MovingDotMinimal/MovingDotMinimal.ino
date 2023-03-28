//display 128x64 (128/8 == 16 (Width))
//snake size 4x4 (64/8 == 8  (Height))

#include "SSD1306_minimal.h"
#include <avr/pgmspace.h>

SSD1306_Mini oled;

#define WIDTH 16
#define HEIGHT 32

int wPin = 2;
int aPin = 8;
int sPin = 1;
int dPin = 7;


// there are different wall types
unsigned char block[8][4]= { 
  {B0000,
   B0000,   // 0: Air
   B0000,
   B0000},
   
  {B0110,
   B1001,   // 1: Player
   B1001,
   B0110},
  
  {B1111,
   B1001,   // 2: Wall
   B1001,
   B1111},

  {B1111,
   B1111,   // 3: Solid
   B1111,
   B1111},
   
  {B0111,
   B1111,   // 4: Solid bottom left
   B1111,
   B1111},
  {B1110,
   B1111,   // 5: Solid top left
   B1111,
   B1111},
  {B1111,
   B1111,   // 6: Solid top rightt
   B1111,
   B1110},
  {B1111,
   B1111,   // 7: Solid bottom right
   B1111,
   B0111}
};


int lastTime, currentTime;
unsigned char posX = 2; 
unsigned char posY = 2; 
char velX = 0;
char velY = 0;

// room with block-arrangement "hello"
const uint8_t room[HEIGHT][WIDTH] PROGMEM = {

// 0 1 2 3 4 5 6 7 8 9 101112131415
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  0
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  1
  {3,0,0,2,2,2,2,2,2,2,2,2,2,0,0,3}, //  2
  {3,0,0,0,0,0,0,0,2,0,0,0,0,0,0,3}, //  3
  {3,0,0,0,0,0,0,0,2,0,0,0,0,0,0,3}, //  4
  {3,0,0,2,2,2,2,2,2,2,2,2,2,0,0,3}, //  5
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  6
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  7
  {3,0,0,2,2,2,2,2,2,2,2,2,0,0,0,3}, //  8
  {3,0,0,0,0,0,0,0,2,0,0,0,2,0,0,3}, //  9
  {3,0,0,0,0,0,0,0,2,0,0,0,2,0,0,3}, // 10
  {3,0,0,0,0,0,0,0,2,0,0,0,2,0,0,3}, // 11 
  {3,0,0,2,2,2,2,2,2,2,2,2,0,0,0,3}, // 12
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 13
  {3,0,0,2,2,2,2,2,2,2,2,2,2,0,0,3}, // 14
  {3,0,0,2,0,0,0,0,0,0,0,0,0,0,0,3}, // 15
  {3,0,0,2,0,0,0,0,0,0,0,0,0,0,0,3}, // 16
  {3,0,0,2,0,0,0,0,0,0,0,0,0,0,0,3}, // 17
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 18
  {3,0,0,2,2,2,2,2,2,2,2,2,2,0,0,3}, // 19
  {3,0,0,2,0,0,0,0,0,0,0,0,0,0,0,3}, // 20
  {3,0,0,2,0,0,0,0,0,0,0,0,0,0,0,3}, // 21
  {3,0,0,2,0,0,0,0,0,0,0,0,0,0,0,3}, // 22
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 23
  {3,0,0,0,0,2,2,2,2,2,2,2,0,0,0,3}, // 24
  {3,0,0,2,0,0,0,0,0,0,0,0,2,0,0,3}, // 25
  {3,0,0,2,0,0,0,0,0,0,0,0,2,0,0,3}, // 26
  {3,0,0,2,0,0,0,0,0,0,0,0,2,0,0,3}, // 27
  {3,0,0,0,2,2,2,2,2,2,2,2,0,0,0,3}, // 28
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 29
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 30
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 31
};


// room with nothing
/*
const uint8_t room[HEIGHT][WIDTH] PROGMEM = {

// 0 1 2 3 4 5 6 7 8 9 101112131415
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  0
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  1
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  2
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  3
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  4
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  5
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  6
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  7
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  8
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, //  9
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 10
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 11
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 12
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 13
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 14
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 15
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 16
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 17
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 18
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 19
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 20
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 21
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 22
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 23
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 24
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 25
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 26
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 27
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 28
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 29
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 30
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3}, // 31
};
*/



unsigned char getBlock (unsigned char x, unsigned char y) {
  return pgm_read_byte(&room[y][x]);
}

void displayRoom(){

  for (char x=WIDTH-1;x>=0; x-=2) {
    for (char y=0;y<HEIGHT; y++) {
    
      unsigned char blockLeft  = pgm_read_byte(&room[y][x-1]);
      unsigned char  blockRight = pgm_read_byte(&room[y][x  ]);

      unsigned char  twoBlockSendPackage[4] = {
      //  L \/ R    //L: Left Block, R: Right Block
      // 1..41..4 
        B00000000, // 1
        B00000000, // 2   create a blank package
        B00000000, // 3
        B00000000, // 4
      };

      for (unsigned char  i = 0; i<4; i++) {

        // add the wall texture to the send package
        twoBlockSendPackage[i] |= block[blockLeft ][i] << 4; //shift left block 4 bits left 
        twoBlockSendPackage[i] |= block[blockRight][i] << 0; 

        // add the wall texture to the send package
        if ((x-1 == posX) && (y == posY)) {twoBlockSendPackage[i]|= block[1][i] << 4;} 
        if ((x   == posX) && (y == posY)) {twoBlockSendPackage[i]|= block[1][i] << 0;}
        
      }
  
      // send a bunch of data in one xmission
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);            // data mode
      for (int i = 0; i<4; i++) {Wire.send(twoBlockSendPackage[i]);}
      Wire.endTransmission();
    }
  }  
}

void setup() {
  pinMode(4, OUTPUT);
  pinMode(wPin, INPUT_PULLUP);
  pinMode(aPin, INPUT_PULLUP);
  pinMode(sPin, INPUT_PULLUP);
  pinMode(dPin, INPUT_PULLUP);
  oled.init(0x3c);
  oled.startScreen();
}
 
void loop(){
  bool w = !digitalRead(wPin); //Button is pressed, when digitalRead returns 0
  bool a = !digitalRead(aPin);
  bool s = !digitalRead(sPin);
  bool d = !digitalRead(dPin);
  

  if (a) {velX = -1;}
  if (d) {velX = +1;}
  if (!a && !d) {velX = 0;}

  if (w) {velY = -1;}
  if (s) {velY = +1;}
  if (!w && !s) {velY = 0;}
  
  if ((posX+velX>=0) && (posX+velX<WIDTH ) && (pgm_read_byte(&room[posY     ][posX+velX])==0)) {posX += velX;}
  if ((posY+velY>=0) && (posY+velY<HEIGHT) && (pgm_read_byte(&room[posY+velY][posX     ])==0)) {posY += velY;}


  displayRoom();
  delay(5); 
}
