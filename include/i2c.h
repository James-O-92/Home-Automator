#ifndef I2C_H
#define I2C_H
#include <string>

using namespace std;

class i2c
{
    public:
        i2c(string file)
        unsigned char* read_register(unsigned char reg, int address);

    private:

        string i2c_File;
};

#endif // I2C_H
