/*
    This is IR Code Finder for Universal PC Remote.
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
#include <IRremote.h>

int RECV_PIN = 10;
int IR_VCC = 8;
int IR_GND = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;
String command="";

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(IR_VCC, OUTPUT);
  pinMode(IR_GND, OUTPUT);
  digitalWrite(IR_VCC, HIGH);
  digitalWrite(IR_GND, LOW);
  
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print(getRemoteType(&results));
    Serial.print("-");
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
String getRemoteType(decode_results *results){
  if (results->decode_type == NEC)
    return "NEC";
  else if (results->decode_type == RC5)
    return "RC5";
  else if (results->decode_type == RC6)
    return "RC6";
  else
    return "UIR";
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
