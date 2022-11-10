#pragma once

class Driving {
  const int ENGINE_LEFT = 41; // -> set on init?
  const int ENGINE_RIGHT = 42;
  const int BACKWARD_LEFT = 33;
  const int FORWARD_LEFT = 34;
  const int BACKWARD_RIGHT = 35;
  const int FORWARD_RIGHT = 36;

  int enginePins[6]{BACKWARD_LEFT, FORWARD_LEFT, BACKWARD_RIGHT, FORWARD_RIGHT, ENGINE_LEFT, ENGINE_RIGHT};
  
  public:
    void init();
    void forward(int speed);
    void backward(int speed);
    void stop();
    void stopForward();
    void handleDriving(int joystickVert, int speed);
};
