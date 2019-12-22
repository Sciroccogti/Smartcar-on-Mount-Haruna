// 比完赛晚上秋名山见 v3.0
// Last updated: 12-4-2019 By 张逸帆
// test: 关闭电机倒转

#ifndef DRIVER_H
#define DRIVER_H

#include "main.h"

extern const int kMidSteer, kTopSpeed;
extern int count;

// 舵机打角设定，0为打直，绝对值最大160，左负右正
void SetSteer(float dir)
{
    if (dir > 170)
    {
        dir = 170;
    }
    else if (dir < -170)
    {
        dir = -170;
    }
    FTM_PWM_Duty(ftm0, ftm_ch0, (int)(dir + kMidSteer));
}

// 电机电压设定，支持直接设置负数
void SetMotor(float floats)
{
    int s = (int)floats;
    if (s > 0)
    {
        FTM_PWM_Duty(ftm2, ftm_ch0, s < kTopSpeed ? s : kTopSpeed); // 设置了速度上限：kTopSpeed
    }
    else if (!s)
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, 0);
        FTM_PWM_Duty(ftm2, ftm_ch0, 0);
    }
    else
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, -s < kTopSpeed ? -s : kTopSpeed); // 设置了速度下限：-kTopSpeed
    }
}

void SetMotor_d(float s)
{
    const float apower = 1000;
    const float dpower = 5000;

    float fade = fabs(s - count) < 4 ? fabs(s - count) * 0.25 : 1.0;

    if (count > s && s > 0) //正向并且实际速度高于预期，减速
    {
        SetMotor(-0.2 * dpower * fade);
        //SetMotor(0);
    }
    else if (count <= s && s > 0) //正向并且实际速度低于预期，加速
    {
        SetMotor(apower * fade);
    }
    else if (count > 2 && s == 0) //s==0即停车，反转电机
    {
        {
            SetMotor(-dpower);
        }
        //Soft_Delay_ms(2);
    }
    else if (count <= 1 && s == 0) //停得差不多了，不反转电机
    {
        SetMotor(0);
    }
}

#endif