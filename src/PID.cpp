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
    float output = 0;
    output = (setpoint - buffer[0])*Kp;
    output = output + Kd*((buffer[0] - buffer[1])/timeStep);

    integral = integral + ((buffer[1] + buffer[0])/2)*timeStep;

    cout << "Integral " << integral << endl;

    output = output + (Kp*integral);

    return output;
}
