/*
  DRV8833.h - Library for the Texas Instruments DRV8833 motor driver.
  Created by TheDIYGuy999 June 2016
  Released into the public domain.
*/

#include "Arduino.h"
#include "DRV8833.h"

// Member definition (code) ========================================================================

// NOTE: The first pin must always be PWM capable, the second only, if the last parameter is set to "true"
// SYNTAX: IN1, IN2, min. input value, max. input value, neutral position width
// invert rotation direction, true = both pins are PWM capable
DRV8833::DRV8833(int pin1, int pin2, int minInput, int maxInput, int neutralWidth, boolean invert, boolean doublePWM) { // Constructor
  _pin1 = pin1;
  _pin2 = pin2;
  _minInput = minInput;
  _maxInput = maxInput;
  _minNeutral = (_maxInput + _minInput) / 2 - (neutralWidth / 2);
  _maxNeutral = (_maxInput + _minInput) / 2 + (neutralWidth / 2);
  _controlValueRamp = (_minNeutral + _maxNeutral) / 2;
  _invert = invert;
  _doublePWM = doublePWM;
  _state = 0;
  _previousMillis = 0;
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  digitalWrite(_pin1, LOW);
  digitalWrite(_pin2, LOW);
}

// Drive function ************************************************************

// SYNTAX: Input value, max PWM, ramptime in ms per 1 PWM increment
// true = brake active, false = brake in neutral position inactive
void DRV8833::drive(int controlValue, int maxPWM, int rampTime, boolean brake, boolean neutralBrake) {
  _controlValue = controlValue;
  _maxPWM = maxPWM;
  _rampTime = rampTime;
  _brake = brake;
  _neutralBrake = neutralBrake;

  if (_invert) {
    _controlValue = map (_controlValue, _minInput, _maxInput, _maxInput, _minInput); // invert driving direction
  }

  // Fader (allows to ramp the motor speed slowly up & down) --------------------
  unsigned long currentMillis = millis();
  if (currentMillis - _previousMillis >= _rampTime) {
    // Increase
    if (_controlValue > _controlValueRamp && _controlValueRamp < _maxInput) {
      _controlValueRamp++;
    }
    // Decrease
    if (_controlValue < _controlValueRamp && _controlValueRamp > _minInput) {
      _controlValueRamp--;
    }
    _previousMillis = currentMillis;
  }

  // H bridge controller -------------------
  if (_doublePWM) { // Mode with two PWM capable pins (both pins must be PWM capable!) -----
    if (!_brake) { // Coast mode (fast decay)
      if (_controlValueRamp >= _maxNeutral) { // Forward
        digitalWrite(_pin1, LOW);
        analogWrite(_pin2, map(_controlValueRamp, _maxNeutral, _maxInput, 0, _maxPWM));
      }
      else if (_controlValueRamp <= _minNeutral) { // Reverse
        digitalWrite(_pin2, LOW);
        analogWrite(_pin1, map(_controlValueRamp, _minNeutral, _minInput, 0, _maxPWM));
      }
      else { // Neutral
        digitalWrite(_pin1, LOW);
        digitalWrite(_pin2, LOW);
      }
    }
    else { // Brake mode (slow decay)
      if (_controlValueRamp >= _maxNeutral) { // Forward
        digitalWrite(_pin2, HIGH);
        analogWrite(_pin1, 255 - map(_controlValueRamp, _maxNeutral, _maxInput, 0, _maxPWM));
      }
      else if (_controlValueRamp <= _minNeutral) { // Reverse
        digitalWrite(_pin1, HIGH);
        analogWrite(_pin2, 255 - map(_controlValueRamp, _minNeutral, _minInput, 0, _maxPWM));
      }
      else { // Neutral
        if (_neutralBrake) {
          digitalWrite(_pin1, HIGH); // Brake in neutral position active
          digitalWrite(_pin2, HIGH);
        }
        else {
          digitalWrite(_pin1, LOW); // Brake in neutral position inactive
          digitalWrite(_pin2, LOW);
        }
      }
    }
  }
  else { // Mode with only one PWM capable pin (pin 1 = PWM, pin2 = direction) -----
    // NOTE: the brake is always active in one direction and always inactive in the other!
    // Only use this mode, if your microcontroller does not have enough PWM capable pins!
    // If the brake is active in the wrong direction, simply switch both motor wires and
    // change the "invert" boolean!
    if (_controlValueRamp >= _maxNeutral) { // Forward
      digitalWrite(_pin2, HIGH);
      analogWrite(_pin1, 255 - map(_controlValueRamp, _maxNeutral, _maxInput, 0, _maxPWM));
    }
    else if (_controlValueRamp <= _minNeutral) { // Reverse
      digitalWrite(_pin2, LOW);
      analogWrite(_pin1, map(_controlValueRamp, _minNeutral, _minInput, 0, _maxPWM));
    } else { // Neutral
      digitalWrite(_pin1, HIGH);
      digitalWrite(_pin2, HIGH);
    }
  }
}

