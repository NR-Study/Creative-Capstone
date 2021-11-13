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
  timeStep = int(60.0 / (float(stepsPerRevolution) * pow(10, -6) * _rpm * velocity_coefficient[1])); // RPM to time step

  if (angle > 0) {
    digitalWrite(dirPin, HIGH);
  } else {
    digitalWrite(dirPin, LOW);
  }

  iteration = int(float(stepsPerRevolution) * float(abs(angle)) / 360.0);

  for (int i = 0; i < iteration; i ++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(timeStep);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(timeStep);
  }
}

void stepper::begin()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  calibrate();
}

void stepper::drive(int angle)
{  
  timeStep = int(60.0 / (float(stepsPerRevolution) * pow(10, -6) * 60 * velocity_coefficient[1])); // RPM to time step (Calibrate to 60 RPM)
  
  digitalWrite(dirPin, LOW);

  iteration = int(float(stepsPerRevolution) * float(abs(angle)) / 360.0);
  for (int i = 0; i < iteration * 2; i ++) {
    digitalWrite(13, HIGH);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(timeStep);
    digitalWrite(13, LOW);
  }
}

void stepper::calibrate()
{
  float calibrationSpeed = 36;
  float targetTime = calibrationSpeed / 360.0 * pow(10, 6);
  
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
  while (true) {
    firstTick = micros();
    drive(calibrationSpeed);
    secondTick = micros();
    interval[1] = secondTick - firstTick;
    //Serial.println("Vc: " + String(velocity_coefficient[1],5) + "\t t: " + String(interval[1],0));
    if (abs(interval[1] - targetTime) < ALLOWED_TIME_ERROR) {
      break;
    }
    velocity_coefficient[1] = ( ( ((targetTime - interval[1])) * ((targetTime - interval[2])) ) / ( ((interval[0] - interval[1])) * ((interval[0] - interval[2])) ) ) * velocity_coefficient[0] + ( ( ((targetTime - interval[0])) * ((targetTime - interval[2])) ) / ( ((interval[1] - interval[0])) * ((interval[1] - interval[2])) ) ) * velocity_coefficient[1] + ( ( ((targetTime - interval[0])) * ((targetTime - interval[1])) ) / ( ((interval[2] - interval[0])) * ((interval[2] - interval[1])) ) ) * velocity_coefficient[2];
  }
}
