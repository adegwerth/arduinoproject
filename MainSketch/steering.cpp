#include <Arduino.h>
#include <Stepper.h>
#include "steering.h"

Stepper StepEn(2048, 3, 5, 4, 6);

void Steering::init() {
  pinMode(BUTTONCALIB, INPUT);
  StepEn.setSpeed(10);
}

int Steering::calibrationSingle(int steps, int readPin) {
  int stepCount = 0;
  while (digitalRead(readPin) == 0) {  //as long as the button from the max ountput is not pressed the stepper will step to the given side
    StepEn.step(steps);
    stepCount += steps;
  }

  StepEn.step(-stepCount);  //engine gets back to start position

  return stepCount;  //returning stepCount
}

void Steering::startCalibration() {
  int stepCountRight = calibrationSingle(-STEPS, BUTTONCALIB);
  Serial.println("StepCountRight: " + String(stepCountRight));
  
  int stepCountLeft = calibrationSingle(STEPS, BUTTONCALIB);
  Serial.println("StepCountLeft: " + String(stepCountLeft));

  

  int stepsAll = abs(stepCountLeft) + abs(stepCountRight);
  Serial.println("StepsAll: " + String(stepsAll));

  //if (abs(stepCountLeft) >= abs(stepCountRight)) {
    int centre = (stepCountLeft + stepCountRight) / 2;  //subtracting the number to get the amount of stepps
    Serial.println("Mitte: " + String(centre));
    StepEn.step(centre);  //turn stepper the amount of steps to the right side
  //}

  steeringMax = stepsAll / 2;
  steeringMin = -(stepsAll / 2);


  calibState = true;  //calibration process is done
   
}

bool Steering::isCalibrated() {
  return calibState;
}

void Steering::handleSteering(int joystickHorz) {  
  if (joystickHorz > 600 && currentSteering == 0){
    //if ((currentSteering - STEPS) < steeringMax) {  
      StepEn.step(-(steeringMax - 50) );
      currentSteering += (steeringMax -50);
      
    //}
  } else if (joystickHorz < 400 && currentSteering == 0) {
    //if ((currentSteering + STEPS) > steeringMin) {
      StepEn.step(- (steeringMin + 50));
      currentSteering += steeringMin + 50;
    //}
  } else if (currentSteering != 0 && !(joystickHorz > 600) && !(joystickHorz < 400)) {
      StepEn.step(currentSteering);
      Serial.println("mitte");
      currentSteering = 0;
  }; 
  
  Serial.println("joy: " + String(joystickHorz) + " min: " + String(steeringMin)  + " max: " + String(steeringMax) + " cur: " + String(currentSteering));  
}

void Steering::detailCalib(int joystickHorz) {
  if (joystickHorz > 600) {
    StepEn.step(-1);    
  }
  if (joystickHorz < 600) {
    StepEn.step(1);
  }
}
