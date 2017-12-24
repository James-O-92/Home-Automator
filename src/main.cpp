#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string.h>
#include "i2c.h"
#include "ADC.h"
#include "DAC.h"
#include "PT1000.h"
#include <stdlib.h>     /* atexit */

using namespace std;

int main(int argc, char* argv[])
{
    unsigned char buffer[60];
    unsigned char temp[2];
    unsigned short TEMP = 0;
    float calibrated = 0;
    int addr = 0;
    unsigned char *arr;

    i2c* i2c_bus = new i2c;
    ADC* ADS1015 = new ADC(i2c_bus,0x49);
    DAC* MCP4725 = new DAC(i2c_bus,0x63);
    ADS1015->updateVoltage();
    //cout << ADS1015->getVoltage() << endl;
    MCP4725->updateVoltage(3.5);

    PT1000* pt1000 = new PT1000(ADS1015,0.033433,-17.58827);

    pt1000->updateTemperature();
    cout << "Temperature " << pt1000->getTemperature() << endl;

	float sp1 = 0;
    float sp2 = 0;
    int wait = 0;
    int flag = 0;

    if(argc == 4)
    {
        sp1 = atof(argv[1]);
        sp2 = atof(argv[2]);
        wait = atof(argv[3]);
    }else
    {
        return -1;
    }

    cout << "initializing i2c" << endl;

    while(1)

    {
        i2c_bus->init("/dev/i2c-1");
        addr = 0x49;

        cout << "Writing to config" << endl;
        buffer[0] = 0b00000001;
        buffer[1] = 0b10000100;
        buffer[2] = 0b10000011;
        i2c_bus->write_register(addr,0x01,3,buffer);

        cout << "writing register addr pointer i2c" << endl;
        buffer[0] = 0x00;
        i2c_bus->write_register(addr,0x01,1,buffer);

        arr = i2c_bus->read_register(addr,0x01,2);



        cout << endl << "-- i2c OUTPUT BUFFER --" << endl;

        for(int i = 0; i < 2; i++)
        {
            if(*arr != '\0')
            {
                printf("0x%X ", *(arr + i));
                temp[i] = *(arr + i);
            }
        }
        cout << endl << "--------- END ---------" << endl;

        //temperature conversion
        TEMP = 0;
        TEMP = (unsigned short) temp[0];
        TEMP = TEMP << 8;
        TEMP = TEMP + (unsigned short) temp[1];

        calibrated = (float) TEMP;
        calibrated = calibrated*0.0033433;
        calibrated = calibrated - 17.58827;

        printf("Temp: %f degrees\n",calibrated);

        addr = 0x63;          //<<<<<The I2C address of the slave

        if(calibrated <= sp1)
        {

            //write FFF to drive max output
            buffer[0] = 0x0f;
            buffer[1] = 0xff;

            i2c_bus->write_register(addr,0x01,2,buffer);
            flag = 1;

            cout << "DAC 5V" << endl;

        }else if(calibrated >= sp2)
        {

            //write FFF to drive max output
            buffer[0] = 0x00;
            buffer[1] = 0x00;

            i2c_bus->write_register(addr,0x01,2,buffer);
            cout << "DAC 0V" << endl;

            if(flag == 1)
            {
                flag = 0;
                cout << "cool down..." << endl;
                this_thread::sleep_for (std::chrono::seconds(wait));
            }
        } else
        {
            cout << endl;
        }

        i2c_bus->i2c_close();
        this_thread::sleep_for (std::chrono::milliseconds(500));


    }
    i2c_bus->i2c_close();

}

