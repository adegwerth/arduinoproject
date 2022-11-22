#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Display : public LiquidCrystal_I2C {
  const char strWait[4] = {
    '|',
    '/',
    '-',
    0,
  };

  byte customCharBackslash[8] = {
    B00000,
    B10000,
    B01000,
    B00100,
    B00010,
    B00001,
    B00000,
    B00000
  };

  unsigned long DISPLAY_CLEAN = 5000;
  unsigned long timeLastClean = 0;

  unsigned long lastTimeWaitIndicator = 0;
  int curWaitIndicator = 0;
  

  public: 
    Display(int, int, int);
    void init();
    void print(int row, int col, const char* msg, bool clean = false);
    void printWait();
    void displayClear();
    void progressIndicator();
};

