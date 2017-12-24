#include "PT1000.h"

PT1000::PT1000(ADC *_ADC,float _m, float _c)
{
    ADC = _ADC;
    grad = _m;
    y_intercept = _c;
    //ctor
}

float getTemperature()
{
    return temperature;
}

void updateTemperature()
{
    ADC->updateVoltage();
    temperature = (ADC->getVoltage())*grad;
    temperature = temperature + y_intercept;
}

void calibrate(float _grad, float _y_intercept)
{
    grad = _grad;
    y_intercept = _y_intercept;
}

