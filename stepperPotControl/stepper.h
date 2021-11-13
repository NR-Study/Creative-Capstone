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
      : Took out the iteration number outside the loop initialization of the drive() function
      : Added a calibration function for setting the velocity constant
    > v 1.2.0 (Nov. 12, 2021)
      : Updated the calibration function so it can calibrate by the millisecond unit
      : Changed the interpolation order from first to second
    > v 1.3.0 (Nov. 13, 2021)
      : Updated the calibration function so it can calibrate without moving the stepper motor
    > v 1.3.1 (Nov. 13, 2021)
      : Integrated the calibration function to the constructor
      
*/
// file: stepper.h
#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#define DEBUG true
#define ALLOWED_TIME_ERROR 100 // microsecond

class stepper
{
  private:
    const int dirPin;
    const int stepPin;
    int stepsPerRevolution;
    int timeStep;
    int iteration;
    double velocity_coefficient[3] = {0, 0, 1};
    // Motor rotation speed may change depending on the MCU speed.
    long firstTick = 0;
    long secondTick = 0;
    double interval[3] = {};
  public:
    stepper(const int _dirPin, const int _stepPin, int _stepsPerRevolution);
    ~stepper();

    void drive(int angle, float _rpm);
    void drive(int angle);
    void calibrate();
};
