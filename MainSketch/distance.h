#pragma once

class Distance {
  int echo = 11;
  int trigger = 12;
  int distance; //distance of detector
  bool barrierState = false; //is barrier present?
  
  public: 
  int currentDistance();
  bool barrier();
  void stopIf(Driving& driving, int maxDistance);
  void init(int echoPin, int triggerPin);
};
