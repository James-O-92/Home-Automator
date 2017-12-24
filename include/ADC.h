#ifndef ADC_H
#define ADC_H
#include "i2c.h"
#include <iostream>

using namespace std;

class ADC
{
    public:
        ADC(i2c* i2c_bus, int addr);
        float getVoltage();
        void updateVoltage(i2c* i2c_bus);

    private:
        i2c* i2c_bus;
        int address;
        float voltage;
};

#endif // ADC_H
