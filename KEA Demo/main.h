// 比完赛晚上秋名山见 v1.6
// Last updated: 2-26-2019 By 张逸帆
// What's new: 增加自定义显示，编码器读值，count变为int型
// 命名规范参见https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#
// const常量请以 k 开头，大小写混合
// 函数请以大写开头，大小写混合

#include "common.h"

const int kTopSpeed = 150; //  速度上限
const float kMidSteer = 520.0;
const int kTotalLap = 1; //  圈数（资格赛）
const float kPsteer = 60, kDsteer = 100;
const float kPmotor = 200, kImotor = 50, kDmotor = 100, kMaxMotorError = 200;
uint16_t AD1 = 0, AD2 = 0, AD3 = 0, AD4 = 0, ADV = 0;
int count = 0;
float pre_offset = 0, offset = 0;
float offset_p = 0, offset_d = 0;
float pre_speed_error = 0, speed_error = 0, sum_speed_error = 0;
float motor_p = 0, motor_i = 0, motor_d = 0;
int speed = 0, steer = 0;
int isRing = 0; // 1：第一次垂直电感到达阈值，2：第二次，3：第三次
char spring_oled[20];
uint8 data_getstring[2];

// 舵机打角设定，0为打直，绝对值最大160，左负右正
void SetSteer(float dir)
{
    if (dir > 160)
    {
        dir = 160;
    }
    else if (dir < -160)
    {
        dir = -160;
    }
    FTM_PWM_Duty(ftm0, ftm_ch0, dir + kMidSteer);
}

// 电机电压设定，支持直接设置负数
void SetMotor(int s)
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

// 自定义显示函数，输出AD，speed，steer，isRing，count
void MYOledShow()
{
    if (Pin(H7))
    {
        OLED_Clear(0x00);
        sprintf(spring_oled, "L:%5d R:%5d", AD1, AD4);
        OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
        sprintf(spring_oled, "V:%5d L-R:%3d", ADV, AD1 - AD4);
        OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
        sprintf(spring_oled, "S%3d D%3d R%d", speed, steer, isRing);
        OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);
        sprintf(spring_oled, "Count:%3d", count);
        OLED_Show_String(8, 16, 0, 48, 1, spring_oled, 0);
        OLED_Refresh_Gram();
    }
}

void GetSpeed()
{
    count = FTM_Pulse_Get(ftm1); //编码器数值读取
    if (!Pin(H6))
        count = -count;
    FTM_Count_Clean(ftm1); //编码器数值清零
    speed = count / 8.0;
}

// 舵机PD控制
float PDSteer()
{
    float P, D;
    AD1 = ADC_Read(ADC0_SE1);
    ADV = ADC_Read(ADC0_SE2);
    AD4 = ADC_Read(ADC0_SE9);
    offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);

    pre_offset = offset;
    P = kPsteer * offset;
    D = kDsteer * (offset - pre_offset);
    return (P + D);
}

// 电机PID控制
float PIDMotor(float speed_target)
{
    float P, I, D;
    speed_error = speed_target - speed; 
    pre_speed_error = speed_error;
    sum_speed_error += speed_error;
    if (sum_speed_error > kMaxMotorError)
        sum_speed_error = kMaxMotorError;
    else if (sum_speed_error < - kMaxMotorError)
        sum_speed_error = - kMaxMotorError;
    P = kPmotor * speed_error;
    I = kImotor * sum_speed_error;
    D = kDmotor * (speed_error - pre_speed_error);
    return (P + I + D);
}

// PID控制
void Control()
{
    GetSpeed();

    steer = PDSteer();
    SetSteer(steer); //乘数为转弯系数
    // speed = kTopSpeed - 0.1 * turnconvert(fabs(offset));
    SetMotor(PIDMotor(kTopSpeed - 0.1 * steer)); //在offset<24时不减速

    MYOledShow();
}

void MYInit()
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
    GPIO_Init(G1, GPO, LOW); // B
    //GPIO_Init(G2, GPO, LOW); // G
    //GPIO_Init(G3, GPO, LOW); // R
}