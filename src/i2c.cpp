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
    int file_i2c;

    char *filename = (char*)i2c_File;
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		cout << "Failed to open the i2c bus" << endl;
		return buffer;
	}else
	{
	    cout << "opened i2c bus" << endl;
	}
	return buffer;
}

int write_register(unsigned char reg, int address, unsigned char *bytes)
{
    return 0;
}
