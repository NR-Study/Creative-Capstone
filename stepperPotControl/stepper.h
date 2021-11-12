/*
 Stepper Motor Control Library
 Author: Won Bin Choi (B4)
 E-mail: dc07650@naver.com
 Organization: Sogang University(Korea, Republic of), Mechanical Engineering
 Date: November 12, 2021
 - ChangeLog - (Major.Minor.Bug)
    > v 1.0.0 (Nov. 12, 2021)
      : Created Stepper Motor Control Library
      : Added motor drive function (Contains direction determination based on given angle value)
*/
// file: stepper.h
#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

class stepper
{
  private:
    const int dirPin;
    const int stepPin;
    int stepsPerRevolution;
    int velocity;
  
  public:
    stepper(const int _dirPin, const int _stepPin, int _stepsPerRevolution);
    ~stepper();

    drive(int angle, int _velocity);
};