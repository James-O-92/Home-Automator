#include "SSR.h"
#include <iostream>

SSR::SSR(DAC* _DAC, float _dT, float _frequency, float _minOutput)
{

  MCP4725 = _DAC;
  dT= _dT;
  frequency = _frequency;
  T = (1/frequency);
  minOutput = _minOutput;
  minDuty =  minOutput/5;

}

float SSR::getOutput()
{

  return output;

}

void SSR::updateOutput(float dutyCycle)
{

  if(dutyCycle <= minDuty)
  {

    duty = dutyCycle/minDuty;
    if(timeStep >= T)
    {
        timeStep= 0;
        timeStep = timeStep + dT;
    }
    else if((timeStep/T) >= duty)
    {

      MCP4725->updateVoltage(0);
      timeStep = timeStep + dT;

    }
    else
    {
      MCP4725->updateVoltage(minOutput);
      timeStep = timeStep + dT;
    }
  }
  else
  {

    output = dutyCycle*5;
    MCP4725->updateVoltage(output);

  }

}
