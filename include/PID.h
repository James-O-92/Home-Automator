#ifndef PID_H
#define PID_H


class PID
{
    public:
        PID();
        void tune(float _Kp, float _Ki, float _Kd);
        float generateOutput(float buffer[], float setpoint, float timeStep);
        float scaleOutput(float output);
        void setScaler(float grad, float y_int);

    private:
        float Kp;
        float Ki;
        float Kd;
        float integral;
        float scale_grad;
        float scale_Y_Int;
        float scale_upperBound;
        float scale_lowerBound;

};

#endif // PID_H
