// 比完赛晚上秋名山见
// Last updated: 2-26-2019 By 朱雨齐

#include "main.h"
#include <math.h>

void PIT_Interrupt(uint8 ch)
{
    AD1 += 100;
}
int main(void)
{
    MYInit();
    int lap = 0;           // 干簧管控制的 圈数计数器
    int isStartLine = 0;   // 起跑线检测标识

    while (1)
    {
        Refresh();
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

        if (AD1 + AD4 <= 12) // 出赛道自动停车，赛时需要移除  15
        {
            Soft_Delay_ms(5);
            if (AD1 + AD4 <= 10)
            {
                Soft_Delay_ms(5);
                if (AD1 + AD4 <= 10)
                {
                    SetMotor_d(0);
                }
            }
        }
        /*
        else if (AD2 > 500 && AD3 > 500) // 判环
        {
            static int dir = 0;
            SetMotor_d(5);
            if (AD2 > AD3 + 300)
            {
                if (isRing == 0) // 判右环
                {
                    isRing = 1;
                    while (AD2 > AD3 && AD3 > 400)
                    {
                        AD1 = ADC_Read(ADC0_SE1);
                        AD2 = ADC_Read(ADC0_SE3);
                        AD3 = ADC_Read(ADC0_SE2);
                        AD4 = ADC_Read(ADC0_SE9);
                        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
                        steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                        SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
                        GetCount();
                        MYOledShow();
                    }
                }
                if (isRing == -2) // 出左环
                {
                    isRing = 0;
                    while (AD2 > 500 && AD3 > 500)
                    {
                        Control();
                        MYOledShow();
                    }
                }
            }

            else if (AD3 > AD2 + 300)
            {
                if (isRing == 2) // 出右环
                {
                    isRing = 0;
                    while (AD2 > 500 && AD3 > 500)
                    {
                        Control();
                        MYOledShow();
                    }
                }
                else if (isRing == 0) // 判左环
                {
                    isRing = -1;
                    while (AD2 > AD3 && AD3 > 400)
                    {
                        SetMotor_d(5);
                        AD1 = ADC_Read(ADC0_SE1);
                        AD2 = ADC_Read(ADC0_SE3);
                        AD3 = ADC_Read(ADC0_SE2);
                        AD4 = ADC_Read(ADC0_SE9);
                        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
                        offset += 50;
                        steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                        SetSteer(steer); //乘数为转弯系数
                        MYOledShow();
                    }
                }
            }

            else
            {
                if (isRing == 1) // 进右环
                {
                    isRing = 2;
                    while (AD1 > AD4 && (AD2 < 1000 || AD3 < 1000))
                    {
                        AD1 = ADC_Read(ADC0_SE1);
                        AD2 = ADC_Read(ADC0_SE3);
                        AD3 = ADC_Read(ADC0_SE2);
                        AD4 = ADC_Read(ADC0_SE9);
                        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
                        offset += 50;
                        steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                        SetSteer(steer); //乘数为转弯系数
                        GetCount();
                        MYOledShow();
                    }
                }
                else if (isRing == -1) // 进左环
                {
                    isRing = -2;
                    while (AD1 < AD4 && (AD2 < 1000 || AD3 < 1000))
                    {
                        AD1 = ADC_Read(ADC0_SE1);
                        AD2 = ADC_Read(ADC0_SE3);
                        AD3 = ADC_Read(ADC0_SE2);
                        AD4 = ADC_Read(ADC0_SE9);
                        offset = (float)100 * (AD3 - AD2) / (AD2 + AD3 + 10);
                        offset -= 50;
                        steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                        SetSteer(steer); //乘数为转弯系数
                        GetCount();
                        MYOledShow();
                    }
                }
            }
        }
*/
        else if (ADV > 150 && AD1 > 500 && AD4 > 500) // 判环
        {
            if (isRing == 0) // 第一次
            {
                GPIO_Turn(G1);
                while (ADV > 150 && AD1 > 500 && AD4 > 500)
                {
                    Refresh();
                    MYOledShow();
                    Control();
                    checkstop();
                }
                if (AD2 > AD3) // 判右环
                {
                    isRing = 1;
                }
                else // 判左环
                {
                    isRing = -1;
                }
                GPIO_Turn(G1);
            }
            else if (isRing == 1 || isRing == -1)
            {
                GPIO_Turn(G2);
                for(int i = 0;i < 60000;i++)
                {
                  Refresh();
                  Control();
                }

                  SetSteer(isRing * 110);
                
                for(int i = 0;i < 100000;i++)
                {
                  Refresh();
                  SetMotor_d(6);
                }
                while (ADV > 150 || AD2 > 800 || AD3 > 800)
                {
                    Refresh();
                    AD1 = AD2;
                    AD4 = AD3;
                    Control(1);
                    MYOledShow();
                    checkstop();
                }

                isRing *= 2;
                GPIO_Turn(G2);
            }
            else if (isRing == 2 || isRing == -2)
            {
                GPIO_Turn(G3);
                while (ADV > 150 && AD1 > 500 && AD4 > 500)
                {
                    Refresh();
                    Control();
                    MYOledShow();
                    checkstop();
                }
                isRing = 0;
            }
        }

        else
        {
            Control();
        }
        MYOledShow();
    }
}
