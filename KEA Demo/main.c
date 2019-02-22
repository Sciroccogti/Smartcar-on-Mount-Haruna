// 比完赛晚上秋名山见
// Last updated: 2-21-2019 By 张逸帆

#include "main.h"
void PIT_Interrupt(uint8 ch)
{
    GPIO_Turn(G2);
    GPIO_Turn(G3);
}

int main(void)
{
    MYInit();
    int speed = 0;
    int inRing = 0;
    int lap = 0;         // 干簧管控制的 圈数计数器
    int isStartLine = 0; // 起跑线检测标识

    while (1)
    {
        // 读取AD值并打印
        OLED_Clear(0x00);
        AD1 = ADC_Read(ADC0_SE1);
        AD1 = AD1 > 15 ? AD1 - 15 : (AD1 > 7 ? AD1 - 7 : AD1); // 对AD值的偏差做了调整！！
        AD2 = ADC_Read(ADC0_SE3);
        AD3 = ADC_Read(ADC0_SE2);
        AD4 = ADC_Read(ADC0_SE9);
        sprintf(spring_oled, "%3d%3d%3d%3d", AD1, AD2, AD3, AD4);
        OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
        sprintf(spring_oled, "offset:%.2f", offset);
        OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);
        sprintf(spring_oled, "I0:%d", Pin(I0));
        OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
        /*
        //编码器部分：请注意，编码器应选用脉冲-方向型，且须接在speed2
        count = FTM_Pulse_Get(ftm1);
        sprintf(spring_oled, "%d", count);
        OLED_Show_String(8, 16, 0, 48, 1, spring_oled, 0);
        FTM_Count_Clean(ftm1);
        */
        // 使用PD设置偏移量
        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
        const int straight_adjust_thres = 30, turn_thres = 45;

        for (isStartLine = 0; isStartLine < 3; isStartLine++) // 起跑线检测模块
        {
            if (Pin(I0) && Pin(H1) && Pin(H0)) // 软件消抖
            {
                if(isStartLine == 2) // 若连续三次键值为真
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
            SetMotor(-100);
            Soft_Delay_ms(200);
            SetMotor(0);
            OLED_Refresh_Gram();
            break;
        }

        if (AD1 + AD4 <= 20 && speed) // 出赛道自动停车，赛时需要移除
        {
            Soft_Delay_ms(500);
            speed = 0;
            SetMotor(0);
        }
        else if (fabs(offset) > straight_adjust_thres && fabs(offset) <= turn_thres) //直道调整
        {
            SetSteer((int)(kMidSteer - (offset > 0 ? 1 : -1) * (fabs(offset) - straight_adjust_thres) * 0.7)); //乘数为转弯系数
            //FTM_PWM_Duty(ftm2, ftm_ch1, kTopSpeed);//除数为减速系数
        }
        else if (fabs(offset) > turn_thres)
        {                                        //转弯的offset阈值
            SetSteer((int)(kMidSteer - offset * 2.6)); //乘数为转弯系数
            if (fabs(offset) > turn_thres + 20)
                SetMotor(kTopSpeed - fabs(offset) / 4.8); //除数为减速系数
            else
                SetMotor(kTopSpeed - fabs(offset) / 2.7); //除数为减速系数
        }
        else
        { //直行
            SetMotor(kTopSpeed);
            SetSteer(kMidSteer);
        }

        OLED_Refresh_Gram();
    }
}
