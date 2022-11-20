#include <SPI.h>  // Not actualy used but needed to compile

//uncluding needed subfiles
#include "steering.h"
#include "driving.h"
#include "distance.h"
#include "MessageHandler.h"

MessageHandler messageHandler;
HardwareSerial& serialCom = Serial3;

int speedVal = 200;  //this will be the speed variable, which can be controled by remote device TEMPORARY

int temp = 0;

int joyVert;
int joyHorz;

//defining of different subclasses
Steering steering;
Driving driving;
Distance distance;

void setup() {
  //initalizing subclasses, to set pinModes or Pins
  steering.init();
  driving.init();
  distance.init(50, 51);

  Serial.begin(115200);
  serialCom.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  messageHandler.pollMessage(serialCom);
  if (messageHandler.isMessageAvailable()) {
  
    
    Serial.print("Message available");

    char id;
    const char* data = messageHandler.getMessage(&id);
    Serial.println(id);
    // handle message
    switch (id) {
      case '1': {
          Serial.println(data);
          //messageHandler.sendMessage(Serial3, id, data);
          sscanf(data, "%d;%d;%d", &joyHorz, &joyVert, &speedVal);
          Serial.println(joyHorz);
          //driving process of car; with current position of joystick and speed of engines
          serialCom.end();
          distance.stopIf(5);
          driving.handleDriving(joyVert, speedVal / 4, distance);
          if (steering.isCalibrated() == true) {
            steering.handleSteering(joyHorz);
          }
          serialCom.begin(9600);
       
          break;
        }
      case '2':
        {
          serialCom.end();
          Serial.println("Calibration Starting");
          steering.startCalibration();  //starting calibration
          messageHandler.sendMessage(Serial2 , 'a', " ");  
          serialCom.begin(9600); 
          break;
        }
      case '4':
        {
          //serialCom.end();
          tone(24, 200, 1000);
          //serialCom.begin(9600);
          break;
        }
      case '9':
        {
          Serial.println("\n");
          Serial.println(data);
          break;
        }
    }
    //Serial.println("Horz: " + String(joyHorz) + " Vert: " + String(joyVert) + " Speed: " + String(speedVal/4) + " Cal: " + String(0) + " Horn: " + String(0));
  }


  //messageHandler.sendMessage(Serial2, 1, "");
  //delay(500);
  //Serial.println("data send");
  //messageHandler.sendMessage(Serial1, 1, "");
  //Serial.println("data send");
  //delay(500);


  //calibrates the steering of remote button pressed and wasn't calibrated as far

  //x = up/down; y = left/right
  //ggf. while Schleife
  //  if (calibStateRem == true && steering.isCalibrated()) {
  //    steering.detailCalib(joyPos.y);
  //  }

  //egine will be stopped if distance detector has got an distance smaler ore equal to 5cm
  //distance.stopIf(5);

  //steering process of car; with current position of joystick
  //steering.handleSteering(joyPos.y);
}