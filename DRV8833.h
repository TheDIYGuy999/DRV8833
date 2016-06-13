/*
  DRV8833.h - Library for the Texas Instruments DRV8833 motor driver.
  Created by TheDIYGuy999 June 2016
  Released into the public domain.
*/

#ifndef DRV8833_h
#define DRV8833_h

#include "Arduino.h"

// Class definition (header) ========================================================================
class DRV8833 {
  public:
    DRV8833(int pin1, int pin2, int minInput, int maxInput, int neutralWidth, boolean invert, boolean doublePWM);
    void drive(int controlValue, int maxPWM, int rampTime, boolean brake, boolean neutralBrake);

  private:
    int _pin1;
    int _pin2;
    int _minInput;
    int _maxInput;
    int _minNeutral;
    int _maxNeutral;
    int _controlValue;
    int _controlValueRamp = 50;
    int _maxPWM;
    int _rampTime;
    boolean _brake;
    boolean _neutralBrake;
    boolean _invert;
    boolean _doublePWM;
    unsigned long _previousMillis = 0;
    byte _state = 0;
};

#endif
