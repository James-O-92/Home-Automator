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
#include "PID.h"
#include <stdlib.h>     /* atexit */

using namespace std;

int main(int argc, char* argv[])
{
    unsigned char buffer[60];
    float u = 0;
    float buf[2];
    unsigned char temp[2];
    unsigned short TEMP = 0;
    float calibrated = 0;
    int addr = 0;
    unsigned char *arr;

    i2c* i2c_bus = new i2c;
    ADC* ADS1015 = new ADC(i2c_bus,0x49);
    DAC* MCP4725 = new DAC(i2c_bus,0x63);
    ADS1015->updateVoltage();
    MCP4725->updateVoltage(3.5);
    PT1000* pt1000 = new PT1000(ADS1015,42.9487,-19.3551);
    PID* pid = new PID();
    pid->tune(0.005,0.0005,0.0);
    pt1000->updateTemperature();
    cout << "Temperature " << pt1000->getTemperature() << endl;

	float sp1 = 0;
    float sp2 = 0;
    float setpoint = 0;
    int wait = 0;
    int flag = 0;

    if(argc == 4)
    {
        sp1 = atof(argv[1]);
        sp2 = atof(argv[2]);
        wait = atof(argv[3]);
    }
    else if(argc == 2)
    {
        setpoint = atof(argv[1]);
    }
    else
    {
        return -1;
    }

    buf[0] = pt1000->getTemperature();
    buf[1] = pt1000->getTemperature();

    while(1)

    {
        pt1000->updateTemperature();
        cout << "Voltage " << (ADS1015->getVoltage()) << endl;
        cout << "Temperature " << (pt1000->getTemperature()) << endl << endl;

        buf[0] = pt1000->getTemperature();

        u = pid->generateOutput(buf,setpoint,0.5);

        cout << "Output " << u << endl;

        if(u > 5.0)
        {
            MCP4725->updateVoltage(5.0);
        }else if (u < 0)
        {
            MCP4725->updateVoltage(0.0);
        }else
        {
            MCP4725->updateVoltage(u);
        }

        cout << "output " << MCP4725->getVoltage() << "V" << endl;

        buf[1] = buf[0];

        /*
        if(pt1000->getTemperature() <= sp1)
        {

            MCP4725->updateVoltage(5.0);
            flag = 1;

        }else if(pt1000->getTemperature() >= sp2)
        {

            MCP4725->updateVoltage(0.0);

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
        */
        this_thread::sleep_for (std::chrono::milliseconds(500));


    }
    i2c_bus->i2c_close();

}
