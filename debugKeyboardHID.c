void debugKeyboardNumber(int number){
  char char1 = (0x0f & (number >> 12));
  char char2 = (0x0f & (number >> 8));
  char char3 = (0x0f & (number >> 4));
  char char4 = 0x0f & number;
  UsbKeyboard.sendKeyStroke(0x28);
  charToHID(char1);
  charToHID(char2);
  charToHID(char3);
  charToHID(char4);
}
void charToHID(char hidChar){
  if(hidChar == 0x00){
    UsbKeyboard.sendKeyStroke(0x27);
  }else if(hidChar >= 0x01 && hidChar <= 0x09){
     UsbKeyboard.sendKeyStroke(0x1D + hidChar);
  }else if(hidChar >= 0x0A && hidChar <= 0x0F){
     UsbKeyboard.sendKeyStroke(0x03 + hidChar - 0x09);
  }
}