
#include <SPI.h>  // Not actualy used but needed to compile

//uncluding needed subfiles
#include "steering.h" 
#include "driving.h"
#include "distance.h"
#include "MessageHeader.h"

MessageHandler messageHandler;

bool calibStateRem = true;  //this will be the status of the switch on the remote device TEMPORARY

int speed = 200;  //this will be the speed variable, which can be controled by remote device TEMPORARY

int temp = 0;

//defining of different subclasses
Steering steering; 
Driving driving;
Distance distance;

void setup() {
  Serial.begin(9600 *2);
  Serial2.begin(9600 *4);
  
  //initalizing subclasses, to set pinModes or Pins
  steering.init();  
  driving.init();
  distance.init(11, 12);
}

void loop() {

    messageHandler.pollMessage(Serial2);
    if (messageHandler.isMessageAvailable()) {

      Serial.println("Message available");
      int id;
      int data;
      messageHandler.getMessage(id, data);

      // handle message
      switch (id) {
        case 1: {
        Serial.print(temp++);
        break;}
      }
    }

    messageHandler.sendMessage(Serial2, 1, 100);
    Serial.println("data send");

  

  //calibrates the steering of remote button pressed and wasn't calibrated as far
  //if ((calibStateRem == true) && (!steering.isCalibrated())) {
  //  Serial.println("Calibration Starting");
  //  steering.startCalibration(); //starting calibration
  //}

  //x = up/down; y = left/right
  //ggf. while Schleife
  //  if (calibStateRem == true && steering.isCalibrated()) {
  //    steering.detailCalib(joyPos.y);
  //  }

  //egine will be stopped if distance detector has got an distance smaler ore equal to 5cm
  //distance.stopIf(5);
  
  //steering process of car; with current position of joystick
  //steering.handleSteering(joyPos.y);
  
  //driving process of car; with current position of joystick and speed of engines
  //driving.handleDriving(joyPos.x, speed);
}
