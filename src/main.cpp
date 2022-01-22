#include <Arduino.h>
#include "AnalogJoystick.h"
AnalogJoystick j;
void setup() {
// write your initialization code here

Serial.begin(115200);
j.init(36, 39, 9, 1);
//j.setCalculateVoltage(1);
}

void loop() {
// write your code here

    //Serial.printf("RAW X is %d Vol is %f\n", j.getRawX(), j.getXVoltage());
    //Serial.printf("RAW Y is %d Vol is %f\n", j.getRawY(), j.getYVoltage());
    //delay(500);
    j.getRawX();
    Serial.print("\t");
    j.getRawY();
    Serial.println(" ");
    delay(500);
}