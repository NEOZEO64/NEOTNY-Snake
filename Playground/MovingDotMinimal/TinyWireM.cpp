/*
   TinyWireM.cpp - a wrapper class for TWI/I2C Master library for the ATtiny on Arduino
  1/21/2011 BroHogan -  brohoganx10 at gmail dot com

  **** See TinyWireM.h for Credits and Usage information ****

  This library is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2.1 of the License, or any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/


#include "USI_TWI_Master.h"
#include "TinyWireM.h"


// Initialize Class Variables //////////////////////////////////////////////////
	uint8_t USI_TWI::USI_Buf[USI_BUF_SIZE];             // holds I2C send and receive data
	uint8_t USI_TWI::USI_BufIdx = 0;                    // current number of bytes in the send buff
	uint8_t USI_TWI::USI_LastRead = 0;                  // number of bytes read so far
	uint8_t USI_TWI::USI_BytesAvail = 0;                // number of bytes requested but not read

// Constructors ////////////////////////////////////////////////////////////////

USI_TWI::USI_TWI(){
}

// Public Methods //////////////////////////////////////////////////////////////

void USI_TWI::begin(){ // initialize I2C lib
  USI_TWI_Master_Initialise();          
}

void USI_TWI::beginTransmission(uint8_t slaveAddr){ // setup address & write bit
  USI_BufIdx = 0; 
  USI_Buf[USI_BufIdx] = (slaveAddr<<TWI_ADR_BITS) | USI_SEND; 
}

void USI_TWI::send(uint8_t data){ // buffers up data to send
  if (USI_BufIdx >= USI_BUF_SIZE) return;         // dont blow out the buffer
  USI_BufIdx++;                                   // inc for next byte in buffer
  USI_Buf[USI_BufIdx] = data;
}

uint8_t USI_TWI::endTransmission(){ // actually sends the buffer
  bool xferOK = false;
  uint8_t errorCode = 0;
  xferOK = USI_TWI_Start_Read_Write(USI_Buf,USI_BufIdx+1); // core func that does the work
  USI_BufIdx = 0;
  if (xferOK) return 0;
  else {                                  // there was an error
    errorCode = USI_TWI_Get_State_Info(); // this function returns the error number
    return errorCode;
  }
}

// Preinstantiate Objects //////////////////////////////////////////////////////

USI_TWI TinyWireM = USI_TWI();
