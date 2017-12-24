#include "PT1000.h"

PT1000::PT1000(ADC *_ADC,float _m, float _c)
{
    ADS1015 = _ADC;
    grad = _m;
    y_intercept = _c;
    //ctor
}

float PT1000::getTemperature()
{
    return temperature;
}

void PT1000::updateTemperature()
{
    ADS1015->updateVoltage();
    cout << "ADC voltage " << ADS1015->getVoltage();
    temperature = (ADS1015->getVoltage())*grad;
    temperature = temperature + y_intercept;
}

void PT1000::calibrate(float _grad, float _y_intercept)
{
    grad = _grad;
    y_intercept = _y_intercept;
}

