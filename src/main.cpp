#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;

int main(int argc, char* argv[])
{
    int temp = 0;

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
		printf("ADC conversion register: 0x%X 0x%X\n\n\n", buffer[0], buffer[1]);
		//cout << "Data read: " << hex(buffer[0]) << endl;
	}

	temp = buffer[0];
	if(temp <= 0x6C)
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
        cout << "DAC output 5V" << endl;
        }
    }else if(temp >= 0x6D)
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
        cout << "DAC output 0V" << endl;
        }
    }


    this_thread::sleep_for (std::chrono::seconds(5));


    }
}

