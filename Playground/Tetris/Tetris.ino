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

int gameOverDelay = 3000;

// there are different wall types
unsigned char block[8][4]= { 
  {B0000,
   B0000,   // 0: Air
   B0000,
   B0000},
   
  {B0110,
   B1111,   // 1: Player
   B1111,
   B0110},
  
  {B0110,
   B1001,   // 2: Apple
   B1001,
   B0110},

  {B1111,
   B1111,   // 3: Solid
   B1111,
   B1111},
   
  {B0001,
   B0111,   // 4: Solid bottom left
   B0111,
   B1111},
  {B1000,
   B1110,   // 5: Solid top left
   B1110,
   B1111},
  {B1111,
   B1110,   // 6: Solid top right
   B1110,
   B1000},
  {B1111,
   B0111,   // 7: Solid bottom right
   B0111,
   B0001}
};

char velX = 0;
char velY = 0;
long oldTime;
const unsigned char maxSnakeLen = 100;
unsigned char snakeX[maxSnakeLen];
unsigned char snakeY[maxSnakeLen];
unsigned char snakeBlock = 1;
unsigned char snakeLen = 3;
unsigned char appleX;
unsigned char appleY;
char dir = 1; // 0: Right;   1: Down;    2: Left   3: Up


// room with nothing
const unsigned char room[HEIGHT][WIDTH] PROGMEM = {

// 0 1 2 3 4 5 6 7 8 9 101112131415
  {4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,5}, //  0
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
  {7,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6}, // 31
};


const unsigned char gameLogo[12][4] = {
  {B00000000,B00000000,B00000000,B00000000},
  {B01111100,B01111100,B11000110,B11111110},
  {B11111110,B11111110,B11101110,B11111110},
  {B11000110,B11000110,B11111110,B11000110},
  {B11000000,B11000110,B11111110,B11000000},
  {B11011110,B11111110,B11111110,B11111100},
  {B11011110,B11111110,B11010110,B11111100},
  {B11000110,B11000110,B11000110,B11000000},
  {B11000110,B11000110,B11000110,B11000110},
  {B11111110,B11000110,B11000110,B11111110},
  {B01111100,B11000110,B11000110,B11111110},
  {B00000000,B00000000,B00000000,B00000000},
};

const unsigned char overLogo[12][4] = { //16px below gameLogo
  {B00000000,B00000000,B00000000,B00000000},
  {B01111100,B11000110,B11111110,B11111100},
  {B11111110,B11000110,B11111110,B11111110},
  {B11101110,B11000110,B11000110,B11000110},
  {B11000110,B11000110,B11000000,B11000110},
  {B11000110,B11101110,B11111100,B11111110},
  {B11000110,B11101110,B11111100,B11111100},
  {B11000110,B11111110,B11000000,B11111000},
  {B11101110,B01111100,B11000110,B11011100},
  {B11111110,B01111100,B11111110,B11001110},
  {B01111100,B00111000,B11111110,B11000110},
  {B00000000,B00010000,B00000000,B00000000},
};


unsigned char getBlock (unsigned char x, unsigned char y) {
  return pgm_read_byte(&room[y][x]);
}


void displayGameOver() {
  for (unsigned char x=0;x<64; x+=8) {
    for (unsigned char y=0;y<128; y++) {
      unsigned char lineSendPackage = B00000000;

      if (x >= 16 && x < 48 && y>=52 && y<64) {lineSendPackage |= gameLogo[y-52][5-x/8];}
      if (x >= 16 && x < 48 && y>=64 && y<76) {lineSendPackage |= overLogo[y-64][5-x/8];}
  
      // send a bunch of data in one xmission
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);
      Wire.send(lineSendPackage);
      Wire.endTransmission();
    }
  }  
}


void displayRoom(){
  for (char x=WIDTH-1;x>=0; x-=2) {
    for (char y=0;y<HEIGHT; y++) {
    
      unsigned char blockLeft  = getBlock(x-1,y);
      unsigned char blockRight = getBlock(x,y);

      for (int i = 0; i<snakeLen; i++) {
        if (x-1 == snakeX[i] && y == snakeY[i]) {blockLeft  = snakeBlock;}
        if (x   == snakeX[i] && y == snakeY[i]) {blockRight = snakeBlock;} 
      }

      if (x-1 == appleX && y == appleY) {blockLeft = 2;} 
      if (x   == appleX && y == appleY) {blockRight = 2;} 

      unsigned char twoBlockSendPackage[4] = {
      //  L \/ R    //L: Left Block, R: Right Block
      // 1..41..4 
        B00000000, // 1
        B00000000, // 2   create a blank package
        B00000000, // 3
        B00000000, // 4
      };

      // send a bunch of data in one xmission
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);            // data mode
      for (int i = 0; i<4; i++) {
        // add the wall texture to the send package
        twoBlockSendPackage[i] |= block[blockLeft ][i] << 4; //shift left block 4 bits left 
        twoBlockSendPackage[i] |= block[blockRight][i] << 0; 
        Wire.send(twoBlockSendPackage[i]);
      }
      Wire.endTransmission();
    }
  }  
}

void newGame() {
  for (unsigned char i = 0; i < snakeLen; i++) {
     snakeX[i] = 0; snakeY[i] = 0;
  }
  snakeX[0] = 4;
  snakeY[0] = 4;
  snakeX[1] = 4;
  snakeY[1] = 5;
  snakeX[2] = 4;
  snakeY[2] = 6;
  dir = 1;
  snakeLen = 3;
  appleX = random(1,16);
  appleY = random(1,32);
}


void setup() {
  pinMode(4, OUTPUT);
  pinMode(wPin, INPUT_PULLUP);
  pinMode(aPin, INPUT_PULLUP);
  pinMode(sPin, INPUT_PULLUP);
  pinMode(dPin, INPUT_PULLUP);
  oldTime = millis();
  
  oled.init(0x3c);
  oled.startScreen();

  newGame();
}
 
void loop(){
  if (!digitalRead(aPin) && dir != 0) {dir = 2;}
  if (!digitalRead(dPin) && dir != 2) {dir = 0;}
  if (!digitalRead(wPin) && dir != 1) {dir = 3;}
  if (!digitalRead(sPin) && dir != 3) {dir = 1;}

  if (millis()-oldTime > 200) {
    oldTime = millis();
    switch(dir) {
      case 0: //right
        snakeX[snakeLen] = snakeX[snakeLen-1] + 1;
        snakeY[snakeLen] = snakeY[snakeLen-1];
        break;
      case 1: //down
        snakeX[snakeLen] = snakeX[snakeLen-1];
        snakeY[snakeLen] = snakeY[snakeLen-1] + 1;
        break;
      case 2: //left
        snakeX[snakeLen] = snakeX[snakeLen-1] - 1;
        snakeY[snakeLen] = snakeY[snakeLen-1];
        break;
      case 3: //up
        snakeX[snakeLen] = snakeX[snakeLen-1];
        snakeY[snakeLen] = snakeY[snakeLen-1] - 1;
        break;
    }
  
    if (snakeX[snakeLen-1] == appleX && snakeY[snakeLen-1] == appleY) {
      snakeLen ++;
      appleX = random(1,15);
      appleY = random(1,31);
      if (maxSnakeLen == snakeLen) {
        newGame();
      }
    } else {
      for (int i = 1; i <= snakeLen; i++) {
        snakeX[i-1] = snakeX[i];
        snakeY[i-1] = snakeY[i];
      }
    }

    displayRoom();
    
    for (int i=0; i<snakeLen-1; i++) {
      if ((snakeX[snakeLen-1]==snakeX[i]) && (snakeY[snakeLen-1]==snakeY[i])) {
        delay(500);
        newGame();
        displayGameOver();
        delay(gameOverDelay);
      }
    }
    
    if ((snakeX[snakeLen-1]<1) || (snakeX[snakeLen-1]>WIDTH-2) || (snakeY[snakeLen-1]<1) || (snakeY[snakeLen-1] > HEIGHT-2)) {
      delay(500);
      newGame();
      displayGameOver();
      delay(gameOverDelay);
    } 
  } 
}
