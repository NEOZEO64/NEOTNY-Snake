//display 128x64 (128/8 == 16 (Width))
//snake size 4x4 (64/8 == 8  (Height))

#include "SSD1306_minimal.h"
#include <avr/pgmspace.h>

SSD1306_Mini oled;

#define FULLWIDTH 16
#define FULLHEIGHT 32

#define WIDTH 10 //12*4 = 48
#define HEIGHT 20 //22*4 = 88

//#define OFFSETX 3
//#define OFFSETY 0

#define UPPIN 2
#define LEFTPIN 8
#define DOWNPIN 1
#define RIGHTPIN 7

#define SPAWNPOSX 5

bool up   = 0;
bool down = 0;
bool left = 0;
bool right= 0;

uint16_t fallSpeed = 600; 

//important variables to save information about the current, falling tetrino (t for tetrino)
uint8_t tForm; //type of tetrino (0-6), defined as the index in the TETRINOS-list
uint8_t tRot;  //rotation of tetrino (0-3), defined as the second index in the Tetrino list
uint8_t tPosX;
uint8_t tPosY;

const bool tForms[7][4][4] = {
    { //0. tetrino: I
        {B0000,
         B0000,
         B1111,
         B0000,},

        {B0010,
         B0010,
         B0010,
         B0010,},
        
        {B0000,
         B0000,
         B1111,
         B0000,},

        {B0010,
         B0010,
         B0010,
         B0010,}
    },
    { //1. tetrino: O
        {B11,
         B11,},

        {B11,
         B11,},

        {B11,
         B11,},
        
        {B11,
         B11,},
    },
    { //2. tetrino: T
        {B000,
         B111,
         B010},
    
        {B010,
         B110,
         B010,},
        
        {B010,
         B111,
         B000},
    
        {B010,
         B011,
         B010}
    },
    { //3. tetrion: S
        {B000,
         B011,
         B110},

        {B010,
         B011,
         B001},

        {B000,
         B011,
         B110},

        {B010,
         B011,
         B001}
    },
    { //4. tetrino: Z
        {B000,
         B110,
         B011},

        {B001,
         B011,
         B010},

        {B000,
         B110,
         B011},

        {B001,
         B011,
         B010}
    },
    { //5. tetrino: J
        {B000,
         B111,
         B001},

        {B010,
         B010,
         B110},

        {B100,
         B111,
         B000},

        {B011,
         B010,
         B010},
    },
    { //6. tetrino: L
        {B000,
         B111,
         B100},

        {B110,
         B010,
         B010},

        {B001,
         B111,
         B000},

        {B010,
         B010,
         B011}
    }
};

const uint8_t tFormWidth[7] = {4,2,3,3,3,3,3};

void newTetrino() {
    tRot = 0; //getRandom(0,3);
    tForm = random(0,6);
    if (tForm == 0) {tPosX = SPAWNPOSX-2;}
    else if (tForm == 1) {tPosX = SPAWNPOSX+1;}
    else {tPosX = SPAWNPOSX;}
    tPosY = 0;
}

//unsigned int field[HEIGHT] = {0};
uint16_t field[FULLHEIGHT] = {0};

bool collide(uint8_t tXTemp, uint8_t tYTemp, uint8_t tFormTemp, uint8_t tRotTemp) {
    //returns 1 if tetrino collides, 0 if it doesn't collide

    for (uint8_t y = 0; y<tFormWidth[tFormTemp]; y++) {
        for (uint8_t x = 0; x<tFormWidth[tFormTemp];x++) {
            if (bitRead(tForms[tFormTemp][tRotTemp][y],x) == 1 && bitRead(field[tYTemp+y],tXTemp+x)) {
                return true;
            }
        }
    }
    return false;
}

unsigned long oldTime;

void displayRoom(){
  bool blockLeft, blockRight;
  uint8_t blockLeftTexture, blockRightTexture;
  for (int8_t x = FULLWIDTH-1; x>=0; x-=2) {
    for (int8_t y=0;y<FULLHEIGHT; y++) {
      uint8_t twoBlockSendPackage[4] = {0};
      
      blockLeft  = bitRead(field[y],x-1);
      blockRight = bitRead(field[y],x);
      //Show tetrino
      if (x>=tPosX && x<tPosX+tFormWidth[tForm] && y>=tPosY && y<tPosY+tFormWidth[tForm]) {
        for (uint8_t yBlock = 0; yBlock<tFormWidth[tForm]; yBlock++) {
          for (uint8_t xBlock = 0; xBlock<tFormWidth[tForm]; xBlock++) {
            if (y == tPosY+yBlock && bitRead(tForms[tForm][tRot][yBlock],xBlock)) {
              if (x-1 == tPosX+xBlock) { blockLeft = true;}
              if (x == tPosX+xBlock)   { blockRight = true;}
            }
          }
        }  
      }
      
      blockLeftTexture = B11110000 * blockLeft;
      blockRightTexture = B00001111 * blockRight;

      // send a bunch of data in one xmission
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);            // data mode
      for (uint8_t i = 0; i<4; i++) {
        twoBlockSendPackage[i] |= blockLeftTexture;
        twoBlockSendPackage[i] |= blockRightTexture;
        Wire.send(twoBlockSendPackage[i]);
      }
      Wire.endTransmission();
      
    }
  }  
}


void setup() {
  pinMode(4, OUTPUT);
  pinMode(UPPIN, INPUT_PULLUP);
  pinMode(LEFTPIN, INPUT_PULLUP);
  pinMode(DOWNPIN, INPUT_PULLUP);
  pinMode(RIGHTPIN, INPUT_PULLUP);
  oldTime = millis();
  
  oled.init(0x3c);
  oled.startScreen();

  for (uint8_t block = 0; block<HEIGHT; block++) {field[block] = false;}
  newTetrino();
}
 
void loop(){
  if (!digitalRead(LEFTPIN) && left==0) {
    left = 1;
    if (!collide(tPosX-1, tPosY, tForm, tRot)) {tPosX -= 1;
    }
  } else {left = 1;}
  if (!digitalRead(RIGHTPIN) && right==0) {
    right = 1;
    if (!collide(tPosX+1, tPosY, tForm, tRot)) {tPosX += 1;
    }
  } else {right = 0;}
  if (!digitalRead(UPPIN) && up==0) {
    up = 1;  
    if (!collide(tPosX, tPosY, tForm, tRot+1)) {
      tRot += 1;
      if (tRot>3) {tRot=0;}
    }
  } else {up = 0;}
  //if (!digitalRead(DOWNPIN)) {}

  if (millis()-oldTime > fallSpeed) {
    oldTime = millis();

    if (collide(tPosX, tPosY+1, tForm, tRot)) {
      //Put tetrino into field as solid part
      for (uint8_t y = 0; y<tFormWidth[tForm]; y++) {
          for (uint8_t x = 0; x<tFormWidth[tForm]; x++) {
              if (bitRead(tForms[tForm][tRot][y],x) != 0) {
                  field[tPosY+y] += pow(2,tPosX+x);
              }
          }
      }
      
      //Delete line if line is complete (check every row)
      for (uint8_t y = 0; y < HEIGHT; y++) {
        uint8_t lineComplete = 1;
        for (uint8_t x = 0; x<WIDTH; x++) {
          if (bitRead(field[y],x)==0) {lineComplete = 0;} //line isnt complete
        }

        if (lineComplete) { 
          for (uint8_t yToShift = y; yToShift>0; yToShift--) {
            for (uint8_t x = 0; x<FULLWIDTH; x++) {
              field[yToShift] = field[yToShift-1];
            }
          }
          field[0] = 0;
        }
      }

      newTetrino();
      
      if (collide(tPosX, tPosY, tForm, tRot)) {
        // Game Over!
        // New Game!
        for (uint8_t block = 0; block<HEIGHT; block++) {
          field[block] = 0;
        }
        newTetrino();
      }
    } else {
      tPosY += 1;
    }
    displayRoom();
  }
}
