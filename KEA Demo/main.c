#include "common.h"
char spring_oled[20];
uint8 data_getstring[2];
uint16_t AD1 = 0, AD2 = 0;
uint16_t count;
float pre_offset = 0, offset = 0;

void Control()
{
  //补充你的控制代码

  //读取AD值
  AD1 = ADC_Read(ADC0_SE1);
  AD2 = ADC_Read(ADC0_SE2);

  //舵机控制 建议使用位置式PD控制 请参考相应的手册
  pre_offset = offset;
  offset = (float)1000 * (AD1 - AD2) * 1.0 / (AD1 + AD2 + 1);

  count = FTM_Pulse_Get(ftm1); //编码器数值读取
  FTM_Count_Clean(ftm1);       //编码器数值清零

  //电机控制，建议对电机与舵机的占空比限幅，电机0~100%，舵机根据安装情况设置
  FTM_PWM_Duty(ftm2,ftm_ch0,100);
}

void PIT_Interrupt(uint8 ch)
{
  GPIO_Turn(G2);
  GPIO_Turn(G3);
}

int main(void)
{
  while ((1280 * ex_clk_khz) != (256 * ics_clk_khz))
    ; //确保时钟配置无误
  GPIO_Init(G1, GPO, LOW);
  Soft_Delay_ms(1000);
  GPIO_Turn(G1);
  GPIO_Init(G2, GPO, LOW);
  GPIO_Init(G3, GPO, HIGH);

  PIT_Init1(pit0, 100000);
  PIT_SetCallback(PIT_Interrupt);
  Disable_Interrupt(INT_PIT_CH0);
  Enable_Interrupt(INT_PIT_CH0);
  FTM_PWM_Init(ftm0, ftm_ch0, A0, 300, 500);   //舵机
  FTM_PWM_Init(ftm2, ftm_ch1, F1, 14000, 100); //电机

  //ADC
  ADC_Init(ADC0_SE1, ADC_12bit);  //A1
  ADC_Init(ADC0_SE2, ADC_12bit);  //A6
  ADC_Init(ADC0_SE3, ADC_12bit);  //A7
  ADC_Init(ADC0_SE9, ADC_12bit);  //C1
  ADC_Init(ADC0_SE10, ADC_12bit); //C2

 //OLED部分书写（不确定） 
    OLED_Init();
    OLED_Display_Config(1);
    while(1){
       OLED_Clear(0x00);
       AD1 = ADC_Read(ADC0_SE1);
       AD2 = ADC_Read(ADC0_SE3);
       OLED_Show_String(8,16,0,20,1,(char*)AD1,0);
       OLED_Show_String(8,16,80,20,1,(char*)AD2,0);
       OLED_Refresh_Gram();
       //在0行0列显示AD1
       //里面的AD1要求是char,不知道强制转换能不能转化为char
      offset = (float)100*(AD1 - AD2)/(AD1 + AD2);
      FTM_PWM_Duty(ftm0, ftm_ch0, 500+offset);
    }
 //电机部分书写（不确定）
    while(1){
      AD1 = ADC_Read(ADC0_SE1);
      AD2 = ADC_Read(ADC0_SE3);
      if(AD1-AD2>30){
        offset = (float)1000*(AD1 - AD2)/(AD1 + AD2);
        FTM_PWM_Duty(ftm0, ftm_ch0, 500+offset);
        FTM_PWM_Duty(ftm2, ftm_ch1, 80-fabs(offset));
      }
      else {
        FTM_PWM_Duty(ftm0, ftm_ch0, 500);
        FTM_PWM_Duty(ftm2, ftm_ch1, 100);
      }
    }
}
