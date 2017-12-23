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
        unsigned char* read_register(int address, unsigned char reg, int length);
        int write_register(int addr1, unsigned char reg, int length1, unsigned char bytes[]);

    private:

        string i2c_File;
        char *filename;
        //unsigned char buffer[100];
        int file_i2c;
};

#endif // I2C_H
