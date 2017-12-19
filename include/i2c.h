#ifndef I2C_H
#define I2C_H
#include <string>


class i2c
{
    public:
        i2c();
        unsigned char* read_register(unsigned char reg, int address);

    private:

        std::string i2c_File;
};

#endif // I2C_H
