#include "Arduino.h"
#include "driver/adc.h"

#define NUM_OF_ADC_SAMPLES 20
#define NUM_OF_DELTA_SAMPLES 100
#define DEBUG

enum JOYSTICK_POS{
    UP = 1,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT,
    IDLE_DEADZONE
};
class AnalogJoystick {
public:
    AnalogJoystick(){};
    AnalogJoystick(uint8_t x_pin, uint8_t y_pin, uint8_t resolution = 10, uint16_t deadzoneDeviation =  150) {
        init(x_pin, y_pin, resolution, deadzoneDeviation);

    };
    void init (uint8_t x_pin, uint8_t y_pin, uint8_t resolution, uint16_t deadzoneDeviation);
    int16_t getRawX1() const;
    int16_t getRawY1() const;
    void calibrate(uint16_t deadzoneDeviation);
    void printState();

private:
    uint8_t _pinX;
    uint8_t _pinY;
    typeof(ADC1_CHANNEL_MAX) _ADC_ChannelX;
    typeof(ADC1_CHANNEL_MAX) _ADC_ChannelY;
    uint8_t _ADC_Resolusion;
    uint16_t _abs_coordinateMax; // -abs_coord ... + abs_coord are bounds on X and Y
    uint16_t _maxAdcValue;
    uint16_t _deadzoneDeviation;
    int16_t _deltaX;
    int16_t _deltaY;

    int16_t  ADCMeasure(adc1_channel_t ch) const;
    void calculateDeltaXY();
    void setupAbsMaxCoordinates();
};

