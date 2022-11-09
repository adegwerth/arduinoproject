#include <RH_ASK.h>
#include <SPI.h>  // Not actualy used but needed to compile

#include "steering.h"
#include "driving.h"

struct JOY_POS {
  int x;
  int y;
};

RH_ASK driver(2000 /*speed*/, 7 /*rxPin*/, 2 /*txPin*/);

bool calibStateRem = true;  //this will be the status of the switch on the remote device

int speed = 200;

Steering steering;
Driving driving;

void setup() {
  Serial.begin(9600);
  if (!driver.init())
    Serial.println("driver init failed");
  steering.init();
  driving.init();
}

void loop() {
  JOY_POS joyPos;

  uint8_t buflen = sizeof(joyPos);
  if (driver.recv((uint8_t*)&joyPos, &buflen)) {
    Serial.println("Position: " + String(joyPos.x) + " " + String(joyPos.y));
  }

  if ((calibStateRem == true) && (!steering.isCalibrated())) {
    Serial.println("Calibration Starting");
    steering.startCalibration();
  }

  //x = up/down; y = left/right
  //ggf. while Schleife
  if (calibStateRem == true && steering.isCalibrated()) {
    steering.detailCalib(joyPos.y);
  }

  steering.handleSteering(joyPos.y);

  driving.handleDriving(joyPos.x, speed);
}