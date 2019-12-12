#include "mbed.h"
#include "Omni.hpp"

float g_coefMatrix[5][4][3] = {
{},
{},
{},
{
    {-2.f/3, 0, 1.f},
    {1.f/3, -1.f/1.73205f, 1.f},
    {1.f/3, 1.f/1.73205f, 1.f}},
{
    {-2.0, -2.0, 0.0},
    {-2.0, 1.0, 0.0},
    {0.7, -1.0, 0.0},
    {0.7, 1.0, 0.0}}
};

Omni::Omni(size_t _whee_num){
    wheel_num = _whee_num;
    PI = acosf(-1.0);
}


void Omni::calculate(float velocityVector[], float _out[], float currentAngle, bool radian)
{
    if(!radian)
        currentAngle = currentAngle*PI/180.f;
    //フィールド座標系での速度ベクトルをロボット座標系に回転行列を用いて変換
    float velocityVectorOnRobot[3];
    velocityVectorOnRobot[0] = velocityVector[0]*cos(-currentAngle)-velocityVector[1]*sin(-currentAngle);
    velocityVectorOnRobot[1] = velocityVector[0]*sin(-currentAngle)+velocityVector[1]*cos(-currentAngle);
    velocityVectorOnRobot[2] = velocityVector[2];

    for(size_t motorNum=0; motorNum<wheel_num; motorNum++) _out[motorNum] = 0.f;
    for(size_t motorNum=0; motorNum<wheel_num; motorNum++){
        for(size_t element=0; element<3; element++){
            _out[motorNum] += g_coefMatrix[wheel_num][motorNum][element] * velocityVectorOnRobot[element];
        }
    }
}

void Omni::calculate(double velocityVector[], double _out[], double currentAngle, bool radian)
{
    if(!radian)
        currentAngle = currentAngle*PI/180.f;
    //フィールド座標系での速度ベクトルをロボット座標系に回転行列を用いて変換
    float velocityVectorOnRobot[3];
    velocityVectorOnRobot[0] = velocityVector[0]*cos(-currentAngle)-velocityVector[1]*sin(-currentAngle);
    velocityVectorOnRobot[1] = velocityVector[0]*sin(-currentAngle)+velocityVector[1]*cos(-currentAngle);
    velocityVectorOnRobot[2] = velocityVector[2];

    for(size_t motorNum=0; motorNum<wheel_num; motorNum++) _out[motorNum] = 0.f;
    for(size_t motorNum=0; motorNum<wheel_num; motorNum++){
        for(size_t element=0; element<3; element++){
            _out[motorNum] += g_coefMatrix[wheel_num][motorNum][element] * velocityVectorOnRobot[element];
        }
    }
}
