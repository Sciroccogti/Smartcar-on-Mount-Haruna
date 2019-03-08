// 比完赛晚上秋名山见
// Last updated: 2-26-2019 By 朱雨齐

#include "main.h"
#include <math.h>

void PIT_Interrupt(uint8 ch)
{
    //GPIO_Turn(G1);
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
        if (AD1 + AD4 >= 2000&&AD1+AD4<=4000)  // && (AD2 > 500 || AD3 > 500) && ADV > 400 && ADV < 800)
        {
            flag = 5;
            if (isRing == 0)
            {
                Pout(G1, 0);
                Pout(G2, 1);
                Pout(G3, 1);
                // Disable_Interrupt(INT_PIT_CH0);
                // SetSteer(isRing * 110);
                // Soft_Delay_ms(500);
                // Enable_Interrupt(INT_PIT_CH0);
                // flag = -2;
                // while (AD2 < 600 || AD3 < 600)
                // {
                //     MYOledShow();
                // }
                // flag = 4;
                isRing = 1;
                //while(AD1 + AD4 >= 2000)  // && (AD2 > 500 || AD3 > 500) && ADV > 400 && ADV < 800)
                //while(AD2-AD3>200 || AD3-AD2>200)
                {
                    //MYOledShow();
                  
                }
            }
            else if (isRing == 1) // 进环
            {
                Pout(G1, 1);
                Pout(G2, 0);
                Pout(G3, 1);
                
                if(Pin(H2))
                //if (AD2 + AD1 < AD3 + AD4) // 判右环
                {
                    isRing = 1;
                }
                else // 判左环
                {
                    isRing = -1;
                }
                Disable_Interrupt(INT_PIT_CH0);
                Soft_Delay_ms(550);
                for(int i = 0;i < 1900;i++)
                {
                  SetSteer(isRing * 150);
                  Refresh();
                  SetMotor_d(8);
                  Soft_Delay_ms(1);
                }
                Enable_Interrupt(INT_PIT_CH0);
                //flag = -2;
               // while (AD2 < 600 || AD3 < 600)
                //{
                  //  MYOledShow();
                //}
                flag = 5;
                isRing *= 2;
            }
            else
            {
                Pout(G1, 1);
                Pout(G2, 1);
                Pout(G3, 0);
                flag = 5;
                while (AD1 + AD4 <= 2000)  // && (AD2 > 500 || AD3 > 500) && ADV > 400 && ADV < 800)
                //while(AD2-AD3>200 || AD3-AD2>200)
                {
                    MYOledShow();
                }
                flag = 5;
                isRing = 0;
                Soft_Delay_ms(2000);
            }
        }
        else
        {
            Pout(G1, 1);
            Pout(G2, 1);
            Pout(G3, 1);
            flag = -1;
        }

        /*
        if (AD2 - AD3 > 500 || AD2 - AD3 < -500 && AD)
        {
            //flag = -2;
            Pout(G2, 0);
        }
        else
        {
            flag = -1;
            Pout(G2, 1);
        }*/
        MYOledShow();
    }

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
    }
}