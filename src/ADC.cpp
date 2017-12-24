#include "ADC.h"

using namespace std;

ADC::ADC(i2c* _i2c_bus,int addr)
{
    i2c_bus = _i2c_bus;
    address = addr;
    //ctor
}

float ADC::getVoltage()
{
    return voltage;
}

void ADC::updateVoltage()
{
    unsigned char *arr;
    unsigned char buffer[60];

    //cout << "initializing i2c" << endl;
    i2c_bus->init("/dev/i2c-1");

    //cout << "writing to config" << endl;
    buffer[0] = 0b00000001;
    buffer[1] = 0b10000100;
	buffer[2] = 0b10000011;
    i2c_bus->write_register(address,0x01,3,buffer);

    buffer[0] = 0x00;
    //cout << "writing register addr pointer i2c" << endl;
    i2c_bus->write_register(address,0x01,1,buffer);

    arr = i2c_bus->read_register(address,0x01,2);

    i2c_bus->i2c_close();

    unsigned short prcnt = 0;
    prcnt = (unsigned short) *arr;
    prcnt = prcnt << 8;
    prcnt = prcnt + (unsigned short) *(arr + 1);

    float Prcnt = (float)prcnt;
    Prcnt = Prcnt/65535;

    voltage = Prcnt *5;

    cout << "ADC voltage: " << voltage << endl;

}
