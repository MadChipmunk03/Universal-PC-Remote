/*
    This is Attiny85 Based IR Receiver for RC6 protocol for Universal PC Remote.
    Copyright (C) 2016  Rahul Kundu
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "UsbKeyboard.h"
#define IR_PIN 1
boolean previousFlipBit = true;
boolean isRepeatableKey = false;
void setup() {
 pinMode(IR_PIN, INPUT);
 TIMSK&=!(1<<TOIE0); // Disable timer0 as it can interfere with USB timing
}

void loop() {
  boolean flipBit = false;
  unsigned char address = 0x00;
  unsigned char command = 0x00;
  unsigned char count=0x00;
  UsbKeyboard.update();                 //Needs to be called often
  while(digitalRead(IR_PIN)==HIGH){	//Wait for first bit 
    UsbKeyboard.update();               //Needs to be called often
  }
  delayMicroseconds(8668);              //Delay for 8668ums
  flipBit=digitalRead(IR_PIN);	        //Reading flip bit
  for(count=0;count<8;count++){
    delayMicroseconds(889);	        //Delay for 889us
    address = address << 1;
    if(digitalRead(IR_PIN)==1)
      address = address | 0x01;
  }
  for(count=0;count<8;count++){
    delayMicroseconds(889);	        //Delay for 889us
    command = command << 1;
    if(digitalRead(IR_PIN)==1)
      command = command | 0x01;
  }
  UsbKeyboard.update();               //Needs to be called often
  if(address == 0x04){                //Cheking for valid device address
    if(previousFlipBit != flipBit){
      previousFlipBit = flipBit;
      processIR(command);
    }else if(isRepeatableKey){  //Repeat
      processIR(command);
    }
  }
} // loop end
void processIR(char command){
  if(command == 0x31){          //Stop 
    UsbKeyboard.sendKeyStroke(0x29);    // Esc
    isRepeatableKey = false;
  }
  else if(command == 0x2C){    //Play
    UsbKeyboard.sendKeyStroke(0x2C);  // Space
    isRepeatableKey = false;
  }
  else if(command == 0x5C){    //Ok
    UsbKeyboard.sendKeyStroke(0x28);   //Enter
    isRepeatableKey = false;
  }
  else if(command == 0x5A){    //Left
    UsbKeyboard.sendKeyStroke(0x50);   //Left
    isRepeatableKey = true;
  }
  else if(command == 0x5B){    //Right
    UsbKeyboard.sendKeyStroke(0x4F);  //Right
    isRepeatableKey = true;
  }
  else if(command == 0x58){    //UP
    UsbKeyboard.sendKeyStroke(0x52);   //UP
    isRepeatableKey = true;
  }
  else if(command == 0x59){    //Down
    UsbKeyboard.sendKeyStroke(0x51);  //Down
    isRepeatableKey = true;
  }
  delay(200);
}

