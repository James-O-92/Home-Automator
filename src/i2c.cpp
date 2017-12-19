#include "i2c.h"

using namespace std;

i2c::i2c(string file)
{
    i2c_File = file;
}


unsigned char* i2c::read_register(unsigned char reg, int address)
{

    unsigned char buffer[100];
    return buffer;
}

