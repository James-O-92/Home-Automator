#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[])
{

	int file_i2c;
	int length;
	unsigned char buffer[60] = {0};


	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		cout << "Failed to open the i2c bus" << endl;
		return -1;
	}


	//----- INITIALISE -----

	int addr = 0x63;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
		//ERROR HANDLING; you can check errno to see what went wrong
		return -2;
	}

	//----- READ BYTES -----
	length = 3;		//<<< Number of bytes to read
	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;

	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		cout << "Failed to read from the i2c bus.\n" << endl;
	}
	else
	{
		printf("Data read: 0x%X 0x%X Ox%X\n", buffer[0], buffer[1],buffer[2]);
		//cout << "Data read: " << hex(buffer[0]) << endl;
	}

	//Write 000 to output 5V
    buffer[0] = 0x00;
	buffer[1] = 0x00;

	length = 2;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		// ERROR HANDLING: i2c transaction failed
		cout << "Failed to write to the i2c bus.\n" << endl;
	}
	else
    {
        cout << "DAC output 0V" << endl;
	}

}
