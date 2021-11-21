# include "stepper.h"
# include <TimeInterrupt.h>
// Direction Pin, Step Pin, Steps Per Revolution
stepper motor(2, 3, 800);

// User Variables
const uint8_t analogInPin = A5;
uint16_t sensorValue = 0;
int16_t outputValue = 0;

// Time Variables
bool timeFlag = true;

void setup() {
  Serial.begin(115200);

  motor.begin(); // Initiate stepper motor
  sensorValue = millis();
  motor.drive_s(-360, 60, 7); // Spin Angle, Step Time (= timeStep)
  Serial.println(millis() - sensorValue);
  delay(1000);
  TimeInterrupt.begin(PRECISION);
  TimeInterrupt.addInterrupt(timedog, 5);
}

int16_t oldValue = -180;

void loop() {
  if (timeFlag)
  {
    // Get sensor value
    sensorValue = analogRead(analogInPin);
    outputValue = map(sensorValue, 0, 1023, -180, -98);

    // Check value change
    if (oldValue != outputValue) {
      // Give incremental drive
      motor.drive(outputValue - oldValue, 120);
    }
    oldValue = outputValue; // Get old value
    timeFlag = !(timeFlag);
  }
}

void timedog ()
{
  timeFlag = !(timeFlag);
}
