//
// Created by Vlad on 20.01.2022.
//

#include "AnalogJoystick.h"


void AnalogJoystick::init (uint8_t x, uint8_t y, uint8_t resolution, bool calculateVoltage){

    if (x != 36 && x!=39 && x!= 34 && x!=35 && x!= 32 && x!= 33){
        Serial.println("X pin is not ADC1");
        return;
    }
    if (y != 36 && y!=39 && y!= 34 && y!=35 && y!= 32 && y!= 33){
        Serial.println("Y pin is not ADC1");
        return;
    }
    if (x == y) { Serial.println("X === Y"); return;}

    _pinX = x;
    _pinY = y;
    pinMode(_pinX, OUTPUT);
    pinMode(_pinY, OUTPUT);
    adc_bits_width_t t;
    switch (resolution) {
        case 12: t = ADC_WIDTH_BIT_12; break;
        case 11: t = ADC_WIDTH_BIT_11; break;
        case 10: t=ADC_WIDTH_BIT_10; break;
        case 9: t=ADC_WIDTH_BIT_9; break;
        default: {Serial.println("Wrong resolution; Set max"); t=ADC_WIDTH_MAX;}
    }
    adc1_config_width(t);
    switch (_pinX) {
        case GPIO_NUM_36: _ADC_ChannelX = ADC1_GPIO36_CHANNEL; break;
        case GPIO_NUM_39: _ADC_ChannelX = ADC1_GPIO39_CHANNEL; break;
        case GPIO_NUM_34: _ADC_ChannelX=ADC1_GPIO34_CHANNEL; break;
        case GPIO_NUM_35: _ADC_ChannelX=ADC1_GPIO35_CHANNEL; break;
        case GPIO_NUM_32: _ADC_ChannelX=ADC1_GPIO32_CHANNEL; break;
        case GPIO_NUM_33: _ADC_ChannelX=ADC1_GPIO33_CHANNEL; break;
        default:{Serial.println("FuckX"); break;}
    }
    switch (_pinY) {
        case GPIO_NUM_36: _ADC_ChannelY = ADC1_GPIO36_CHANNEL; break;
        case GPIO_NUM_39: _ADC_ChannelY = ADC1_GPIO39_CHANNEL; break;
        case GPIO_NUM_34: _ADC_ChannelY=ADC1_GPIO34_CHANNEL; break;
        case GPIO_NUM_35: _ADC_ChannelY=ADC1_GPIO35_CHANNEL; break;
        case GPIO_NUM_32: _ADC_ChannelY=ADC1_GPIO32_CHANNEL; break;
        case GPIO_NUM_33: _ADC_ChannelY=ADC1_GPIO33_CHANNEL; break;
        default:{Serial.println("FuckY"); break;}
    }
    //adc1_config_channel_atten(_ADC_ChannelX, ADC_ATTEN_DB_0);
    //adc1_config_channel_atten(_ADC_ChannelY, ADC_ATTEN_DB_0);
    _calculateVoltage = calculateVoltage;
    _ADC_Resolusion = resolution;
    maxAdcValue = (1 << _ADC_Resolusion) -1 ;
    abs_coordinate = (maxAdcValue-1)/2;
};


void AnalogJoystick::measure(adc1_channel_t ch, Position* pos) const {

    uint32_t adc_accumulator = 0;
    for (uint8_t i = 0; i < NUM_OF_SAMPLES; i++){
        adc_accumulator+= adc1_get_raw(ch);
    }
    Serial.print("ADC_ ACCUM IS "); Serial.print(adc_accumulator);
    adc_accumulator /= NUM_OF_SAMPLES;

    pos->val = map(adc_accumulator, 0, maxAdcValue, -abs_coordinate, abs_coordinate);
    Serial.printf("max ADC val is %d, abs is %d\n", maxAdcValue, abs_coordinate);
    Serial.print(" VAl is "); Serial.print(pos->val );
    if (_calculateVoltage) {
        pos->voltage = (float) (adc_accumulator * 3.286) / (maxAdcValue+1);
        Serial.print(" Voltage  IS "); Serial.print(pos->voltage, 4);
    }

}

bool AnalogJoystick::isCalculateVoltage() const {
    return _calculateVoltage;
}

void AnalogJoystick::setCalculateVoltage(bool calculateVoltage) {
    _calculateVoltage = calculateVoltage;
}

void AnalogJoystick::calibrate() {
    Position oldX = _posX;
    Position  oldY = _posY;
    uint32_t time = millis();
    measure(_ADC_ChannelX, &_posX);
    measure(_ADC_ChannelY, &_posY);
    time = millis() - time;
    Serial.printf("Measuring X and Y took %d milliseconds\n", time);




}
