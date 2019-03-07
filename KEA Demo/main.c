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
    int lap = 0;           // 干簧管控制的 圈数计数器
    int isStartLine = 0;   // 起跑线检测标识
    UART_RX_IRQ_Enable(uart0);// 蓝牙中断
    
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
