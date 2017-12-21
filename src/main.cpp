#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "i2c.h"

void i2c_test(i2c* i2c_bus)
{
    cout << "test" << endl;
    unsigned char reg = 0x01;
    int addr = 49;
    i2c_bus->read_register(reg, addr);
}

using namespace std;

int main(int argc, char* argv[])
{
    unsigned char temp[2];
    unsigned short TEMP = 0;
    int calibrated = 0;

    i2c* i2c_bus = new i2c;

    i2c_test(i2c_bus);

    while(1){

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

	length = 3;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		// ERROR HANDLING: i2c transaction failed
		cout << "Failed to write to the i2c bus.\n" << endl;
	}
	else
    {
        printf("wrote to config register: 0x%X 0x%X 0x%X\n", buffer[0], buffer[1], buffer[2]);
	}

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

	temp[0] = buffer[0];
	temp[1] = buffer[1];
    TEMP = 0;
	TEMP = (unsigned short) temp[0];
	TEMP = TEMP << 8;
	TEMP = (unsigned short) temp[1];



	printf("TEMP: %d degrees\n",TEMP);


	//temperature conversion


	if(TEMP <= 190)
    {
        int addr = 0x63;          //<<<<<The I2C address of the slave
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
        {
            cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
            //ERROR HANDLING; you can check errno to see what went wrong
            return -2;
        }

        //write FFF to drive max output
        buffer[0] = 0x0f;
        buffer[1] = 0xff;

        length = 2;			//<<< Number of bytes to write
        if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
        {
            // ERROR HANDLING: i2c transaction failed
            cout << "Failed to write to the i2c bus.\n" << endl;
        }
        else
        {
        cout << "DAC output 5V" << endl << endl;
        }
    }else if(TEMP >= 240)
    {
         int addr = 0x63;          //<<<<<The I2C address of the slave
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
        {
            cout << "Failed to acquire bus access and/or talk to slave.\n" << endl;
            //ERROR HANDLING; you can check errno to see what went wrong
            return -2;
        }

        //write FFF to drive max output
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
        cout << "DAC output 0V" << endl << endl;
        }
    }


    this_thread::sleep_for (std::chrono::seconds(5));


    }
}

