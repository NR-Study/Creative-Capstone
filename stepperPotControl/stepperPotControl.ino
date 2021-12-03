# include "stepper.h"
# include <Servo.h>
#define MIN_PULSE_LENGTH 1000 // Minimum pulse length in µs
#define MAX_PULSE_LENGTH 2000 // Maximum pulse length in µs

// Direction Pin, Step Pin, Steps Per Revolution
stepper motor(2, 3, 800);
Servo motA;

// User Variables
const uint8_t analogInPin = A5;
uint16_t sensorValue = 0;
int16_t outputValue = 0;
int16_t command = 0;


void setup() {
  Serial.begin(9600);
  
  motA.attach(9, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  
  motA.writeMicroseconds(MAX_PULSE_LENGTH);
  delay(1000);
  motA.writeMicroseconds(MIN_PULSE_LENGTH);
  delay(1000);
  
  motor.begin(); // Initiate stepper motor
  motor.drive_s(-360, 60, 7); // Spin Angle, Step Time (= timeStep)
}

int16_t oldValue = 0;
int16_t angle = 0;

void loop() {
    // Get sensor value
    sensorValue = analogRead(analogInPin);
    outputValue = map(sensorValue, 0, 1023, 1000, 2000);
    
    motA.writeMicroseconds(outputValue);
        
    if (Serial.available() > 0)
    {
      command = Serial.parseInt();
      command = map(command, 0, 100, 0, 80);
      Serial.parseInt();
      Serial.flush();
      // Check value change
      if (command > 80 || command < 0)
      {
        Serial.println("ERROR - Angle Range Exceeded");
      } 
      else if (command != oldValue) // Absolute Drive
      {
        // Give incremental drive
        motor.drive(command - oldValue, 120);
        angle += command - oldValue;
        oldValue = command; // Get old value
      }
      Serial.println(angle);
    }
    delay(1); 
}
