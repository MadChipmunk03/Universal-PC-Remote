/*
    This is Attiny85 Based IR Receiver for NEC protocol for Universal PC Remote.
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
unsigned int count = 0;
unsigned char index = 0;
unsigned int command = 0;
boolean startBit = false;
boolean isRepeatableKey = false;
void setup() {
 pinMode(IR_PIN, INPUT);
 TIMSK&=!(1<<TOIE0); // Disable timer0 as it can interfere with USB timing
}

void loop() {
  count = 0;
  UsbKeyboard.update();                 //Keyboard should be updated often
  while(digitalRead(IR_PIN)==HIGH){	//Wait and count 
    UsbKeyboard.update();               //Keyboard should be updated often
    count ++;
  }
  if ((count > 600) && (count < 700)){          //Critical 630-670
    startBit = true; // set flag
    command = 0; // reset old command
  }
  else if ((count > 290) && (count < 330)){     //Critical 300-320
    repeatIR();
  }
  else if(startBit){
    if(index >= 16){ // Ignoring Address and Reading Command
      if((count > 220) && (count < 260)){     //Critical 230-250
        if(index > 16){
          command = command << 1;
        }
        command |= 1;
      }else if ((count > 50) && (count < 90)){ //Critical 60-80
        if (index > 16){
          command = command << 1;
        }
      }
    }
      index ++;
  }
  
  UsbKeyboard.update();      //Keyboard should be updated often
  if(index >= 32){           //Full message received
    processIR();
    index = 0;
    startBit = false;
  }
  while (digitalRead(IR_PIN)==LOW){           // wait until sensor flips
    UsbKeyboard.update();    //Keyboard should be updated often
  }
} // loop end
void repeatIR(){
    if(isRepeatableKey){
      processIR();
    }
}
void processIR(){
  if(command == 0x28D7){          //Power 
    UsbKeyboard.sendKeyStroke(0x29);    // Esc
    isRepeatableKey = false;
  }
  else if(command == 0x708F){    //Preview
    UsbKeyboard.sendKeyStroke(0x2C);  // Space
    isRepeatableKey = false;
  }
  else if(command == 0xA857){    //Enter
    UsbKeyboard.sendKeyStroke(0x28);   //Enter
    isRepeatableKey = false;
  }
  else if(command == 0x8877){    //Left
    UsbKeyboard.sendKeyStroke(0x50);   //Left
    isRepeatableKey = true;
  }
  else if(command == 0x08F7){    //Right
    UsbKeyboard.sendKeyStroke(0x4F);  //Right
    isRepeatableKey = true;
  }
  else if(command == 0x48B7){    //UP
    UsbKeyboard.sendKeyStroke(0x52);   //UP
    isRepeatableKey = true;
  }
  else if(command == 0xC837){    //Down
    UsbKeyboard.sendKeyStroke(0x51);  //Down
    isRepeatableKey = true;
  }
  delay(200);
}

