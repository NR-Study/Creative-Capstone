/*
  Stepper Motor Control Library
  Author: Won Bin Choi (B4)
  E-mail: dc07650@naver.com
  Organization: Sogang University(Korea, Republic of), Mechanical Engineering
  Date: November 12, 2021
*/
// file: stepper.cpp
#include "Arduino.h"
#include "stepper.h"

stepper::stepper(const int _dirPin, const int _stepPin, int _stepsPerRevolution)
  : dirPin(_dirPin), stepPin(_stepPin), stepsPerRevolution(_stepsPerRevolution)
{

}

stepper::~stepper()
{

}

void stepper::drive(int angle, float _rpm)
{
  timeStep = int((60.0 / (float(stepsPerRevolution) * pow(10, -6) * _rpm * velocity_coefficient[1]))/2.0); // RPM to time step
  byte comm = B00000000;
  if (angle > 0) {
    comm = 1<<dirPin | comm;
  }

  iteration = int(float(stepsPerRevolution) * float(abs(angle)) / 360.0);

  for (int i = 0; i < iteration; i ++) {
    PORTD = 1<<stepPin | comm;
    delayMicroseconds(timeStep);
    PORTD = comm;
    delayMicroseconds(timeStep);
  }
}

void stepper::drive_s(int angle, float _rpm, int _touchPin)
{
  timeStep = int((60.0 / (float(stepsPerRevolution) * pow(10, -6) * _rpm * velocity_coefficient[1]))/2.0); // RPM to time step
  byte comm = B00000000;
  DDRD = ~((~((1<<dirPin | 1<<stepPin))) | 1<<_touchPin) & (B11111100); // Setup an extra pin configuration for tactile
  
  if (angle > 0) {
    comm = 1<<dirPin | comm;
  }

  iteration = int(float(stepsPerRevolution) * float(abs(angle)) / 360.0);

  for (int i = 0; i < iteration; i ++) {
    if (!((PIND & 1<<_touchPin)>>_touchPin)) {
      PORTD = 1<<stepPin | comm;
      delayMicroseconds(timeStep);
      PORTD = comm;
      delayMicroseconds(timeStep);
    }
  }
}

void stepper::begin()
{
  DDRD = (1<<dirPin | 1<<stepPin) & (B11111100);
  calibrate();
}

void stepper::drive(int angle)
{
  timeStep = int((60.0 / (float(stepsPerRevolution) * pow(10, -6) * 60 * velocity_coefficient[1]))/2.0); // RPM to time step
  byte comm = B00000000;
  if (angle > 0) {
    comm = 1<<dirPin | comm;
  }
  
  iteration = int(float(stepsPerRevolution) * float(abs(angle)) / 360.0);
  for (int i = 0; i < iteration * 2; i ++) {
    PORTD = comm;
    delayMicroseconds(timeStep);
  }
}

void stepper::calibrate()
{ // Calibration is required for fast stepper motor response
  float calibrationSpeed = 36;
  float targetTime = calibrationSpeed / 360.0 * pow(10, 6);
  int counter = 0;
  
  firstTick = micros();
  drive(calibrationSpeed);
  secondTick = micros();
  interval[0] = secondTick - firstTick;

  velocity_coefficient[0] = velocity_coefficient[1];
  velocity_coefficient[1] *= 4.0;
  firstTick = micros();
  drive(calibrationSpeed);
  secondTick = micros();
  interval[2] = secondTick - firstTick;

  velocity_coefficient[2] = velocity_coefficient[1];
  velocity_coefficient[1] /= 2.0;
  while (counter < ITERATION_LIMIT) {
    firstTick = micros();
    drive(calibrationSpeed);
    secondTick = micros();
    interval[1] = secondTick - firstTick;
    //Serial.println("Vc: " + String(velocity_coefficient[1],5) + "\t t: " + String(interval[1],0));
    if (abs(interval[1] - targetTime) < ALLOWED_TIME_ERROR) {
      break;
    }
    velocity_coefficient[1] = ( ( ((targetTime - interval[1])) * ((targetTime - interval[2])) ) / ( ((interval[0] - interval[1])) * ((interval[0] - interval[2])) ) ) * velocity_coefficient[0] + ( ( ((targetTime - interval[0])) * ((targetTime - interval[2])) ) / ( ((interval[1] - interval[0])) * ((interval[1] - interval[2])) ) ) * velocity_coefficient[1] + ( ( ((targetTime - interval[0])) * ((targetTime - interval[1])) ) / ( ((interval[2] - interval[0])) * ((interval[2] - interval[1])) ) ) * velocity_coefficient[2];
    counter ++;
  }
}
