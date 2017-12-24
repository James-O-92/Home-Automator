#ifndef DAC_H
#define DAC_H
#include <iostream>
#include "i2c.h"

using namespace std;

class DAC
{
    public:
        DAC(i2c* i2c_bus, int addr);
        float getVoltage();
        void updateVoltage(float volts);

    private:
        i2c* i2c_bus;
        int address;
        float voltage;
};

#endif // DAC_H
