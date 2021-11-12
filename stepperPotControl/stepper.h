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
    > v 1.1.0 (Nov. 12, 2021)
      : Changed velocity variable to timeStep
      : Added a conversion command to change from rpm to time step
      : Added DEBUG definition for debugging
      : Added velocity coefficient for processor adjustment
      : Took out the iteration number outside the loop initilization of the drive() function
      : Added a calibration function for setting the velocity constant
*/
// file: stepper.h
#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#define DEBUG true
#define ALLOWED_TIME_ERROR 0 // microsecond

class stepper
{
  private:
    const int dirPin;
    const int stepPin;
    int stepsPerRevolution;
    int timeStep;
    int iteration;
    float velocity_coefficient = 1.0; 
    float velocity_coefficient_p = 0; 
    // Motor rotation speed may change depending on the MCU speed.
    long firstTick = 0;
    long secondTick = 0;
    long currentInterval = 0;
    long pastInterval = 0;
  public:
    stepper(const int _dirPin, const int _stepPin, int _stepsPerRevolution);
    ~stepper();

    void drive(int angle, float _rpm);
    void calibrate();
};
