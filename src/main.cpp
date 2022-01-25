#include <Arduino.h>
#include "AnalogJoystick.h"
AnalogJoystick j;
void setup() {
// write your initialization code here

Serial.begin(115200);
j.init(36, 39, 10, 150);
//j.setCalculateVoltage(1);
}

void loop() {
    j.calibrate(150);
    while (1) {
        j.printState();
        delay(1000);
    }
}