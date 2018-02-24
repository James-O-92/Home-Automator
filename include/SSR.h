#ifndef SSR_H
#define SSR_H
#include "i2c.h"
#include "DAC.h"

using namespace std;

class SSR
{

  public:
      SSR(DAC* _DAC, float _dT, float _frequency, float _minOutput);
      float getOutput();
      void updateOutput(float dutyCycle);


    private:
      float duty;
      float minDuty;
      DAC* MCP4725;
      float output;
      float dT;
      float timeStep;
      float frequency;
      float T;
      float minOutput;

};

#endif // SSR_H
