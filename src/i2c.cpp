#include "i2c.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

void i2c::init(string file)
{
    i2c_File = file;

    char *filename = (char*)i2c_File.c_str(); //"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		cout << "Failed to open the i2c bus" << endl;

	}


}

void i2c_close()
{
    close(file_i2c);
}

string i2c::read_register(int addr,unsigned char reg,int length)
{
    string output;

    cout << "read register called" << endl;

    if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
		//ERROR HANDLING; you can check errno to see what went wrong

	}

    if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		cout << "Failed to read from the i2c bus.\n" << endl;
	}
    if(buffer[0] != '\0')
    {
        ostringstream os;
        os << buffer[0];
        output = os.str(); // str is what you want.
    }

	return output;
}

int i2c::write_register(int addr, unsigned char reg, int length, unsigned char bytes[])
{

	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
		//ERROR HANDLING; you can check errno to see what went wrong
		return -2;
	}

	if (write(file_i2c, bytes, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		// ERROR HANDLING: i2c transaction failed
		cout << "Failed to write to the i2c bus.\n" << endl;
	}


    return 0;
}
