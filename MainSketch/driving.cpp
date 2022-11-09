#include "driving.h"
#include <Arduino.h>


void Driving::forward(int speed) {
  analogWrite(ENGINE_LEFT, speed);
  digitalWrite(BACKWARD_LEFT, LOW);
  digitalWrite(FORWARD_LEFT, HIGH);

  analogWrite(ENGINE_RIGHT, speed);
  digitalWrite(BACKWARD_RIGHT, LOW);
  digitalWrite(FORWARD_RIGHT, HIGH);
}

void Driving::backward(int speed) {
  analogWrite(ENGINE_LEFT, speed);
  digitalWrite(BACKWARD_LEFT, HIGH);
  digitalWrite(FORWARD_LEFT, LOW);

  analogWrite(ENGINE_RIGHT, speed);
  digitalWrite(BACKWARD_RIGHT, HIGH);
  digitalWrite(FORWARD_RIGHT, LOW);
}

void Driving::stop(){
  digitalWrite(BACKWARD_LEFT, LOW);
  digitalWrite(FORWARD_LEFT, LOW);
  digitalWrite(BACKWARD_RIGHT, LOW);
  digitalWrite(FORWARD_RIGHT, LOW);  
}

void Driving::init() {
  for (int i = 0; i <= 5; i++) {
    pinMode(enginePins[i], OUTPUT);
  }
    /*    
    pinMode(BACKWARD_LEFT, OUTPUT);
    pinMode(FORWARD_LEFT, OUTPUT);
    pinMode(BACKWARD_RIGHT, OUTPUT);
    pinMode(FORWARD_RIGHT, OUTPUT);
    pinMode(ENGINE_LEFT, OUTPUT);
    pinMode(ENGINE_RIGHT, OUTPUT);
    */
}

void Driving::handleDriving(int joystickVert, int speed){
  if (joystickVert > 550) {
    forward(speed);
  } else if (joystickVert < 450) {
    backward(speed);
  } else {
    stop();    
  }      
}