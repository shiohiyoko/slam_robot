#pragma once
#include <cmath>

class Omni
{
    public:
        Omni(size_t _wheel_num);
        void calculate(float velocityVector[], float _out[], float currentAngle=0, bool radian=true); //マシンのワールド座標系の角度を使用する
        void calculate(double velocityVector[], double _out[], double currentAngle=0, bool radian=true); //マシンのワールド座標系の角度を使用する
    private:
        float PI;
        size_t wheel_num;
};