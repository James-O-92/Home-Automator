#ifndef DAC_H
#define DAC_H
#include <iostream>
#include "i2c.h"

using namespace std;

class DAC
{
    public:
        DAC(int addr);
        float getVoltage();
        void updateVoltage(i2c* i2c_bus, float volts);

    private:
        int address;
        float voltage;
};

#endif // DAC_H
