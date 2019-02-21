// 比完赛晚上秋名山见 
// Last updated: 2-21-2019 By 张逸帆
// 命名规范参见https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#
// const常量请以 k 开头，大小写混合
// 函数请以大写开头，大小写混合

#include "common.h"

#define SetSteer(dir) FTM_PWM_Duty(ftm0, ftm_ch0, dir)

char spring_oled[20];
uint8 data_getstring[2];
uint16_t AD1 = 0, AD2 = 0, AD3 = 0, AD4 = 0;
uint16_t count;
float pre_offset = 0, offset = 0;
const int kTopSpeed = 136; //  速度上限
const int kBrakeSpeed=-50;
const float kMidSteer = 520.0;//530.0
const int kTotalLap = 1; //  圈数（资格赛）

void SetMotor(int s) // 支持直接设置负数
{
    if (s > 0)
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, s < kTopSpeed ? s : kTopSpeed); // 设置了速度上限：kTopSpeed
    }
    else if (!s)
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, 0);
        FTM_PWM_Duty(ftm2, ftm_ch0, 0);
    }
    else
    {
        FTM_PWM_Duty(ftm2, ftm_ch0, -s < kTopSpeed ? -s : kTopSpeed); // 设置了速度下限：-kTopSpeed
    }
}

void MYInit()
{
    while ((1280 * ex_clk_khz) != (256 * ics_clk_khz))
        ; //确保时钟配置无误

    FTM_PWM_Init(ftm0, ftm_ch0, A0, 300, kMidSteer); //舵机
    FTM_PWM_Init(ftm2, ftm_ch1, F1, 14000, 0); //电机
    FTM_PWM_Init(ftm2, ftm_ch0, F0, 14000, 0); //电机倒转

    //ADC
    ADC_Init(ADC0_SE1, ADC_12bit);  //A1,AD1
    ADC_Init(ADC0_SE3, ADC_12bit);  //A7,AD2
    ADC_Init(ADC0_SE2, ADC_12bit);  //A6,AD3
    ADC_Init(ADC0_SE9, ADC_12bit);  //C1,AD4
    ADC_Init(ADC0_SE10, ADC_12bit); //C2,AD5

    //编码器
    FTM_Pulse_Init(ftm1, FTM_PS_1, TCLK2);

    //OLED
    OLED_Init();
    OLED_Display_Config(1);

    //干簧管
    GPIO_Init(I0, GPI, LOW); //IO1
    GPIO_Init(H1, GPI, LOW); //IO2
    GPIO_Init(H0, GPI, LOW); //IO3

    //蜂鸣器
    GPIO_Init(I1, GPO, LOW);

    //LED
    GPIO_Init(G1, GPO, LOW);
}