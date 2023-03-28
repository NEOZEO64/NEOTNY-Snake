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
unsigned char block[4][4]= { 
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
};

long oldTime;


const unsigned char ballSpawnPosX = 2;
const unsigned char ballSpawnPosY = 29;
const unsigned char ballSpawnVelY = -1;
const unsigned char ballSpawnVelX = -1;

char ballPosX = ballSpawnPosX;
char ballPosY = ballSpawnPosY;
char ballVelX = ballSpawnVelX;
char ballVelY = ballSpawnVelY;

unsigned char ballBlock = 2;
unsigned char paddleBlock = 3;
unsigned char paddleWidth = 3;
const unsigned char enemyPaddleSpawnX = WIDTH-paddleWidth;

unsigned char playerPaddlePosX = WIDTH/2-paddleWidth/2;
unsigned char playerPaddlePosY = 2;
unsigned char enemyPaddlePosX = WIDTH/2-paddleWidth/2;
unsigned char enemyPaddlePosY = HEIGHT-2;

char nextBallPosX = ballPosX;
char nextBallPosY = ballPosY;

char playerPaddleVelX = 0;

// room with nothing
const unsigned char room[HEIGHT][WIDTH] PROGMEM = {

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
  {B00000000,B00000000,B00000000,B00000000},
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

void displayGame(){
  for (char x=WIDTH-1;x>=0; x-=2) {
    for (char y=0;y<HEIGHT; y++) {
    
      unsigned char blockLeft  = getBlock(x-1,y);
      unsigned char blockRight = getBlock(x,y);

      if (ballPosX == x-1 && ballPosY == y) {blockLeft = ballBlock;}
      if (ballPosX == x  && ballPosY == y) {blockRight = ballBlock;}

      if (x-1>= playerPaddlePosX && x-1<=playerPaddlePosX+paddleWidth && y == playerPaddlePosY) {blockLeft = paddleBlock;}
      if (x>= playerPaddlePosX && x<=playerPaddlePosX+paddleWidth && y == playerPaddlePosY) {blockRight = paddleBlock;}

      if (x-1>= enemyPaddlePosX && x-1<=enemyPaddlePosX+paddleWidth && y == enemyPaddlePosY) {blockLeft = paddleBlock;}
      if (x>= enemyPaddlePosX && x<=enemyPaddlePosX+paddleWidth && y == enemyPaddlePosY) {blockRight = paddleBlock;}

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
  ballPosX = ballSpawnPosX;
  ballPosY = ballSpawnPosY;
  nextBallPosX = ballSpawnPosX;
  nextBallPosY = ballSpawnPosY;
  ballVelX = ballSpawnVelX;
  ballVelY = ballSpawnVelY;
  enemyPaddlePosX = enemyPaddleSpawnX;
  playerPaddlePosX = enemyPaddleSpawnX;
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
  if (!digitalRead(aPin)) {playerPaddleVelX = -1;}
  if (!digitalRead(dPin)) {playerPaddleVelX =  1;}
  if (digitalRead(dPin) && digitalRead(aPin)) {playerPaddleVelX = 0;}

  if (millis()-oldTime > 100) {

    if (ballPosY<0 || ballPosY>=HEIGHT-1) {
      delay(500);
      newGame();
      displayGameOver();
      delay(gameOverDelay);
    }

    //PlayerPaddle Bewegung
    if (playerPaddleVelX==-1 && playerPaddlePosX>0) {playerPaddlePosX--;}
    if (playerPaddleVelX==1 && playerPaddlePosX<WIDTH) {playerPaddlePosX++;}

    //EnemyPaddle Bewegung langsam
    //if (ballPosX<enemyPaddlePosX) {enemyPaddlePosX-=1;}
    //if (ballPosX>enemyPaddlePosX+paddleWidth) {enemyPaddlePosX+=1;}

    //PlayerPaddle Bewegung
    if (playerPaddleVelX==-1 && playerPaddlePosX>0) {enemyPaddlePosX--;}
    if (playerPaddleVelX==1 && playerPaddlePosX<WIDTH) {enemyPaddlePosX++;}
    

    
    nextBallPosX += ballVelX;
    nextBallPosY += ballVelY;

    if (nextBallPosX <2 || nextBallPosX>=WIDTH-2) {ballVelX*=-1;}

    if (nextBallPosX >= enemyPaddlePosX && nextBallPosX <= enemyPaddlePosX+paddleWidth && nextBallPosY == enemyPaddlePosY) {ballVelY*=-1;}
    if (nextBallPosX >= playerPaddlePosX && nextBallPosX <= playerPaddlePosX+paddleWidth && nextBallPosY == playerPaddlePosY) {ballVelY*=-1;}

    ballPosX = nextBallPosX;
    ballPosY = nextBallPosY;
 
    displayGame();
  }
}
