// 比完赛晚上秋名山见 v3.0
// Last updated: 12-18-2019 By 张逸帆
// feat: 接收到's'则停车
// 命名规范参见https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#
// const常量请以 k 开头，大小写混合
// 函数请以大写开头，大小写混合

#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "driver.h"
#include "communicate.h"
#include "control.h"

// 调参常改常量
const int kTopSpeed = 200; //  速度上限
const int kMidSteer = 520; // 舵机物理中值
const float kStraightSpeed = 12, kCornerSpeed = 6.2;
// const int kTotalLap = 1;       //  圈数（资格赛）

uint16_t AD1 = 0, AD2 = 0, AD3 = 0, AD4 = 0, ADV = 0; // electromagnet sensor cache
int speed_mode = -1, count = 0;         // 速度控制标志，-1为自动速度
float steer = 0, offset = 0, speed = 0, expected_steer = 0;

const FTMn encoder_port = ftm1; // 编码器接口

void PIT_Interrupt(uint8 ch); // 中断函数

// 刷新数据
void Refresh()
{
    count = FTM_Pulse_Get(encoder_port); //编码器数值读取
    if (Pin(H6))                         // 电机倒转
        count = -count;
    FTM_Count_Clean(ftm1); //编码器数值清零

    AD1 = ADC_Read(ADC0_SE1);
    AD2 = ADC_Read(ADC0_SE3);
    AD3 = ADC_Read(ADC0_SE2);
    AD4 = ADC_Read(ADC0_SE9);
    ADV = ADC_Read(ADC0_SE10);

    if (GetBluetooth() == 's')
    {
        speed_mode = 0;
    }
}

void Init()
{
    while ((1280 * ex_clk_khz) != (256 * ics_clk_khz))
        ; //确保时钟配置无误

    FTM_PWM_Init(ftm0, ftm_ch0, A0, 300, kMidSteer); //舵机
    FTM_PWM_Init(ftm2, ftm_ch1, F1, 14000, 0);       //电机
    FTM_PWM_Init(ftm2, ftm_ch0, F0, 14000, 0);       //电机倒转

    //ADC
    ADC_Init(ADC0_SE1, ADC_12bit);  //A1,AD1
    ADC_Init(ADC0_SE3, ADC_12bit);  //A7,AD2
    ADC_Init(ADC0_SE2, ADC_12bit);  //A6,AD3
    ADC_Init(ADC0_SE9, ADC_12bit);  //C1,AD4
    ADC_Init(ADC0_SE10, ADC_12bit); //C2,AD5

    //编码器
    FTM_Pulse_Init(ftm1, FTM_PS_1, TCLK2);
    GPIO_Init(H6, GPI, HIGH);

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
    GPIO_Init(G1, GPO, HIGH); // B
    GPIO_Init(G2, GPO, HIGH); // G
    GPIO_Init(G3, GPO, HIGH); // R

    //SW
    GPIO_Init(C5, GPI, 1); // SW1，控制起跑线检测模块
    GPIO_Init(H7, GPI, 1); // SW2，控制OLED显示函数
    GPIO_Init(H5, GPI, 1); // SW3，控制低速
    GPIO_Init(H2, GPI, 1); // SW4，控制全速

    // 蓝牙
    UART_Init(uart2, 115200, RXTX_B0B1);
    //// UART_SetCallback(UART_Interrupt);
    //// NVIC_SetPriority(UART2_IRQn, 0x02); //如果我们不对优先级进行配置的话，则默认相应中断源的向量号越低其优先级越高
    //// UART_RX_IRQ_Disable(uart2);

    //PIT定时器
    PIT_Init1(pit0, 700); //单位us,0.1ms
    PIT_SetCallback(PIT_Interrupt);
    Disable_Interrupt(INT_PIT_CH0);
    Enable_Interrupt(INT_PIT_CH0);
}

#endif