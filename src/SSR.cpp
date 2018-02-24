#include "SSR.h"
#include <iostream>

void SSR::SSR(DAC* _DAC, float _dT, float _frequency, float _minOutput)
{

  MCP4725 = _DAC;
  dT= _dT;
  frequency = _frequency;
  T = (1/frequency);
  minOutput = _minOutput;

}

float SSR::getOutput()
{

  return output;

}

void SSR::updateOutput(float dutyCycle)
{

  if((dutyCycle*5) <= minOutput)
  {
    if((timeStep/T) > dutyCycle)
    {

      MCP4725->updateVoltage(0);
      timeStep = timeStep + dT;

    }else
    {

      MCP4725->updateVoltage(minOutput);
      timeStep = timeStep + dT;

    }else if(timeStep >= T)
    {
        timeStep= 0;
    }

  }else
  {

    output = dutyCycle*5;
    MCP4725->updateVoltage(output);

  }
}
