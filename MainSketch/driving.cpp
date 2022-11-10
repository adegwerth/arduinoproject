#include <Arduino.h>

#include "driving.h"
#include "distance.h"

//Distance distance1;
// handle in main loop or provide as parameter

// ?? create a class engine
// use engineLeft and engineRight

void Driving::forward(int speed) {
  //setting pins to drive foreward with secific speed
  analogWrite(ENGINE_LEFT, speed);
  digitalWrite(BACKWARD_LEFT, LOW);
  digitalWrite(FORWARD_LEFT, HIGH);

  analogWrite(ENGINE_RIGHT, speed);
  digitalWrite(BACKWARD_RIGHT, LOW);
  digitalWrite(FORWARD_RIGHT, HIGH);
}

void Driving::backward(int speed) {
  //driving backward with specific speed
  analogWrite(ENGINE_LEFT, speed);
  digitalWrite(BACKWARD_LEFT, HIGH);
  digitalWrite(FORWARD_LEFT, LOW);

  analogWrite(ENGINE_RIGHT, speed);
  digitalWrite(BACKWARD_RIGHT, HIGH);
  digitalWrite(FORWARD_RIGHT, LOW);
}

void Driving::stop(){
  //stops driving 
  digitalWrite(BACKWARD_LEFT, LOW);
  digitalWrite(FORWARD_LEFT, LOW);
  digitalWrite(BACKWARD_RIGHT, LOW);
  digitalWrite(FORWARD_RIGHT, LOW);  
}

void Driving::stopForward() {
  digitalWrite(FORWARD_LEFT, LOW);
  digitalWrite(FORWARD_RIGHT, LOW);
}

void Driving::init() {
  for (int i = 0; i <= 5; i++) {
    pinMode(enginePins[i], OUTPUT);
  }
}

// why uses dist here?
// can be handled in main-loop?
// also the joystick variable should be handles outside to avoid 
// dependencies between joy and drive
void Driving::handleDriving(int joystickVert, int speed){
  if (joystickVert > 600 && (!distance1.barrier())) {
    forward(speed);
  } else if (joystickVert < 400) {
    backward(speed);
  } else {
    stop();    
  }      
}
