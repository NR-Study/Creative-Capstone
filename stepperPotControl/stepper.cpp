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
  timeStep = int(60.0/(float(stepsPerRevolution)*pow(10,-6)*_rpm*velocity_coefficient[2])); // RPM to time step
  
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
  float targetTime = calibrationSpeed/360.0 * pow(10,6);

  firstTick = micros();
  drive(calibrationSpeed, 60);
  secondTick = micros();
  interval[0] = secondTick - firstTick;
  
  velocity_coefficient[0] = velocity_coefficient[2];
  velocity_coefficient[2] *= 2.0;
  firstTick = micros();
  drive(calibrationSpeed, 60);
  secondTick = micros();
  interval[1] = secondTick - firstTick;

  velocity_coefficient[1] = velocity_coefficient[2];
  velocity_coefficient[2] *= 2.0;
  while(true){
    firstTick = micros();
    drive(calibrationSpeed, 60);
    secondTick = micros();
    interval[2] = secondTick - firstTick;
    Serial.println(interval[2]);
    if (abs(interval[2] - targetTime) < ALLOWED_TIME_ERROR) {
      break;
    }
    velocity_coefficient[2] = ( ( ((targetTime - interval[1]))*((targetTime - interval[2])) ) / ( ((interval[0] - interval[1]))*((interval[0] - interval[2])) ) )* velocity_coefficient[0] + ( ( ((targetTime - interval[0]))*((targetTime - interval[2])) ) / ( ((interval[1] - interval[0]))*((interval[1] - interval[2])) ) )* velocity_coefficient[1] + ( ( ((targetTime - interval[0]))*((targetTime - interval[1])) ) / ( ((interval[2] - interval[0]))*((interval[2] - interval[1])) ) )* velocity_coefficient[2];
  }
}
