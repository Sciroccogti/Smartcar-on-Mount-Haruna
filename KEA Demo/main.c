// 比完赛晚上秋名山见
// Last updated: 2-22-2019 By 张逸帆

#include "main.h"
#include <math.h>

void PIT_Interrupt(uint8 ch)
{
    GPIO_Turn(G2);
    GPIO_Turn(G3);
}

double turnconvert(double x) //offset与舵机转向的转换函数
{
    const double a = 1.14828e-4, b = 6, c = 9.77256e-11; //b = 5.15858
    return exp(a * x * x + c * x + b) - exp(b);
}

void SetMotor_d(int s, int c)
{
    const int apower = 170;
    const int dpower = 500;
    int fade = abs(s - c) < 10 ? abs(s - c) * 0.1 : 1;
    if (c > s && s > 0) //正向并且实际速度高于预期，减速
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, 0);
        FTM_PWM_Duty(ftm2, ftm_ch0, dpower * fade);
    }
    else if (c <= s && s > 0) //正向并且实际速度低于预期，加速
    {
        FTM_PWM_Duty(ftm2, ftm_ch0, 0);
        FTM_PWM_Duty(ftm2, ftm_ch1, apower * fade);
    }
    else if (c > 10 && s == 0) //s==0即停车，反转电机
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, 0);
        FTM_PWM_Duty(ftm2, ftm_ch0, dpower * fade);
    }
    else if (c <= 10 && s == 0) //停得差不多了，不反转电机
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, 0);
        FTM_PWM_Duty(ftm2, ftm_ch0, 0);
    }
}

int main(void)
{
    MYInit();
    GPIO_Init(C5, GPI, 1); // SW1，控制起跑线检测模块
    GPIO_Init(H7, GPI, 1); // SW2，控制OLED显示函数
    int lap = 0;         // 干簧管控制的 圈数计数器
    int isStartLine = 0; // 起跑线检测标识

    while (1)
    {
        if (Pin(C5)) // 使用拨码器控制起跑线检测模块，SW1为真时启用
        {
            for (isStartLine = 0; isStartLine < 3; isStartLine++) // 起跑线检测模块
            {
                if (Pin(I0) && Pin(H1) && Pin(H0)) // 软件消抖
                {
                    if (isStartLine == 2) // 若连续三次键值为真
                    {
                        lap++;
                        isStartLine = 0;
                        break;
                    }
                    Soft_Delay_ms(5);
                }
                else
                {
                    break;
                }
            }
            if (lap > kTotalLap) // 若跑完要求圈数，则停车
            {
                speed = -100;
                SetMotor(-100);
                Soft_Delay_ms(200);
                speed = 0;
                SetMotor(0);
                break;
            }
        }

        if (AD1 + AD4 <= 15) // 出赛道自动停车，赛时需要移除
        {
            Soft_Delay_ms(5);
            if (AD1 + AD4 <= 10)
            {
                Soft_Delay_ms(5);
                if (AD1 + AD4 <= 10)
                {
                    SetMotor(0);
                    count = FTM_Pulse_Get(ftm1); //编码器数值读取
                    FTM_Count_Clean(ftm1);       //编码器数值清零
                    MYOledShow();
                }
            }
        }

        /*else if (ADV > 150 && AD1 > 500 && AD4 > 400) // 判环
        {
            if (isRing == 0) // 第一次
            {
                //GPIO_Turn(I1);
                //GPIO_Turn(I1);
                while (ADV > 150 && AD1 > 500 && AD4 > 400)
                {
                    Control();
                }
                isRing++;
            }
            else if (isRing == 1)
            {
                GPIO_Set(I1, HIGH);
                SetSteer(-100);
                //SetMotor()
                Soft_Delay_ms(500);
                while (ADV > 150 || AD1 > 500)
                {
                    Control();
                }

                isRing++;
            }
            else if (isRing == 2)
            {
                GPIO_Set(I1, LOW);
                isRing = 0;
            }
        }*/
        else
        {
            Control();
        }
    }
}
