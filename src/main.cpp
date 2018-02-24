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
#include "SSR.h"
#include "PID.h"
#include <stdlib.h>     /* atexit */
#include <thread>
#include <queue>
#include <mutex>

using namespace std;

mutex I_mtx;
mutex O_mtx;
float input;
float output;

int inputThread()
{

  i2c* i2c_bus = new i2c;
  ADC* ADS1015 = new ADC(i2c_bus,0x49);

  float buf[20];
  float avg_temp;

  //sensor
  PT1000* pt1000 = new PT1000(ADS1015,42.9487,-19.3551);

  while(true)
  {
    pt1000->updateTemperature();
    buf[0] = pt1000->getTemperature();
    avg_temp = 0;
    for(int i = 0; i < 20; i++)
    {
      avg_temp = avg_temp + buf[i];
    }
    avg_temp = avg_temp/20;

    I_mtx.lock();
    input = avg_temp;
    I_mtx.unlock();

    for(int i = 1; i < 20; i++)
    {
      buf[i] = buf[i-1];
    }
    //cout << "inputThread: " << pt1000->getTemperature() << " °C" << endl;
    this_thread::sleep_for (std::chrono::milliseconds(2));
  }
}

int outputThread()
{

  i2c* i2c_bus = new i2c;
  DAC* MCP4725 = new DAC(i2c_bus,0x63);

  //actuator
  SSR* crydom = new SSR(MCP4725,0.05,1,1.2);

  while(true)
  {
    O_mtx.lock();

    if(output > 1)
    {
      crydom->updateOutput(1);
    }else if(output < 0)
    {
      crydom->updateOutput(0);
    }else
    {
      crydom->updateOutput(output);
    }
    O_mtx.unlock();
    //cout << "outputThread: " << crydom->getOutput() << "% " << MCP4725->getVoltage() << "V" << endl;
    this_thread::sleep_for (std::chrono::milliseconds(5));
  }
}

int controlLoopThread(int argc, char* argv[])
{
    //variables
    float u = 0;
    float buf[2];
    float setpoint = 0;

    //Controller
    PID* pid = new PID();
    pid->tune(3.0,0.05,0.0);
    pid->setScaler(0.025,.5);

    if(argc == 2)
    {
        setpoint = atof(argv[1]);
    }
    else
    {
        return -1;
    }

    I_mtx.lock();
    buf[0] = input;
    I_mtx.unlock();

    this_thread::sleep_for (std::chrono::milliseconds(30));

    I_mtx.lock();
    buf[1] = input;
    I_mtx.unlock();


    while(1)

    {

      I_mtx.lock();
      buf[0] = input;
      I_mtx.unlock();

      this_thread::sleep_for (std::chrono::milliseconds(2));

      I_mtx.lock();
      buf[1] = input;
      I_mtx.unlock();

      cout << "Temperature " << buf[0] << endl;

      O_mtx.lock();

      u = (pid->scaleOutput(pid->generateOutput(buf,setpoint,0.032))/5);

      if((u < -0.1) || (u > 1.1))
      {
        pid->setSaturated(true);
      }else
      {
        pid->setSaturated(false);
      }

      output = u;
      cout << "output " << u << endl << endl;

      O_mtx.unlock();

      buf[1] = buf[0];

      //cout << "cntrl" << endl;
      this_thread::sleep_for (std::chrono::milliseconds(30));

    }
}

void serverLoopThread()
{
	while(1)
	{
		this_thread::sleep_for (std::chrono::milliseconds(200));
		//cout << "Server thread" << endl;
	}
}

int main(int argc, char* argv[])
{
    cout << "starting control thread..." << endl;
    thread cntrl(controlLoopThread, argc, argv);
    cout << "starting server thread..." << endl;
    thread server(serverLoopThread);
    cout << "starting input thread..." << endl;
    thread input(inputThread);
    cout << "starting output thread..." << endl;
    thread output(outputThread);

    cntrl.join();
    server.join();
    input.join();
    output.join();

}
