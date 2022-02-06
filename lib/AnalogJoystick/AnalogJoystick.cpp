#include "AnalogJoystick.h"
void AnalogJoystick::init (uint8_t x_pin, uint8_t y_pin, uint8_t resolution, uint16_t deadzoneDeviation){
    if (x_pin != 36 && x_pin != 39 && x_pin != 34 && x_pin != 35 && x_pin != 32 && x_pin != 33){
#ifdef DEBUG
        Serial.println("X pin is not ADC1");
#endif
        return;
    }
    if (y_pin != 36 && y_pin != 39 && y_pin != 34 && y_pin != 35 && y_pin != 32 && y_pin != 33){
#ifdef DEBUG
        Serial.println("Y pin is not ADC1");
#endif
        return;
    }
    if (x_pin == y_pin) {
#ifdef DEBUG
        Serial.println("X === Y");
#endif
        return;}
    _pinX = x_pin;
    _pinY = y_pin;
    pinMode(_pinX, OUTPUT);
    pinMode(_pinY, OUTPUT);
    adc_bits_width_t t;
    switch (resolution) {
        case 12: t = ADC_WIDTH_BIT_12; break;
        case 11: t = ADC_WIDTH_BIT_11; break;
        case 10: t=ADC_WIDTH_BIT_10; break;
        case 9: t=ADC_WIDTH_BIT_9; break;
        default: {t=ADC_WIDTH_MAX;}
    }
    adc1_config_width(t);
    switch (_pinX) {
        case GPIO_NUM_36: _ADC_ChannelX = ADC1_GPIO36_CHANNEL; break;
        case GPIO_NUM_39: _ADC_ChannelX = ADC1_GPIO39_CHANNEL; break;
        case GPIO_NUM_34: _ADC_ChannelX=ADC1_GPIO34_CHANNEL; break;
        case GPIO_NUM_35: _ADC_ChannelX=ADC1_GPIO35_CHANNEL; break;
        case GPIO_NUM_32: _ADC_ChannelX=ADC1_GPIO32_CHANNEL; break;
        case GPIO_NUM_33: _ADC_ChannelX=ADC1_GPIO33_CHANNEL; break;
        default:{ break;}
    }
    switch (_pinY) {
        case GPIO_NUM_36: _ADC_ChannelY = ADC1_GPIO36_CHANNEL; break;
        case GPIO_NUM_39: _ADC_ChannelY = ADC1_GPIO39_CHANNEL; break;
        case GPIO_NUM_34: _ADC_ChannelY=ADC1_GPIO34_CHANNEL; break;
        case GPIO_NUM_35: _ADC_ChannelY=ADC1_GPIO35_CHANNEL; break;
        case GPIO_NUM_32: _ADC_ChannelY=ADC1_GPIO32_CHANNEL; break;
        case GPIO_NUM_33: _ADC_ChannelY=ADC1_GPIO33_CHANNEL; break;
        default:{break;}
    }
    _ADC_Resolusion = resolution;
    _maxAdcValue = (1 << _ADC_Resolusion) - 1 ;
    _abs_coordinateMax = (_maxAdcValue - 1) / 2;
    Serial.printf("INIT _abs_coordinateMax %d", _abs_coordinateMax);
    _deadzoneDeviation = deadzoneDeviation;
};

void AnalogJoystick::calibrate(uint16_t deadzoneDeviation) {
    calculateDeltaXY();
    _deadzoneDeviation = deadzoneDeviation;
    setupAbsMaxCoordinates();
}

int16_t AnalogJoystick::ADCMeasure(adc1_channel_t ch) const {
    uint32_t adc_accumulator = 0;
    for (uint8_t i = 0; i < NUM_OF_ADC_SAMPLES; i++){
        adc_accumulator+= adc1_get_raw(ch);
    }
    adc_accumulator /= NUM_OF_ADC_SAMPLES;
    int16_t tmpVal= map(adc_accumulator, 0, _maxAdcValue, ((_maxAdcValue) >> 1) - 1, -((_maxAdcValue) >> 1) + 1 );
    return tmpVal;
}

int16_t AnalogJoystick::getRawX1() const {
    int16_t tmpVal = ADCMeasure(_ADC_ChannelX) - _deltaX;
    if (abs(tmpVal) <= _deadzoneDeviation) {
#ifdef DEBUG
        Serial.printf(" DEADZONE BY X, val is %d \t", tmpVal);
#endif
        return 0;
    }
    if (abs(tmpVal) >= _abs_coordinateMax + _deadzoneDeviation) {
#ifdef DEBUG
        Serial.printf(" OVERZONE by X, val is %d\t ", tmpVal);
#endif
        return 0;
    }
    return  tmpVal;
}

int16_t AnalogJoystick::getRawY1() const {
    int16_t tmpVal = ADCMeasure(_ADC_ChannelY) - _deltaY;
    if (abs(tmpVal) <= _deadzoneDeviation) {
#ifdef DEBUG
        Serial.printf(" DEADZONE BY Y, val is %d <= %d \t", tmpVal, _deadzoneDeviation);
#endif
        return 0;
    }
    if (abs(tmpVal) > _abs_coordinateMax + _deadzoneDeviation) {
#ifdef DEBUG
        Serial.printf(" OVERZONE by Y, val is %d > %d\t ", tmpVal, _abs_coordinateMax + _deadzoneDeviation);
#endif
        return 0;
    }
    return tmpVal;
}

void AnalogJoystick::printState() {
    JOYSTICK_POS pos;
    int16_t x = getRawX1();
    int16_t y = getRawY1();
    Serial.printf("X is %d Y is %d\n", x, y);
    if (0==x && 0== y) { pos = IDLE_DEADZONE; Serial.println("IDLE"); return;}
    if (0==x){
        if (y > 0){ pos = UP; Serial.println("UP");}
        else {pos = DOWN; Serial.println("DOWN");}
        return;
    }
    if (0==y){
        if (x > 0){ pos = RIGHT; Serial.println("RIGHT");}
        else {pos = LEFT; Serial.println("LEFT");}
        return;
    }
    if (x > 0){
        if (y > 0){pos = UP_RIGHT; Serial.println("UP_RIGHT");}
        else {pos = DOWN_RIGHT; Serial.println("DOWN_RIGHT");}
        return;
    }
    if (x < 0){
        if (y > 0){pos = UP_LEFT; Serial.println("UP_LEFT");}
        else {pos = DOWN_LEFT; Serial.println("DOWN_LEFT");}
        return;
    }
}

void AnalogJoystick::calculateDeltaXY() {
    int32_t accumulator;
    accumulator = 0;
    for (uint8_t i = 0; i<NUM_OF_DELTA_SAMPLES; i++){
        accumulator+= ADCMeasure(_ADC_ChannelX);
    }
    _deltaX = (int16_t)(accumulator / NUM_OF_DELTA_SAMPLES);
#ifdef DEBUG
    Serial.printf("_deltaX is  %d acc %d\n", _deltaX, accumulator);
#endif
    accumulator = 0;
    for (uint8_t i = 0; i<NUM_OF_DELTA_SAMPLES; i++){
        accumulator+= ADCMeasure(_ADC_ChannelY);
    }
    _deltaY = (int16_t)(accumulator / NUM_OF_DELTA_SAMPLES);
#ifdef DEBUG
    Serial.printf("_deltaY is  %d ac %d\n", _deltaY, accumulator);
#endif
}

void AnalogJoystick::setupAbsMaxCoordinates() {
    uint32_t time = millis();
    int16_t localX =  ADCMeasure(_ADC_ChannelX);
    int16_t localY =  ADCMeasure(_ADC_ChannelY);
    time = millis() - time;
    Serial.printf("Measuring X and Y took %d milliseconds\n", time);
    Serial.printf("X is %d Y is %d\n", localX, localX);
    Serial.println ("Set max LEFT THEN BOTOOM and print to serial \n");
    while (Serial.available()) Serial.read();
    while (!Serial.available()) {delay(1);}
    localX = ADCMeasure(_ADC_ChannelX);
    Serial.printf("LEFT X is %d\n", localX);
    while (Serial.available()) Serial.read();
    while (!Serial.available()) {delay(1);}
    localY = ADCMeasure(_ADC_ChannelY);
    Serial.printf("BOTTOM Y is %d\n", localY);
    _abs_coordinateMax = (localX >= localY) ? localY : localX; // get min
}


