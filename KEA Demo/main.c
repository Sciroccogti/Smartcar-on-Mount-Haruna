// 比完赛晚上秋名山见
// Last updated: 2-21-2019 By 张逸帆
/****************************************
2018年12月16日 朱雨齐
调整缩进格式
把PWM控制全写进了一个函数，main函数可读性可修改性增强
然后写了一个新的转向舵机函数模型
y=e^(a*x^2+cx+b)-e^b
*****************************************/
#include "main.h"
#include "math.h"
/*
void servoturn(int offset)//转向
{
	SetSteer(kMidSteer - offset);
}
*/
double turnconvert(double x) //offset与舵机转向的转换函数
{
    const double a = 1.14828e-4, b = 6, c = 9.77256e-11; //b = 5.15858
    return exp(a * x * x + c * x + b) - exp(b);
}

void PIT_Interrupt(uint8 ch)
{
    GPIO_Turn(G2);
    GPIO_Turn(G3);
}

int main(void)
{
    MYInit();
    while (1)
    {
        OLED_Clear(0x00);
        AD1 = ADC_Read(ADC0_SE1);
        AD4 = ADC_Read(ADC0_SE9);
        int sumAD = AD1 + AD2 + AD3 + AD4;
        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
        //servoturn((offset > 0 ? 1 : -1) * turnconvert(fabs(offset)));//乘数为转弯系数
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
        else
        {
            SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
            SetMotor(kTopSpeed - 0.2 * turnconvert(fabs(offset)));        //在offset<24时不减速
        }

        sprintf(spring_oled, "1:%4d 2:%4d", AD1, AD2);
        OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
        sprintf(spring_oled, "3:%4d 4:%4d", AD3, AD4);
        OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
        sprintf(spring_oled, "sum:%4d L-R:%3d", sumAD, AD1 + AD2 - AD3 - AD4);
        OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);
        sprintf(spring_oled, "offset:%.2f", offset);
        OLED_Show_String(8, 16, 0, 48, 1, spring_oled, 0);
        OLED_Refresh_Gram();
    }
}
