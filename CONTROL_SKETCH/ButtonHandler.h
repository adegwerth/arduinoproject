#include <Arduino.h>
#pragma once

class ButtonHandler {
  int buttons[4];

  public:
  ButtonHandler(int b0, int b1, int b2, int b3) {
    buttons[0] = b0;
    buttons[1] = b1;
    buttons[2] = b2;
    buttons[3] = b3;
  };
  
  void init() {
    for(int i = 0; i < sizeof(buttons); i++) {
      pinMode(buttons[i], INPUT);
    }
  }


  bool isButtonPressed(int button) {
     /*int buttonIndex = getButtonIndex(button);
     if (buttonIndex < 0) {
       return false;
     }*/
      return digitalRead(button);
  }

  private: 
  int getButtonIndex(int b) {
    for(int i = 0; i < sizeof(buttons); i++) {
      if(buttons[i] == b) {
        return i;
      } 
    }
    return -1;
  }

};