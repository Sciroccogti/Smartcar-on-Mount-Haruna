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

void Control()
{
    AD1 = ADC_Read(ADC0_SE1);
    ADV = ADC_Read(ADC0_SE2);
    AD4 = ADC_Read(ADC0_SE9);
    if (Pin(H7))
        OLED_Clear(0x00);
    steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
    SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
    speed = kTopSpeed - 0.1 * turnconvert(fabs(offset));
    SetMotor(kTopSpeed - 0.1 * turnconvert(fabs(offset))); //在offset<24时不减速
    if (Pin(H7))
    {
        sprintf(spring_oled, "L:%5d R:%5d", AD1, AD4);
        OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
        sprintf(spring_oled, "V:%5d L-R:%3d", ADV, AD1 - AD4);
        OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
        sprintf(spring_oled, "Sp:%3d St:%3d R:%d", speed, steer, isRing);
        OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);
        OLED_Refresh_Gram();
    }
}

int main(void)
{
    MYInit();
    GPIO_Init(C5, GPI, 1); // SW1，控制起跑线检测模块
    GPIO_Init(H7, GPI, 1); // SW2，控制OLED显示函数
    int speed = 0;
    int steer = 0;
    int isRing = 0;      // 1：第一次垂直电感到达阈值，2：第二次，3：第三次
    int lap = 0;         // 干簧管控制的 圈数计数器
    int isStartLine = 0; // 起跑线检测标识

    while (1)
    {
        // 读取AD值并打印
        if (Pin(H7))
            OLED_Clear(0x00);

        AD1 = ADC_Read(ADC0_SE1);
        //AD1 = AD1 > 15 ? AD1 - 15 : (AD1 > 7 ? AD1 - 7 : AD1); // 对AD值的偏差做了调整！！
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
        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
        const int straight_adjust_thres = 30, turn_thres = 45;
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
                OLED_Refresh_Gram();
                break;
            }
        }

        if (AD1 + AD4 <= 20) // 出赛道自动停车，赛时需要移除
        {
            Soft_Delay_ms(5);
            if (AD1 + AD4 <= 20)
            {
                Soft_Delay_ms(5);
                if (AD1 + AD4 <= 10)
                {
                    SetMotor(0);
                }
            }
        }

        else if (ADV > 150 && AD1 > 500 && AD4 > 400) // 判环
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
                SetSteer(-80);
                Soft_Delay_ms(1000);
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
        }
        else
        {
            steer = -(offset > 0 ? 1 : -1) * turnconvert(fabs(offset));
            SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
            speed = kTopSpeed - 0.2 * turnconvert(fabs(offset));
            SetMotor(kTopSpeed - 0.2 * turnconvert(fabs(offset))); //在offset<24时不减速
        }

        /*
        else if (fabs(offset) > straight_adjust_thres && fabs(offset) <= turn_thres) //直道调整
        {
            SetSteer((int)( - (offset > 0 ? 1 : -1) * (fabs(offset) - straight_adjust_thres) * 0.7)); //乘数为转弯系数
            //FTM_PWM_Duty(ftm2, ftm_ch1, kTopSpeed);//除数为减速系数
        }
        else if (fabs(offset) > turn_thres)
        {                                              //转弯的offset阈值
            SetSteer((int)( - offset * 2.6)); //乘数为转弯系数
            if (fabs(offset) > turn_thres + 20)
                SetMotor(kTopSpeed - fabs(offset) / 4.8); //除数为减速系数
            else
                SetMotor(kTopSpeed - fabs(offset) / 2.7); //除数为减速系数
        }
        else
        { //直行
            SetMotor(kTopSpeed);
            SetSteer(0);
        }*/
        if (Pin(H7))
        {
            sprintf(spring_oled, "L:%5d R:%5d", AD1, AD4);
            OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
            sprintf(spring_oled, "V:%5d L-R:%3d", ADV, AD1 - AD4);
            OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
            sprintf(spring_oled, "Sp:%3d St:%3d R:%d", speed, steer, isRing);
            OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);
            OLED_Refresh_Gram();
        }
    }
}
