#ifndef PID_H
#define PID_H


class PID
{
    public:
        PID();
        void tune(float _Kp, float _Ki, float _Kd);
        float generateOutput(float buffer[], float setpoint, float timeStep);

    private:
        float Kp;
        float Ki;
        float Kd;
        float integral
};

#endif // PID_H
