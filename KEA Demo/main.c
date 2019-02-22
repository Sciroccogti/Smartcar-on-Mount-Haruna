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
    const double a = 1.14828e-4, b = 6, c = 9.77256e-11;//b = 5.15858
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
        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
        //servoturn((offset > 0 ? 1 : -1) * turnconvert(fabs(offset)));//乘数为转弯系数
        SetSteer(-(offset > 0 ? 1 : -1) * turnconvert(fabs(offset))); //乘数为转弯系数
        SetMotor(kTopSpeed - 0.1 * turnconvert(fabs(offset)));        //在offset<24时不减速
        sprintf(spring_oled, "%.2f", offset);
        OLED_Show_String(8, 16, 0, 20, 1, spring_oled, 0);
        OLED_Refresh_Gram();
    }
}
