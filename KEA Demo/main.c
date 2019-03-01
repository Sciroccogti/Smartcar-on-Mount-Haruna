// 比完赛晚上秋名山见
// Last updated: 2-26-2019 By 朱雨齐

#include "main.h"
#include <math.h>

void PIT_Interrupt(uint8 ch)
{
    GPIO_Turn(G2);
    GPIO_Turn(G3);
}

void SetMotor_d(int s)
{
    const float apower = 100;
    const float dpower = 500;
    GetCount();
    float fade = abs(s - count) < 10 ? abs(s - count) * 0.1 : 1.0;
    //Soft_Delay_ms(6);
    if (count > s && s > 0) //正向并且实际速度高于预期，减速
    {
        SetMotor(0);
    }
    else if (count <= s && s > 0) //正向并且实际速度低于预期，加速
    {
        SetMotor(170);
    }
    else if (count > 10 && s == 0) //s==0即停车，反转电机
    {
        SetMotor(-500);
    }
    else if (count <= 10 && s == 0) //停得差不多了，不反转电机
    {
        SetMotor(0);
    }
}

void Control()
{
    static int i = 0;
    static float diff = 0, prev_offset = 0;
    const float c = 0.001;
    AD1 = ADC_Read(ADC0_SE1);
    ADV = ADC_Read(ADC0_SE2);
    AD4 = ADC_Read(ADC0_SE9);
    offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);

    //当offset导数小于某个正值的时候，转向幅度变小

    steer = -(offset > 0 ? 1.0 : -1.0) * (turnconvert(fabs(offset)) + diff * c)*0.8; //乘数为转弯系数
    SetSteer(steer);
    speed = StraightSpeed / (1 + 0.05 * turnconvert(fabs(offset)));
    SetMotor_d(1000);

    MYOledShow();

    if (i % 200 == 0)
    {
        diff = offset - prev_offset;
        prev_offset = offset;
        i = 0;
    }
    else
        i++;
}

 int main(void)
{
    MYInit();
    GPIO_Init(C5, GPI, 1); // SW1，控制起跑线检测模块
    GPIO_Init(H7, GPI, 1); // SW2，控制OLED显示函数
    int lap = 0;           // 干簧管控制的 圈数计数器
    int isStartLine = 0;   // 起跑线检测标识
    while (1)
    { /*
        // 读取AD值并打印
        if (Pin(H7))
            OLED_Clear(0x00);
*/
      int s=200;
      FTM_PWM_Duty(ftm2, ftm_ch1, s < StraightSpeed ? s : StraightSpeed);
      while(1);
      return 0;
        AD1 = ADC_Read(ADC0_SE1);
        AD2 = ADC_Read(ADC0_SE3);
        ADV = ADC_Read(ADC0_SE2);
        AD4 = ADC_Read(ADC0_SE9);
        int sumAD = AD1 + AD2 + AD3 + AD4;

        /*
        //编码器部分：请注意，编码器应选用脉冲-方向型，且须接在speed2
        count = FTM_Pulse_Get(ftm1);
        FTM_Count_Clean(ftm1);
        */
        // 使用PD设置偏移量
        offset = (float)95 * (AD1 - AD4) / (AD1 + AD4 + 10);
        //const int straight_adjust_thres = 30, turn_thres = 45;
        //GPIO_Set(G1, Pin(C5));

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

        /*if (AD1 + AD4 <= 15) // 出赛道自动停车，赛时需要移除
        {
            Soft_Delay_ms(5);
            if (AD1 + AD4 <= 10)
            {
                Soft_Delay_ms(5);
                if (AD1 + AD4 <= 10)
                {
                    SetMotor(0);
                    GetCount();
                    MYOledShow();
                }
            }
        }*/

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
        //else
        {
            Control();
        }
    }
}
