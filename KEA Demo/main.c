// 比完赛晚上秋名山见
// Last updated: 2-26-2019 By 朱雨齐

#include "main.h"
#include <math.h>

void PIT_Interrupt(uint8 ch)
{
    //GPIO_Turn(G1);
    Refresh();
    //ChecktoStop();
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
            
            if (isRing == 0) // 进环
            {
                Pout(G1, 0);
                Pout(G2, 1);
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
                
                //Soft_Delay_ms(500);
                distance = 0;
                
                while(distance <= 3300);
                
                Pout(G1, 1);
                Pout(G2, 0);
                Pout(G3, 1);
                Disable_Interrupt(INT_PIT_CH0);
                distance = 0;
                for(int i = 0;distance < 10000;i++)
                {
                  if(Pin(H2))
                    SetSteer(isRing * 130);
                  else
                    SetSteer(isRing * 140);
                  Refresh();
                  SetMotor_d(7);
                  Soft_Delay_us(700);
                  //MYOledShow();
                }
                Enable_Interrupt(INT_PIT_CH0);
                //flag = -2;
               // while (AD2 < 600 || AD3 < 600)
                //{
                  //  MYOledShow();
                //}
                isRing *= 2;
            }
            else
            {
                Pout(G1, 0);
                Pout(G2, 1);
                Pout(G3, 0);
                flag = -1;
                while (ADV <= 800)  // && (AD2 > 500 || AD3 > 500) && ADV > 400 && ADV < 800)
                //while(AD2-AD3>200 || AD3-AD2>200)
                {
                    //MYOledShow();
                }
                Pout(G1, 1);
                Pout(G2, 0);
                Pout(G3, 0);
                flag = -1;
                isRing = 0;
                distance = 0;
                
                while(distance <= 8000);
                
                distance = 0;
                Pout(G1, 1);
                Pout(G2, 1);
                Pout(G3, 1);
            }
        }
        else
        {
            Pout(G1, 1);
            Pout(G2, 1);
            Pout(G3, 1);
            flag = -1;
            //isRing = 0;
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