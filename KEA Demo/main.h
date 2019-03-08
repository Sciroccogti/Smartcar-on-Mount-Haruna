// 比完赛晚上秋名山见 v1.7
// Last updated: 3-7-2019 By 张逸帆
// What's new: 封装数据采集
// 命名规范参见https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#
// const常量请以 k 开头，大小写混合
// 函数请以大写开头，大小写混合

#include "common.h"

char spring_oled[20];
uint8 data_getstring[2];
uint16_t AD1 = 0, AD2 = 0, AD3 = 0, AD4 = 0, ADV = 0;
int count = 0;
float pre_offset = 0, offset = 0;
int kTopSpeed = 500; //  速度上限
const float kMidSteer = 520.0;
const int kTotalLap = 1;              //  圈数（资格赛）
int speed = 0, steer = 0, isRing = 0; // 1：第一次垂直电感到达阈值，2：第二次，3：第三次
int flag = -1;                        // 速度控制标志，-1为自动速度

// 舵机打角设定，0为打直，绝对值最大160，左负右正
void SetSteer(float dir)
{
    if (dir > 170)
    {
        dir = 170;
    }
    else if (dir < -170)
    {
        dir = -170;
    }
    FTM_PWM_Duty(ftm0, ftm_ch0, dir + kMidSteer);
}

// 刷新数据
void Refresh()
{
    count = FTM_Pulse_Get(ftm1); //编码器数值读取
    if (Pin(H6))
        count = -count;
    //count *= 3;
    FTM_Count_Clean(ftm1); //编码器数值清零
    AD1 = ADC_Read(ADC0_SE1);
    AD2 = ADC_Read(ADC0_SE3);
    AD3 = ADC_Read(ADC0_SE2);
    AD4 = ADC_Read(ADC0_SE9);
    ADV = ADC_Read(ADC0_SE10);
}

void PIT_Interrupt(uint8 ch);

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
        sprintf(spring_oled, "2:%5d 3:%5d", AD2, AD3);
        OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
        sprintf(spring_oled, "V:%5d R%d F%d", ADV, isRing, flag);
        OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);
        sprintf(spring_oled, "Count:%3d", count);
        OLED_Show_String(8, 16, 0, 48, 1, spring_oled, 0);
        OLED_Refresh_Gram();
    }
}

double turnconvert(double x) //offset与舵机转向的转换函数
{
    const double a = 1.14828e-4, b = 6, c = 9.77256e-11; //b = 5.15858
    return exp(a * x * x + c * x + b) - exp(b);
}

/***********************************************************************************************
*
* @brief    UART_Interrupt(UINT8 ch). Callback function called by the UART module on each interrupt
* @param    UART channel
* @return   none
*
************************************************************************************************/

void UART_Interrupt(uint8 ch)
{
    uint8 data_get;
    UART_Getchar(uart2, &data_get);
    data_getstring[0] = data_get;
    if (data_getstring[0] == 's') //蓝牙发送s人工停止车模运行
    {
        GPIO_Turn(G1);
    }
    UART_Putstr(uart2, data_getstring);
}

void SetMotor_d(float s)
{
    const float apower = 1200;
    const float dpower = 5000;

    float fade = fabs(s - count) < 10 ? fabs(s - count) * 0.1 : 1.0;

    if (count > s && s > 0) //正向并且实际速度高于预期，减速
    {
        SetMotor(-0.2 * dpower * fade);
        //SetMotor(0);
    }
    else if (count <= s && s > 0) //正向并且实际速度低于预期，加速
    {
        SetMotor(apower * fade);
    }
    else if (count > 2 && s == 0) //s==0即停车，反转电机
    {
        {
            SetMotor(-dpower);
        }
        //Soft_Delay_ms(2);
    }
    else if (count <= 1 && s == 0) //停得差不多了，不反转电机
    {
        SetMotor(0);
    }
}

// 通用指数控制，flag控制速度，-1为默认，-2为关闭
void Control()
{
    const float StraightSpeed = 12, cornerspeed = 8;
    static int i = 0;
    static float diff = 0, prev_offset = 0;
    const float c = 15;

    if (flag == -2)
    {
        offset = (float)100 * (AD2 - AD3) / (AD2 + AD3 + 10);
    }
    else
    {
        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
    }

    //当offset导数小于某个正值的时候，转向幅度变小

    steer = -(offset > 0 ? 1.0 : -1.0) * (turnconvert(fabs(offset))) * 0.7; //乘数为转弯系数(diff < 0 ? diff * c : 0)
    speed = StraightSpeed / (1.0 + 0.004 * turnconvert(fabs(offset)));
    if (offset <= 20 && fabs(diff) >= 2)
    {
        steer -= diff * c;
        speed = speed / 1.5;
    }
    SetSteer(steer);
    if (flag == -1)
    {
        SetMotor_d(speed);
        //SetMotor_d(6);
    }
    else if (flag == -2)
    {
        SetMotor_d(2);
    }
    else
    {
        SetMotor_d(flag);
    }

    if (i % 20 == 0)
    {
        diff = offset - prev_offset;
        prev_offset = offset;
        i = 0;
    }
    else
        i++;
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
    GPIO_Init(G1, GPO, HIGH); // B
    GPIO_Init(G2, GPO, HIGH); // G
    GPIO_Init(G3, GPO, HIGH); // R

    //SW
    GPIO_Init(C5, GPI, 1); // SW1，控制起跑线检测模块
    GPIO_Init(H7, GPI, 1); // SW2，控制OLED显示函数
    GPIO_Init(H5, GPI, 1); // SW3，控制低速
    GPIO_Init(H2, GPI, 1); // SW4，控制全速

    //UART串口(蓝牙)
    // UART_Init(uart2, 9600, RXTX_B0B1);
    // UART_SetCallback(UART_Interrupt);
    // NVIC_SetPriority(UART2_IRQn, 0x02); //如果我们不对优先级进行配置的话，则默认相应中断源的向量号越低其优先级越高
    // UART_RX_IRQ_Disable(uart2);

    //PIT定时器
    PIT_Init1(pit0, 700); //单位us,0.1ms
    PIT_SetCallback(PIT_Interrupt);
    Disable_Interrupt(INT_PIT_CH0);
    Enable_Interrupt(INT_PIT_CH0);
}

void ChecktoStop()
{
    if (AD1 + AD4 <= 15) // 出赛道自动停车，赛时需要移除
    {
        AD1 = ADC_Read(ADC0_SE1);
        AD4 = ADC_Read(ADC0_SE9);
        if (AD1 + AD4 <= 10)
        {
            AD1 = ADC_Read(ADC0_SE1);
            AD4 = ADC_Read(ADC0_SE9);
            if (AD1 + AD4 <= 10)
            {
                flag = 0;
            }
        }
    }
    else
    {
        if (!flag)
            flag = -1;
    }
}