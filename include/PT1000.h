#ifndef PT1000_H
#define PT1000_H
#include "i2c.h"
#include "ADC.h"

using namespace std;


class PT1000
{
    public:
        PT1000(ADC* _ADC, float _m, float _c);
        float getTemperature();
        void updateTemperature();
        void calibrate(float _grad, float _y_intercept);


    private:
        ADC* ADS1015;
        float grad;
        float y_intercept;
        float temperature;

};

#endif // PT1000_H
