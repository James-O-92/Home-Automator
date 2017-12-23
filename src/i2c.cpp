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

int i2c::write_register(int addr1, unsigned char reg, int length, unsigned char bytes[])
{
    /*
    unsigned char buf[60];
    buf[0] = 0b00000001;
    buf[1] = 0b10000100;
	buf[2] = 0b10000011;

    filename = (char*)i2c_File.c_str();
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		cout << "Failed to open the i2c bus" << endl;
	}else
	{
	    cout << "opened i2c bus" << endl;
	}

    cout << "write register called" << endl;

    if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
		//ERROR HANDLING; you can check errno to see what went wrong
		return -1;
	}

    if (write(file_i2c, buf, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		// ERROR HANDLING: i2c transaction failed
		cout << "Failed to write to the i2c bus.\n" << endl;
	}
	else
    {
        //printf("wrote to register: 0x%X 0x%X 0x%X\n", buffer[0], buffer[1], buffer[2]);
	}
	*/

	int file_i2c;
	//int length;
	unsigned char buffer[60] = {0};

	float sp1 = 0;
    float sp2 = 0;
    int wait = 0;
    int flag = 0;

	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		cout << "Failed to open the i2c bus" << endl;
		return -1;
	}

	int addr = 0x49;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
		//ERROR HANDLING; you can check errno to see what went wrong
		return -2;
	}


    //write to config register
    buffer[0] = 0b00000001;
    buffer[1] = 0b10000100;
	buffer[2] = 0b10000011;

	//length = 3;			//<<< Number of bytes to write
	if (write(file_i2c, bytes, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		// ERROR HANDLING: i2c transaction failed
		cout << "Failed to write to the i2c bus.\n" << endl;
	}
	else
    {
        printf("wrote to config register: 0x%X 0x%X 0x%X\n", buffer[0], buffer[1], buffer[2]);
	}
    /*
    //----- write to address pointer register -----
    buffer[0] = 0x00;

	length = 1;			//<<< Number of bytes to read
	if (write(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		cout << "Failed to write to the i2c bus.\n" << endl;
	}
	else
	{
		printf("wrote to addr pointer register: 0x%X\n", buffer[0]);
		//cout << "Data read: " << hex(buffer[0]) << endl;
	}

    //read conversion register
	buffer[0] = 0x00;
    buffer[1] = 0x00;

	length = 2;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		cout << "Failed to read from the i2c bus.\n" << endl;
	}
	else
	{
		printf("ADC conversion register: 0x%X 0x%X\n", buffer[0], buffer[1]);
		//cout << "Data read: " << hex(buffer[0]) << endl;
	}
    */
    return 0;
}
