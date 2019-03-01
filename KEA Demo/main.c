// 比完赛晚上秋名山见
// Last updated: 2-26-2019 By 朱雨齐

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
                    GetSpeed();
                    MYOledShow();
                }
            }
        }

        else if (ADV > 150 && AD1 > 500 && AD4 > 400) // 判环
        {
            if (isRing == 0) // 第一次
            {
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
                Soft_Delay_ms(500);
                while (ADV > 150 || AD1 > 500)
                {
                    AD1 = ADC_Read(ADC0_SE1);
                    ADV = ADC_Read(ADC0_SE2);
                    AD4 = ADC_Read(ADC0_SE9);
                    MYOledShow();
                    PIT_Init1(pit0, 5000); //单位us,0.1ms
                    PIT_SetCallback(PIT_Interrupt);
                    Enable_Interrupt(INT_PIT_CH0);
                    Disable_Interrupt(INT_PIT_CH0);
                    offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
                    steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                    SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
                    speed = kTopSpeed - 0.1 * turnconvert(fabs(offset));
                    SetMotor(kTopSpeed - 0.1 * turnconvert(fabs(offset))); //在offset<24时不减速
                }

                isRing++;
            }
            else if (isRing == 2)
            {
                GPIO_Set(I1, LOW);
                isRing = 0;
            }
        }
        else
        {
            Control();
        }
    }
}
