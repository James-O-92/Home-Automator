#ifndef ADC_H
#define ADC_H
#include "i2c.h"


class ADC
{
    public:
        ADC();
        float getVoltage();
        void updateVoltage(i2c* i2c_bus);

    private:
        int address;
        float voltage
};

#endif // ADC_H
