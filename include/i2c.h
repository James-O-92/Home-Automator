#ifndef I2C_H
#define I2C_H
#include <string>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

using namespace std;

class i2c
{
    public:
        void init(string file);
        unsigned char* read_register(unsigned char reg, int address);
        int write_register(unsigned char reg, int address, unsigned char *bytes);

    private:

        string i2c_File;
};

#endif // I2C_H
