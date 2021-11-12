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

stepper::drive(int angle, int _velocity)
{
  velocity = _velocity;
  
  if(angle > 0) {
    digitalWrite(dirPin, HIGH);
  } else {
    digitalWrite(dirPin, LOW);
  }
  
  for(int i = 0; i < int(float(stepsPerRevolution) * float(abs(angle))/360.0); i ++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(velocity);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(velocity);
  }
}