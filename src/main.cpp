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
    //variables
    float u = 0;
    float buf[2];
    float setpoint = 0;

    //i2c BUS	
    i2c* i2c_bus = new i2c;
	
    //I/O interfaces
    ADC* ADS1015 = new ADC(i2c_bus,0x49);
    DAC* MCP4725 = new DAC(i2c_bus,0x63);
    ADS1015->updateVoltage();
    MCP4725->updateVoltage(0.0);
	
    //sensor
    PT1000* pt1000 = new PT1000(ADS1015,42.9487,-19.3551);

    //Controller
    PID* pid = new PID();
    pid->tune(0.05,0.005,0.0);
    pid->setScaler(0.205,2.95);
    pt1000->updateTemperature();

    if(argc == 2)
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
        cout << "Input Voltage " << (ADS1015->getVoltage()) << "V" << endl;
        cout << "Temperature " << (pt1000->getTemperature()) << endl;

        buf[0] = pt1000->getTemperature();

        u = pid->generateOutput(buf,setpoint,0.5);
	u = pid->scaleOutput(u);
	    
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

        cout << "output " << MCP4725->getVoltage() << "V" << endl << endl;

        buf[1] = buf[0];

        this_thread::sleep_for (std::chrono::milliseconds(500));

    }
    i2c_bus->i2c_close();

}
