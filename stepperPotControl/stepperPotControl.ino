# include "stepper.h"
// Direction Pin, Step Pin, Steps Per Revolution
stepper motor(2, 3, 800);

// User Variables
const uint8_t analogInPin = A5;
uint16_t sensorValue = 0;
int16_t outputValue = 0;
int16_t command = 0;


void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  motor.begin(); // Initiate stepper motor
  motor.drive_s(-360, 60, 7); // Spin Angle, Step Time (= timeStep)
}

int16_t oldValue = 0;
int16_t angle = 0;

void loop() {
    // Get sensor value
    sensorValue = analogRead(analogInPin);
    outputValue = map(sensorValue, 0, 1023, 0, 255);
    
    analogWrite(9, outputValue);
        
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
