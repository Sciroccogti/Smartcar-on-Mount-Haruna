// 比完赛晚上秋名山见 v3.1
// Last updated: 12-18-2019 By 张逸帆
// feat: 弯道直道采用相同的P

#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"
#include <math.h>

extern uint16_t AD1, AD2, AD3, AD4, ADV;
extern int mode, count;
extern float steer, expected_steer, speed;
extern const float kStraightSpeed, kCornerSpeed;
extern const int kOutMin, kOutMax;

// 速度控制环
float speed_loop(float target_speed)
{
    // "l" stands for "last"
    static float speed_error = 0, lspeed_error = 0, llspeed_error = 0;
    float P = 15;
    float I = 0.5;
    float D = 3;
    if (count > kStraightSpeed)
    {
        target_speed = kStraightSpeed; //暂时人为限速
    }
    llspeed_error = lspeed_error;
    lspeed_error = speed_error;
    speed_error = target_speed - count;
    int DSPEED = speed_error - lspeed_error;
    int ISPEED = speed_error + lspeed_error + llspeed_error;
    return target_speed + P * speed_error + D * DSPEED + I * ISPEED;
}

// 转向控制环
float steer_loop(float P, float I, float D)
{
    // "l" stands for "last"
    static float loffset = 0, lloffset = 0;
    lloffset = loffset;
    loffset = offset;
    offset = (float)100 * (AD4 - AD1) / (AD1 + AD4 + 10);
    int DSTEER = offset - loffset;
    int ISTEER = offset + loffset + lloffset;
    return P * offset + D * DSTEER + I * ISTEER;
}

// 通用指数控制，speedmode控制速度，-1为默认，-2为关闭
void Control()
{
    static int alarm_count = 0;
    float Poffset = 0, Ioffset = 0, Doffset = 0;
    if ((AD1 < 50 || AD4 < 50) && AD1 + AD4 > kOutMin && AD1 + AD4 < kOutMax) // 丢线
    {
        alarm_count++;
        if (alarm_count > 2)
        {
            GPIO_Set(I1, HIGH); // 蜂鸣器
            alarm_count = 0;
        }
        else
        {
            GPIO_Set(I1, LOW);
        }

        if (AD1 > AD4) // 向右偏出
        {
            mode = 3;
        }
        else // 向左偏出
        {
            mode = -3;
        }
        speed = kCornerSpeed;
    }
    else if (abs((int)offset) > 60) // 弯道 TODO: 动态判定条件
    {
        GPIO_Set(I1, LOW);
        Poffset = 2;
        Ioffset = 0;
        Doffset = 0.05;
        speed = kCornerSpeed;
    }
    else // 直道
    {
        GPIO_Set(I1, LOW);
        Poffset = 2;
        Ioffset = 0;
        Doffset = 0;
        speed = kStraightSpeed;
    }

    expected_steer = steer_loop(Poffset, Ioffset, Doffset);

    if (!mode) // 停车
    {
        speed = 0;
        SetMotor(0);
    }
    else if (abs(mode) < 2) // 自动速度
        SetMotor(speed_loop(speed));
    else // 丢线（mode绝对值大于1）
    {
        expected_steer = 160 * mode / abs(mode);
        mode -= mode / abs(mode);     // 绝对值减小1
        mode = mode == 1 ? -1 : mode; // 若为1则改为-1
    }

    // speed -= offset / 20;
    // speed = speed > 5 ? speed : 5;
    SetSteer(expected_steer);
}

// 异常自动停车，赛时需要移除
void ChecktoStop()
{
    // static int alarm_count = 0;
    if (AD1 + AD4 <= kOutMin || AD1 + AD4 >= kOutMax)
    {
        AD1 = ADC_Read(ADC0_SE1);
        AD4 = ADC_Read(ADC0_SE9);
        if (AD1 + AD4 <= kOutMin || AD1 + AD4 >= kOutMax)
        {
            AD1 = ADC_Read(ADC0_SE1);
            AD4 = ADC_Read(ADC0_SE9);
            if (AD1 + AD4 <= kOutMin || AD1 + AD4 >= kOutMax)
            {
                mode = 0;
            }
        }
    }
    else
    {
        GPIO_Set(I1, LOW); // 蜂鸣器
        mode = -1;
    }
}

#endif