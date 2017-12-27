#include "PID.h"
#include <iostream>

using namespace std;

PID::PID()
{
    integral = 0;
}

void PID::tune(float _Kp, float _Ki, float _Kd)
{
    Kp = _Kp;
    Ki = _Ki;
    Kd = _Kd;
    integral = 0;
}

float PID::generateOutput(float buffer[], float setpoint, float timeStep)
{
    float error[2];
    error[0] = setpoint - buffer[0];
    error[1] = setpoint - buffer[1];
    float output = 0;
    output = (error[0])*Kp;
    output = output + Kd*((error[0] - error[1])/timeStep);

    integral = integral + Ki*((error[1] + error[0])/2)*timeStep;

    cout << "Integral " << integral << endl;

    output = output + (integral);

    return output;
}

float PID::scaleOutput(float output)
{
    if((output >= 10) && (output <= -10))
    {
        return ((scale_grad*output) + scale_Y_Int);
    }else if(output > 10)
    {
        return 5;
    }else if(output < -10)
    {
        return 0.9;
    }
}

void PID::setScaler(float grad, float y_int)
{
    scale_grad = grad;
    scale_Y_Int = y_int;
}




