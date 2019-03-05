// 比完赛晚上秋名山见
// Last updated: 2-26-2019 By 朱雨齐

#include "main.h"
#include <math.h>

void PIT_Interrupt(uint8 ch)
{
    AD1 += 100;
}

void select_speed()
{
  int pu = 0, pd = 0, u = 0, d = 0;
  while(!Pin(F4))   //按下中键表示确定
  {
      
      u = Pin(H4), d = Pin(F5);
      if(u||d)
        Pout(I1, HIGH);
      else
        Pout(I1, LOW);
      if(u && !pu)               //上
      {
        StraightSpeed++;
        Pout(I1, HIGH);
      }
      else if(d && !pd)          //下
      {
        StraightSpeed--;
        Pout(I1, HIGH);
      }
      else
        Pout(I1, LOW);
      OLED_Clear(0x00);
      OLED_Show_String(8, 16, 0, 0, 1, "Select Speed:", 0);
      sprintf(spring_oled, "%1f", StraightSpeed);
      OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
      OLED_Refresh_Gram();
      pu = u, pd = d;
  }
  
  
}

int main(void)
{
    MYInit();
    GPIO_Init(C5, GPI, 1); // SW1，控制起跑线检测模块
    GPIO_Init(H7, GPI, 1); // SW2，控制OLED显示函数
    int lap = 0;           // 干簧管控制的 圈数计数器
    int isStartLine = 0;   // 起跑线检测标识
    select_speed();
    while (1)
    {
        AD1 = ADC_Read(ADC0_SE1);
        AD2 = ADC_Read(ADC0_SE3);
        AD3 = ADC_Read(ADC0_SE2);
        AD4 = ADC_Read(ADC0_SE9);
        //GetCount();
        MYOledShow();
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

        else if (AD2 > 1000 && AD3 > 1000) // 判环
        {
            if (AD2 > AD3 + 100)
            {
                if (isRing == 0) // 进右环
                {
                    isRing = 1;
                    while (AD2 > AD3 && AD3 > 900)
                    {
                        SetMotor_d(5);
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
                    while (AD2 > 900 && AD3 > 900)
                    {
                        Control();
                        MYOledShow();
                    }
                }
            }

            else if (AD3 > AD2 + 100)
            {
                if (isRing == 2) // 出右环
                {
                    isRing = 0;
                    while (AD2 > 900 && AD3 > 900)
                    {
                        Control();
                        MYOledShow();
                    }
                }
                else if (isRing == 0) // 进左环
                {
                    isRing = -1;
                    while (AD2 > AD3 && AD3 > 900)
                    {
                        SetMotor_d(5);
                        AD1 = ADC_Read(ADC0_SE1);
                        AD2 = ADC_Read(ADC0_SE3);
                        AD3 = ADC_Read(ADC0_SE2);
                        AD4 = ADC_Read(ADC0_SE9);
                        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
                        steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                        SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
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
                        offset = (float)100 * (AD3 - AD2) / (AD2 + AD3 + 10);
                        steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                        SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
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
                        steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
                        SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
                        GetCount();
                        MYOledShow();
                    }
                }
            }
        }

        else
        {
            Control();
        }
    }
}
