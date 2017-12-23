#include "i2c.h"
#include <string>
#include <iostream>

using namespace std;

void i2c::init(string file)
{
    i2c_File = file;

}


unsigned char* i2c::read_register(int addr,unsigned char reg,int length)
{
    cout << "read register called" << endl;

    if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
		//ERROR HANDLING; you can check errno to see what went wrong
		return buffer;
	}

    if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		cout << "Failed to read from the i2c bus.\n" << endl;
	}
	else
	{
		//printf("ADC conversion register: 0x%X 0x%X\n", buffer[0], buffer[1]);
		//cout << "Data read: " << hex(buffer[0]) << endl;
	}

	return buffer;
}

int i2c::write_register(int addr, unsigned char reg, int length, unsigned char bytes[])
{

	int file_i2c;

	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		cout << "Failed to open the i2c bus" << endl;
		return -1;
	}

	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
		//ERROR HANDLING; you can check errno to see what went wrong
		return -2;
	}


    length =3;
	if (write(file_i2c, bytes, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		// ERROR HANDLING: i2c transaction failed
		cout << "Failed to write to the i2c bus.\n" << endl;
	}
	else
    {
        printf("wrote to config register: 0x%X 0x%X 0x%X\n", buffer[0], buffer[1], buffer[2]);
	}

    return 0;
}
