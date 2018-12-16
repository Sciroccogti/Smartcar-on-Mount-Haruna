//  2018年12月15日  朱雨齐&张逸帆
//  校正了舵机
//  加注释是为了尝试实现PR
#include "common.h"
char spring_oled[20];
uint8 data_getstring[2];
uint16_t AD1 = 0, AD2 = 0;
uint16_t count;
float pre_offset = 0, offset = 0;
const int speed=132;
const float mid=530.0;
void Control()
{
  //补充你的控制代码

  //读取AD值
  AD1 = ADC_Read(ADC0_SE1);
  AD2 = ADC_Read(ADC0_SE3);

  //舵机控制 建议使用位置式PD控制 请参考相应的手册
  pre_offset = offset;
  offset = (float)1000 * (AD1 - AD2) * 1.0 / (AD1 + AD2 + 1);

  count = FTM_Pulse_Get(ftm1); //编码器数值读取
  FTM_Count_Clean(ftm1);       //编码器数值清零

  //电机控制，建议对电机与舵机的占空比限幅，电机0~100%，舵机根据安装情况设置
  FTM_PWM_Duty(ftm2,ftm_ch0,400);
}

void PIT_Interrupt(uint8 ch)
{
  GPIO_Turn(G2);
  GPIO_Turn(G3);
}

void OLED_Myshow(void)
{
  OLED_Clear(0x00);
  sprintf(spring_oled, "LL:%d", 1234); //
  OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
  OLED_Refresh_Gram();
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
  FTM_PWM_Init(ftm2, ftm_ch1, F1, 14000, 110); //电机

  //ADC
  ADC_Init(ADC0_SE1, ADC_12bit);  //A1,AD1
  ADC_Init(ADC0_SE2, ADC_12bit);  //A6,AD3
  ADC_Init(ADC0_SE3, ADC_12bit);  //A7,AD2
  ADC_Init(ADC0_SE9, ADC_12bit);  //C1,AD4
  ADC_Init(ADC0_SE10, ADC_12bit); //C2,AD5


 //OLED部分书写（不确定） 
    OLED_Init();
    
    OLED_Display_Config(1);
    while(1){
       OLED_Clear(0x00);
       AD1 = ADC_Read(ADC0_SE1);
       AD2 = ADC_Read(ADC0_SE3);
       //sprintf(spring_oled, "%d", AD1);
       //OLED_Show_String(8,16,0,20,1,spring_oled,0);
       //sprintf(spring_oled, "%d", AD2);
       //OLED_Show_String(8,16,80,20,1,spring_oled,0);
       //OLED_Refresh_Gram();
       offset = (float)100*(AD1 - AD2)/(AD1 + AD2 + 10);
       const int straight_adjust_thres = 30, turn_thres = 45;
       /*if(fabs(offset)>straight_adjust_thres&&fabs(offset)<=turn_thres){//直道调整
         FTM_PWM_Duty(ftm0, ftm_ch0, (int)(mid-(offset>0?1:-1)*(fabs(offset)-straight_adjust_thres)*0.7));//乘数为转弯系数
        FTM_PWM_Duty(ftm2, ftm_ch1, speed);//除数为减速系数
       }
       else*/ if(fabs(offset)>turn_thres){//转弯的offset阈值
        FTM_PWM_Duty(ftm0, ftm_ch0, (int)(mid-offset*2.6));//乘数为转弯系数
        if(fabs(offset)>turn_thres+20)FTM_PWM_Duty(ftm2, ftm_ch1, speed-fabs(offset)/4.8);//除数为减速系数
        else FTM_PWM_Duty(ftm2, ftm_ch1, speed-fabs(offset)/2.7);//除数为减速系数
       }
       else {//直行
        FTM_PWM_Duty(ftm0, ftm_ch0, mid);
        FTM_PWM_Duty(ftm2, ftm_ch1, speed);
      }
      sprintf(spring_oled, "%.2f", offset);
      OLED_Show_String(8,16,0,20,1,spring_oled,0);
      OLED_Refresh_Gram();
    }
    }
