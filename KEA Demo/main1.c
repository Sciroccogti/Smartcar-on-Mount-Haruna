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
  AD2 = ADC_Read(ADC0_SE3);

  //舵机控制 建议使用位置式PD控制 请参考相应的手册
  pre_offset = offset;
  offset = (float)1000.0 * (AD1 - AD2) * 1.0 / (AD1 + AD2 + 1);

  count = FTM_Pulse_Get(ftm1); //编码器数值读取
  FTM_Count_Clean(ftm1);       //编码器数值清零

  //电机控制，建议对电机与舵机的占空比限幅，电机0~100%，舵机根据安装情况设置
  //FTM_PWM_Duty(ftm2,ftm_ch0,100);
}

void PIT_Interrupt(uint8 ch)
{
  //翻转核心板灯观察工作状态
  GPIO_Turn(G2);
  GPIO_Turn(G3);
  Control();
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
  OLED_Init();
  Soft_Delay_ms(100);
  OLED_Clear(0x00);

  //核心板3色RGB LED初始化,高电平灭,低电平亮
  GPIO_Init(G1, GPO, LOW);
  GPIO_Turn(G1);
  GPIO_Init(G2, GPO, LOW);
  GPIO_Init(G3, GPO, HIGH);
  GPIO_Init(I1, GPO, LOW); //蜂鸣器
  //FTM_PWM_Init(ftm0, ftm_ch0, A0, 50, 0); //舵机
  FTM_PWM_Init(ftm0, ftm_ch0, A0, 300, 500);

  FTM_Pulse_Init(ftm0, FTM_PS_1, TCLK1); //编码器  此处注意，使用DEF车模，初始化两个编码器，删除舵机初始化与控制，使用AB车模，删除此句编码器初始化，否则无法正常工作。
  FTM_Pulse_Init(ftm1, FTM_PS_1, TCLK2);

  FTM_PWM_Init(ftm2, ftm_ch0, F0, 14000, 0); //PWM1
  FTM_PWM_Init(ftm2, ftm_ch1, F1, 14000, 0); //PWM2
  FTM_PWM_Init(ftm2, ftm_ch4, G6, 14000, 0); //PWM3
  FTM_PWM_Init(ftm2, ftm_ch5, G7, 14000, 0); //PWM4

  //ADC
  ADC_Init(ADC0_SE1, ADC_12bit);  //A1
  ADC_Init(ADC0_SE2, ADC_12bit);  //A6
  ADC_Init(ADC0_SE3, ADC_12bit);  //A7
  ADC_Init(ADC0_SE9, ADC_12bit);  //C1
  ADC_Init(ADC0_SE10, ADC_12bit); //C2

  //PIT定时器
  PIT_Init1(pit0, 100000); //单位us,0.1ms
  PIT_SetCallback(PIT_Interrupt);
  Disable_Interrupt(INT_PIT_CH0);
  Enable_Interrupt(INT_PIT_CH0);
  while (1)
  {
    //GPIO_Turn(I1);
    //Control();
    FTM_PWM_Duty(ftm0, ftm_ch0, 700);
  }
}
/*
 // 搬来代码做做笔记
void PID()
{
  ppre_motor_error = pre_motor_error;
  pre_motor_error = motor_error;
  motor_error = motor_target - real_speed;
  //  if(motor_error>10)//加速p控制量 /// 限制加速度的，没用
  //  {
  //    motor_error = 10;
  //  }

  //增量式
  //  motor_ep = motor_error - pre_motor_error;
  //  motor_ei = motor_error;
  //  motor_ed = motor_error - 2*pre_motor_error + ppre_motor_error;
  //位置式
  motor_ep = motor_error;
  motor_ei += motor_error;
  if (motor_ei > 50)
    motor_ei = 50;
  else if (motor_ei < -50)
    motor_ei = -50;
  motor_ed = motor_error - pre_motor_error;
  //  分段积分I
  //  if((motor_error<(motor_target/2))&&(motor_error<-(motor_target/2)))
  //  motor_out += motor_kp * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  //  else  motor_out += motor_kp * motor_ep  + motor_kd * motor_ed;

  //  if(motor_error<=0) motor_ei=0;

  //分段比例P
  //  if((motor_error<(motor_set_speed/2))&&(motor_error>-(motor_set_speed/2)))
  //  motor_out += motor_kp * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  //  else  motor_out += motor_kp * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  //
  //  if((flag_stop==0))
  //    motor_out += motor_kp * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  //  else
  //    motor_out += motor_kp * motor_ep + motor_kd * motor_ed;

  //棒棒控制
  //  if(motor_error>80) motor_out = 800;

  //  if(motor_error<0)

  //  motor_out += 60 * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  //  else  motor_out += motor_kp * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  //  motor_out += motor_kp * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  
  if (state == STATE_ROUND)
  {
    offset_kd = 0;
  }
  else
    offset_kd = 15.12;

  if ((flag_stop == 0) && (offset_and_speed < 20) && (offset_and_speed > -20))
    motor_out = motor_kp * motor_ep + motor_ki * motor_ei + motor_kd * motor_ed;
  else if (state == STATE_ROUND && (round_size[round_count] == 0))
    motor_out = motor_kp * motor_ep + motor_ki * motor_ei;
  else
    motor_out = motor_kp * motor_ep; //*3

  if (max_ad > outrange_admax)
    lastactive_area = area; //有效区域
  pre_area = area;

  //    if(flag_stop==0)
  //    {
  if (offset_out > 0)
  {
    SetMotor((int)(motor_out + motor_target * 5.4));
  }
  else
  {
    SetMotor((int)(motor_out + motor_target * 5.4));
  }
}
*/