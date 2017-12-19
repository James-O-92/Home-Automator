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

    char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		cout << "Failed to open the i2c bus" << endl;
		return -1;
	}else
	{
	    cout << "opened i2c bus" << endl;
	}
}

int write_register(unsigned char reg, int address, unsigned char *bytes)
{
    return 0;
}
