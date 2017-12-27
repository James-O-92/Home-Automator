#include "DAC.h"
#include "i2c.h"

using namespace std;

DAC::DAC(i2c* _i2c_bus,int addr)
{
    i2c_bus = _i2c_bus;
    address = addr;
}

float DAC::getVoltage()
{
    return voltage;
}

void DAC::updateVoltage(float volts)
{
    unsigned char buffer[2];

    i2c_bus->init("/dev/i2c-1");

    volts = volts/5;
    
    if((volts <= 1) && (volts >= 0))
    {
        voltage = volts*5;
        volts = volts*4095;
        unsigned short bin = (unsigned short)volts;
        buffer[1] = (unsigned char)(bin & 0xff);
        bin = bin >> 8;
        buffer[0] = bin;
        
    }else if(volts < 0)
    {
        voltage = 0;
        buffer[0] = 0;
        buffer[1] = 0;
        
    }else if(volts > 5)
    {
        voltage = 5;
        buffer[0] = 0x0f;
        buffer[1] = 0xff;
    }

    i2c_bus->write_register(address,0x01,2,buffer);
    
    i2c_bus->i2c_close();

}
