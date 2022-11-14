
#include <SPI.h>  // Not actualy used but needed to compile

//uncluding needed subfiles
#include "steering.h" 
#include "driving.h"
#include "distance.h"
#include "MessageHeader.h"

MessageHandler messageHandler;
MessageHandler messageHandler3; //serial 3


int speedVal = 200;  //this will be the speed variable, which can be controled by remote device TEMPORARY

int temp = 0;

int joyVert;
int joyHorz;



//defining of different subclasses
Steering steering; 
Driving driving;
Distance distance;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  
  
  //initalizing subclasses, to set pinModes or Pins
  steering.init();  
  driving.init();
  distance.init(11, 12);
}

void loop() {
    //Serial.println("loop starting");
    messageHandler.pollMessage(Serial1);
    if (messageHandler.isMessageAvailable()) {

      Serial.println("Message available");
      byte id;
      const char* data = messageHandler.getMessage(&id);

      // handle message
      switch (id) {
        case 1: {
          messageHandler3.sendMessage(Serial3, id, data);
          sscanf(data, "%d;%d;%d", &joyHorz, &joyVert, &speedVal);
          Serial.println("data got");  
          Serial.println(data);
          //driving process of car; with current position of joystick and speed of engines
          Serial1.end();
          driving.handleDriving(joyVert, 200); 
          steering.handleSteering(joyHorz);
          Serial1.begin(9600);                        
          break;}
        case 3: {
          speedVal = data;
        break;}       
        case 4: {
          Serial1.end();
          Serial.println("Calibration Starting");
          steering.startCalibration(); //starting calibration
          Serial1.begin(9600);   
          //for(int i = 0; i = 5; i++) {
          //messageHandler.sendMessage(Serial1, 1, "");
          //Serial.println("data send"); 
          //delay(500);
          //};                    
        break;} 
        case 5: {
          tone(24, 200, 1000);
        break;}                 
       }
      Serial.println("Horz: " + String(joyHorz) + " Vert: " + String(joyVert) + " Speed: " + String(speedVal/4) + " Cal: " + String(0) + " Horn: " + String(0));
    }
  

     // messageHandler.sendMessage(Serial1, 1, "");
      //Serial.println("data send"); 
      //delay(500);                 
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
