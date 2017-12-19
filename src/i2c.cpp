#include "i2c.h"
#include <string>
#include <iostream>

using namespace std;

void i2c::init(string file)
{
    i2c_File = file;
}


unsigned char* i2c::read_register(unsigned char reg, int address)
{
    cout << "reading register called" << endl;
    unsigned char buffer[100];
    return buffer;
}

