#pragma once



class Steering {

  
  //Steering values
  const int BUTTONCALIB = 8;  //Pin of the max steering button
  bool calibState = false;    //calibration finished or not
  int steeringMin = 0;        // the minimal steering position
  int steeringMax = 0;        // the maximal steering position
  int currentSteering = 0;    // the current steering position
  const int STEPS = 10;       //amount of stepps, with which the motor will go on
  
  public:
    void init();
    int calibrationSingle(int steps, int readPin);
    void startCalibration();
    bool isCalibrated();
    void handleSteering(int joystickHorz);
    void detailCalib(int joystickHorz);
};
