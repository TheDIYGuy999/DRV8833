// This example was cuccessfully tested on an Sparkfun Pro Micro in 16MHz version

//
// =======================================================================================================
// INCLUDE LIRBARIES
// =======================================================================================================
//
#include <DRV8833.h> // https://github.com/TheDIYGuy999/DRV8833

// Optional for motor PWM frequency adjustment:
//#include <PWMFrequency.h> // https://github.com/kiwisincebirth/Arduino/tree/master/libraries/PWMFrequency

//
// =======================================================================================================
// CREATE MOTOR OBJECTS
// =======================================================================================================
//

// Initialize DRV8833 H-Bridge
#define motor1_in1 5 // define motor pin numbers
#define motor1_in2 6
#define motor2_in1 9
#define motor2_in2 10

// NOTE: The first pin must always be PWM capable, the second only, if the last parameter is set to "true"
// SYNTAX: IN1, IN2, min. input value, max. input value, neutral position width
// invert rotation direction, true = both pins are PWM capable
DRV8833 Motor1(motor1_in1, motor1_in2, 0, 1023, 60, false, true); // Motor 1
DRV8833 Motor2(motor2_in1, motor2_in2, 0, 1023, 60, false, true); // Motor 2

//
// =======================================================================================================
// DRIVE MOTOR
// =======================================================================================================
//

void driveMotor() {

  int speed1 = 512;
  int speed2 = 512;
  
  // Read Potentiometer
  speed1 = analogRead(A0); // 0 - 1023
  speed2 = analogRead(A1); // 0 - 1023

  // ***************** Note! The ramptime is intended to protect the gearbox! *******************
  // SYNTAX: Input value, max PWM, ramptime in ms per 1 PWM increment
  // true = brake active, false = brake in neutral position inactive
  Motor1.drive(speed1, 255, 7, true, false); // Motor 1
  Motor2.drive(speed2, 255, 1, false, false); // Motor 2
}

//
// =======================================================================================================
// MAIN ARDUINO SETUP (1x during startup)
// =======================================================================================================
//
void setup() {
  // Optional: Motor PWM frequency (Requires the PWMFrequency.h library)
/*setPWMPrescaler(motor1_in1, 8); // 123Hz = 256,  492Hz = 64, 3936Hz = 8, 31488Hz = 1
  setPWMPrescaler(motor1_in2, 8);
  setPWMPrescaler(motor2_in1, 8);
  setPWMPrescaler(motor2_in2, 8); */
}

//
// =======================================================================================================
// MAIN LOOP
// =======================================================================================================
//

void loop() {
  // Drive the main motor
  driveMotor();
}
