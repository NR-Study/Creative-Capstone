# include "stepper.h"

// Direction Pin, Step Pin, Steps Per Revolution
stepper motor(2, 3, 800);

// User Variables
const int analogInPin = A5;
int sensorValue = 0;
int outputValue = 0;

void setup(){
  Serial.begin(9600);
  
  motor.drive(-360, 60); // Spin Angle, Step Time (= timeStep)
  Serial.println(millis() - sensorValue);
  delay(1000);
  motor.calibrate(); // Automatically set the velocity coefficient
  sensorValue = millis();
  motor.drive(-360, 60); // Spin Angle, Step Time (= timeStep)
  Serial.println(millis() - sensorValue);
  delay(1000);
}

int oldValue = -180;

void loop(){
  // Get sensor value
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, -180, -98);

  // Check value change
  if (oldValue != outputValue){
    // Give incremental drive
    motor.drive(outputValue - oldValue, 120);
  }
  oldValue = outputValue; // Get old value
  delay(1); 
}
