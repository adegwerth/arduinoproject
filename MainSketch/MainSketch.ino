#include <RH_ASK.h>
#include <SPI.h>  // Not actualy used but needed to compile

//uncluding needed subfiles
#include "steering.h" 
#include "driving.h"
#include "distance.h"

struct JOY_POS {
  int x;
  int y;
};

RH_ASK driver(2000 /*speed*/, 7 /*rxPin*/, 2 /*txPin*/);

bool calibStateRem = true;  //this will be the status of the switch on the remote device TEMPORARY

int speed = 200;  //this will be the speed variable, which can be controlled by remote device TEMPORARY

//defining of different subclasses
Steering steering; 
Driving driving;
Distance distance;

void setup() {
  Serial.begin(9600);
  
  if (!driver.init())
    Serial.println("driver init failed");
    
  //initalizing subclasses, to set pinModes or Pins
  steering.init();  
  driving.init();
  distance.init(11, 12);
}

void loop() {
  JOY_POS joyPos;

  uint8_t buflen = sizeof(joyPos);
  if (driver.recv((uint8_t*)&joyPos, &buflen)) {
    Serial.println("Position: " + String(joyPos.x) + " " + String(joyPos.y));
  }

  //calibrates the steering of remote button pressed and wasn't calibrated as far
  if ((calibStateRem == true) && (!steering.isCalibrated())) {
    Serial.println("Calibration Starting");
    steering.startCalibration(); //starting calibration
  }

  //x = up/down; y = left/right
  //ggf. while Schleife
  //  if (calibStateRem == true && steering.isCalibrated()) {
  //    steering.detailCalib(joyPos.y);
  //  }

  //egine will be stopped if distance detector has got an distance smaler ore equal to 5cm
  distance.stopIf(driving, 5);
  
  //steering process of car; with current position of joystick
  steering.handleSteering(joyPos.y);
  
  //driving process of car; with current position of joystick and speed of engines
  driving.handleDriving(joyPos.x, speed);
}
