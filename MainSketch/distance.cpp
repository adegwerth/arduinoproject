#include <Arduino.h>
#include "distance.h"
#include "driving.h"

//getting current distance to barrier from distance detector
int Distance::currentDistance() {
  int duration = 0;

  digitalWrite (trigger, LOW);  //setting pin low to make sure it's off
  delay (5);                    
  digitalWrite (trigger, HIGH); //sending out ultrasonic wave
  delay (10);
  digitalWrite (trigger, LOW);  //turn sending of


  ///TODO: MAKE SURE THAT BARRIER IS NOT TO FAR IN DISTANCE ELSE THIS STEP WOULD NEED A VERY LONG TIME -> SETTING MAX. TIME UNTIL SIGNAL BACK
  duration = pulseIn(echo, HIGH); //time until signal got in
  return (duration/2) *0.03432; //dividing duration with two because we only want time of one distance; mutiplying with speed of sonicwaves to get distance in cm
 
  return distance;  //giving distance back
}

bool Distance::barrier() {
  return barrierState;  //returns if barrier is present or not
}

void Distance::stopIf(Drivin& driving, int maxDistance){ 
  if (currentDistance() <= maxDistance){  //stops car if distance of detector smaller ore equal to maxDistance
    driving.stopForward(); //stops driving process of car if it's driving foreward
    barrierState = true; //barrier is present
  } else {
    barrierState = false; //if not barrier is unpresent
  }
}

void Distance::init(int echoPin, int triggerPin) {
  //defining Pins for distance detector
  echo = echoPin; 
  trigger = triggerPin;
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
}