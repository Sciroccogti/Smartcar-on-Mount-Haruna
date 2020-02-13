#include "driver.h"

const int kTopSpeed = 200; //  速度上限
const int kMidSteer = 520; // 舵机物理中值

const FTMn encoder_port = ftm1; // 编码器接口

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
    FTM_PWM_Duty(ftm0, ftm_ch0, (int)(dir + kMidSteer));
}

// 电机电压设定，支持直接设置负数
void SetMotor(float floats)
{
    int s = (int)floats;
    if (s > 0)
    {
        FTM_PWM_Duty(ftm2, ftm_ch0, s < kTopSpeed ? s : kTopSpeed); // 设置了速度上限：kTopSpeed
    }
    else if (!s)
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, 0);
        FTM_PWM_Duty(ftm2, ftm_ch0, 0);
    }
    else
    {
        FTM_PWM_Duty(ftm2, ftm_ch1, -s < kTopSpeed ? -s : kTopSpeed); // 设置了速度下限：-kTopSpeed
    }
}

void SetMotor_d(float s)
{
    const float apower = 400;
    const float dpower = 2000;

    float fade = fabs(s - count) < 8 ? fabs(s - count) * 0.125 : 1.0;

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

// 刷新数据
void Update()
{
    count = FTM_Pulse_Get(encoder_port); //编码器数值读取
    if (Pin(H6))                         // 电机倒转
        count = -count;
    if (distance <= 32767)
        distance += count;
    FTM_Count_Clean(ftm1); //编码器数值清零

    uint16 result[5];
    ADC_Get(result);

    AD1 = result[0];
    AD2 = result[1];
    AD3 = result[2];
    AD4 = result[3];
    ADV = result[4];
}

void ADC_Get(uint16 result[5])
{
    uint16 list[5][5], temp;
    const ADCn_Chn ADC0[5] = {ADC0_SE1, ADC0_SE3, ADC0_SE2, ADC0_SE9, ADC0_SE10};
    int i, j, k;

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            list[j][i] = (uint16)ADC_Avg(ADC0[j]);
        }
    }

    for (i = 0; i < 5; i++) // 冒泡升序排序
    {
        for (j = 0; j < 4; j++)
        {
            for (k = 0; k < 4 - j; k++) //五个点感知只需要交换4次
            {
                if (list[i][k] > list[i][k + 1]) //前面的比后面的大则进行交换
                {
                    temp = list[i][k + 1];
                    list[i][k + 1] = list[i][k];
                    list[i][k] = temp;
                }
            }
        }
    }
    // 中值滤波
    for (i = 0; i < 5; i++) // 求中间三项的和
    {
        result[i] = list[i][1] + list[i][2] + list[i][3]; // 舍去最大最小取中间三项
        result[i] = result[i] / 3;                        // 求平均值
        result[i] = result[i] / 10 * 10;         // 将数值中个位数除掉,降低过高精度
    }
}

uint16 ADC_Avg(ADCn_Chn adcn_chn)
{
    uint32 ret = 0;
    uint8 i, j;
    for (i = 0; i < 5; i++)
    {
        ret += ADC_Read(adcn_chn);
        j = 20;
        while (j--)
            ;
    }
    ret /= 5;
    return (uint16)ret;
}