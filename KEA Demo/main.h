#include "common.h"

#define setMotor(speed) FTM_PWM_Duty(ftm2, ftm_ch1, speed)
#define setSteer(dir) FTM_PWM_Duty(ftm0, ftm_ch0, dir)

char spring_oled[20];
uint8 data_getstring[2];
uint16_t AD1 = 0, AD2 = 0, AD3 = 0, AD4 = 0;
uint16_t count;
float pre_offset = 0, offset = 0;
const int speed = 132;
const float mid = 520.0;

void MYInit()
{
    while ((1280 * ex_clk_khz) != (256 * ics_clk_khz))
        ; //确保时钟配置无误

    FTM_PWM_Init(ftm0, ftm_ch0, A0, 300, mid);   //舵机
    FTM_PWM_Init(ftm2, ftm_ch1, F1, 14000, 0); //电机

    //ADC
    ADC_Init(ADC0_SE1, ADC_12bit);  //A1,AD1
    ADC_Init(ADC0_SE3, ADC_12bit);  //A7,AD2
    ADC_Init(ADC0_SE2, ADC_12bit);  //A6,AD3
    ADC_Init(ADC0_SE9, ADC_12bit);  //C1,AD4
    ADC_Init(ADC0_SE10, ADC_12bit); //C2,AD5

    //OLED
    OLED_Init();
    OLED_Display_Config(1);
}