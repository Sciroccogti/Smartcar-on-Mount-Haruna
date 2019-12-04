// 比完赛晚上秋名山见 v2.0
// Last updated: 12-4-2019 By 张逸帆
// feat: PID

#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"
#include <math.h>

#define DSPEED (speed_error - lspeed_error)
#define ISPEED (speed_error + lspeed_error + llspeed_error)
#define DSTEER (offset - loffset)
#define ISTEER (offset + loffset + lloffset)

extern uint16_t AD1, AD2, AD3, AD4, ADV;
extern int speed_mode, count;
extern float speed, steer;
extern const float kStraightSpeed, kCornerSpeed;

// 速度控制环
float speed_loop(float expected_speed)
{
    // "l" stands for "last"
    static float speed_error = 0, lspeed_error = 0, llspeed_error = 0;
    float P = 15;
    float I = 3.0;
    float D = 0.5;
    if (speed > 120)
    {
        expected_speed = 110; //暂时人为限速
    }
    llspeed_error = lspeed_error;
    lspeed_error = speed_error;
    speed_error = expected_speed - speed;
    return expected_speed + P * speed_error + D * DSPEED + I * ISPEED;
}

// 转向控制环
float steer_loop(float P, float D, float I)
{
    // "l" stands for "last"
    static float loffset = 0, lloffset = 0;
    lloffset = loffset;
    loffset = offset;
    offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
    return P * offset + D * DSTEER + I * ISTEER;
}

// 通用指数控制，speedmode控制速度，-1为默认，-2为关闭
void Control()
{
    float Poffset = 0, Ioffset = 0, Doffset = 0, expected_steer = 0, expected_speed = 0;
    if (FALSE) // TODO:丢线
    {
    }
    else if (offset > 10) // 弯道 TODO: 动态判定条件
    {
        Poffset = 0.175;
        Ioffset = 0;
        Doffset = 1.95;
        expected_speed = kStraightSpeed;
    }
    else // 直道
    {
        Poffset = 0.08;
        Ioffset = 0;
        Doffset = 1.4;
        expected_speed = kCornerSpeed;
    }
    expected_steer = steer_loop(Poffset, Ioffset, Doffset);
    expected_speed -= offset;
    expected_speed = expected_speed > 5 ? expected_speed : 5;
    SetSteer(expected_steer);
    SetMotor(speed_loop(expected_speed));
}

// 异常自动停车，赛时需要移除
void ChecktoStop()
{
    if (AD1 + AD4 <= 20 || AD1 + AD4 >= 4000)
    {
        GPIO_Set(I1, HIGH); // 蜂鸣器
        AD1 = ADC_Read(ADC0_SE1);
        AD4 = ADC_Read(ADC0_SE9);
        if (AD1 + AD4 <= 20)
        {
            AD1 = ADC_Read(ADC0_SE1);
            AD4 = ADC_Read(ADC0_SE9);
            if (AD1 + AD4 <= 20)
            {
                speed_mode = 0;
            }
        }
    }
    else
    {
        GPIO_Set(I1, LOW); // 蜂鸣器
        if (!speed_mode)
            speed_mode = -1;
    }
}

#endif