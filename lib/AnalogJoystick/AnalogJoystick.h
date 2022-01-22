//
// Created by Vlad on 20.01.2022.
//

#ifndef UNTITLED123123_ANALOGJOYSTICK_H
#define UNTITLED123123_ANALOGJOYSTICK_H
#include "Arduino.h"
#include "driver/adc.h"

#define NUM_OF_SAMPLES 20

typedef struct{
    int16_t val; // - 2047 .. +2047
    double voltage=0;
} Position;
class AnalogJoystick {
public:
    AnalogJoystick(){};
    AnalogJoystick(uint8_t x_pin, uint8_t y_pin, uint8_t resolution = 12, bool calculateVoltage=0) {
        init(x_pin, y_pin, resolution, calculateVoltage);

    };
    void init (uint8_t x, uint8_t y, uint8_t resolution, bool calculateVoltage);
    int16_t getRawX() { measure(_ADC_ChannelX, &_posX); return _posX.val;};
    int16_t getRawY(){measure(_ADC_ChannelY, &_posY); return _posY.val;};
    float getXVoltage(){/* measure(_ADC_ChannelX, &_posX);*/ return _posX.voltage;};
    float getYVoltage(){ /*measure(_ADC_ChannelY, &_posY);*/ return _posY.voltage;};
    bool isCalculateVoltage() const;
    void setCalculateVoltage(bool calculateVoltage);






private:
    uint8_t _pinX;
    uint8_t _pinY;
    typeof(ADC1_CHANNEL_MAX) _ADC_ChannelX;
    typeof(ADC1_CHANNEL_MAX) _ADC_ChannelY;

    Position _posX;
    Position _posY;
    bool _calculateVoltage;
    uint8_t _ADC_Resolusion;

    uint16_t abs_coordinate; // -abs_coord ... + abs_coord on X and Y
    uint16_t maxAdcValue;

    uint16_t deadRadius;

    void measure(adc1_channel_t ch, Position* pos) const;
    void calibrate();


};


#endif //UNTITLED123123_ANALOGJOYSTICK_H
