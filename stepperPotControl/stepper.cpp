/*
 Stepper Motor Control Library
 Author: Won Bin Choi (B4)
 E-mail: dc07650@naver.com
 Organization: Sogang University(Korea, Republic of), Mechanical Engineering
 Date: November 12, 2021
*/
// file: stepper.cpp
#include "stepper.h"

stepper::stepper(const int _dirPin, const int _stepPin, int _stepsPerRevolution)
:dirPin(_dirPin), stepPin(_stepPin), stepsPerRevolution(_stepsPerRevolution)
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

stepper::~stepper()
{
  
}

void stepper::drive(int angle, float _rpm)
{
  timeStep = int(60.0/(float(stepsPerRevolution)*pow(10,-6)*_rpm*velocity_coefficient)); // RPM to time step
  
  if(angle > 0) {
    digitalWrite(dirPin, HIGH);
  } else {
    digitalWrite(dirPin, LOW);
  }
  
  iteration = int(float(stepsPerRevolution) * float(abs(angle))/360.0);
    
  for(int i = 0; i < iteration; i ++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(timeStep);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(timeStep);
  }
}

void stepper::calibrate()
{
  float calibrationSpeed = 36;
  firstTick = millis();
  drive(calibrationSpeed, 60);
  secondTick = millis();
  pastInterval = secondTick - firstTick;

  velocity_coefficient_p = velocity_coefficient;
  velocity_coefficient *= 4.0;
  while(true){
    firstTick = millis();
    drive(calibrationSpeed, 60);
    secondTick = millis();
    currentInterval = secondTick - firstTick;
    if (currentInterval == calibrationSpeed/360.0 * pow(10,3)) {
      break;
    }
    velocity_coefficient = float( (currentInterval * velocity_coefficient_p - pastInterval * velocity_coefficient + calibrationSpeed/360.0 * pow(10,3) * ( velocity_coefficient - velocity_coefficient_p ) ) ) / float(currentInterval - pastInterval);
  }
}
