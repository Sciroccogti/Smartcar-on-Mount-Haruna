// 比完赛晚上秋名山见
// Last updated: 2-26-2019 By 朱雨齐

#include "main.h"
#include <math.h>

void PIT_Interrupt(uint8 ch)
{
    GPIO_Turn(G1);
    Refresh();
    ChecktoStop();
    Control();
}

int main(void)
{
    MYInit();
    int lap = 0;         // 干簧管控制的 圈数计数器
    int isStartLine = 0; // 起跑线检测标识
    //UART_RX_IRQ_Enable(uart0);// 蓝牙中断

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
                while (1)
                {
                    flag = 0;
                }
            }
        }

        if (ADV > 150 && AD1 > 500 && AD4 > 400) // 判环
        {
            if (isRing == 0) // 第一次
            {
                while (ADV > 150 && AD1 > 500 && AD4 > 400)
                {
                    Refresh();
                    MYOledShow();
                    Control();
                    ChecktoStop();
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
                for (int i = 0; i < 60000; i++)
                    ;

                SetSteer(isRing * 110);

                flag = 6;
                for (int i = 0; i < 100000; i++)
                {
                    Refresh();
                }
                flag = -1;

                while (ADV > 150 || AD2 > 800 || AD3 > 800)
                {
                    Refresh();
                    AD1 = AD2;
                    AD4 = AD3;
                    Control();
                    MYOledShow();
                    ChecktoStop();
                }

                isRing *= 2;
                GPIO_Turn(G2);
            }
            else if (isRing == 2 || isRing == -2)
            {
                GPIO_Turn(G3);
                while (ADV > 150 && AD1 > 500 && AD4 > 500)
                    ;
                isRing = 0;
            }
        }
        MYOledShow();
    }
}
