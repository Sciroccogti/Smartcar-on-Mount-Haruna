#include "control.h"

const float kStraightSpeed = 12.5, kCornerSpeed = 7.5;
const int kOutMin = 10, kOutMax = 4000;

uint16_t AD1 = 0, AD2 = 0, AD3 = 0, AD4 = 0, ADV = 0; // electromagnet sensor cache
int distance = 0, mode = -1, count = 0; // mode：速度控制标志，-1为自动速度
float steer = 0, offset = 0, speed = 0, expected_steer = 0;

float target_spd()
{
    const float dec_offset = 32;
    const float dec_rate = 0.8;
    return (kStraightSpeed - kCornerSpeed) / (1 + exp(dec_rate * (fabs(offset) - dec_offset))) + kCornerSpeed;
}

// 速度控制环
float speed_loop(float target_speed)
{
    // "l" stands for "last"
    static float speed_error = 0, lspeed_error = 0, llspeed_error = 0;
    float Pacc = 90;
    float Pdec = 1500;
    float I = 0.1;
    float D = 0.1;
    if (count > kStraightSpeed)
    {
        target_speed = kStraightSpeed; //暂时人为限速
    }
    llspeed_error = lspeed_error;
    lspeed_error = speed_error;
    speed_error = target_speed - count;
    return (speed_error > 0 ? Pacc : Pdec) * speed_error; // + D * DSPEED + I * ISPEED;
}

// 转向控制环
float steer_loop(float P, float I, float D)
{
    // "l" stands for "last"
    const float convert = 52;
    float steerconv = 0;
    static float loffset = 0, lloffset = 0;
    lloffset = loffset;
    loffset = offset;
    offset = (float)100 * (AD4 - AD1) / (AD1 + AD4 + 10);
    steerconv = offset / fabs(offset) * convert * exp((fabs(offset) <= steerconv ? 0.21 : 0.4) * (fabs(offset) - convert));
    return P * steerconv + D * DSTEER + I * ISTEER;
}

// 通用指数控制，speedmode控制速度，-1为默认，-2为关闭
void Control()
{
    static int alarm_count = 0;
    float Poffset = 0, Ioffset = 0, Doffset = 0;
    if (FALSE && (AD1 < 50 || AD4 < 50) && AD1 + AD4 > kOutMin && AD1 + AD4 < kOutMax) // 丢线
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
    else if (abs((int)offset) > 55) // 弯道 TODO: 动态判定条件
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
        Poffset = 1;
        Ioffset = 0;
        Doffset = 3;
    }
    else // 直道
    {
        GPIO_Set(I1, LOW);
        Poffset = 1;
        Ioffset = 0.6;
        Doffset = 0;
    }

    // Poffset = 1;
    // Ioffset = 0.8 - offset /100;
    // Ioffset = Ioffset > 0.3 ? Ioffset : 0;
    // Doffset = 2;
    expected_steer = steer_loop(Poffset, Ioffset, Doffset);
    // speed -= offset / 20;
    // speed = speed > 5 ? speed : 5;
    SetSteer(expected_steer);
    speed = target_spd();
    if (mode)
        //SetMotor(speed_loop(speed));
        SetMotor_d(speed);
    else
    {
        speed = 0;
        SetMotor(0);
    }
}

// 异常自动停车，赛时需要移除
void ChecktoStop()
{
    // static int alarm_count = 0;
    if (AD1 + AD4 <= 10 || AD1 + AD4 >= 4000)
    {
        AD1 = ADC_Read(ADC0_SE1);
        AD4 = ADC_Read(ADC0_SE9);
        if (AD1 + AD4 <= 10 || AD1 + AD4 >= 4000)
        {
            AD1 = ADC_Read(ADC0_SE1);
            AD4 = ADC_Read(ADC0_SE9);
            if (AD1 + AD4 <= 10 || AD1 + AD4 >= 4000)
            {
                mode = 0;
            }
        }
    }
    else
    {
        GPIO_Set(I1, LOW); // 蜂鸣器
        // if (!mode)
        mode = -1;
    }
}