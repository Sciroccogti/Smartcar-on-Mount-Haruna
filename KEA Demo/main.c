// 比完赛晚上秋名山见
// Last updated: 2-21-2019 By 张逸帆

#include "main.h"
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
    sprintf(spring_oled, "%d", AD1);
    //OLED_Show_String(8,16,0,20,1,spring_oled,0);
    sprintf(spring_oled, "%d", AD4);
    //OLED_Show_String(8,16,80,20,1,spring_oled,0);
    //OLED_Refresh_Gram();
    offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
    float stop_offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 20);
    const int straight_adjust_thres = 10, turn_thres = 60;

    if (AD1 + AD4 <= 30 && stop_offset < 30) //停下来
    {
      //SetMotor(kBrakeSpeed);
      //Soft_Delay_ms(600);
      SetMotor(0);
      //while(1);
    }
    else
    {
      if (fabs(offset) > straight_adjust_thres && fabs(offset) <= turn_thres)
      {                                                                                                    //直道调整
        SetSteer((int)(kMidSteer - (offset > 0 ? 1 : -1) * (fabs(offset) - straight_adjust_thres) * 1.7)); //乘数为转弯系数
        SetMotor(kTopSpeed);                                                                               //除数为减速系数
      }
      else if (fabs(offset) > turn_thres)
      { //转弯的offset阈值

        /*if(fabs(offset)>turn_thres+30)
      {
        SetMotor(kBrakeSpeed-fabs(offset)*5);
        Soft_Delay_ms(300);//刹车时间
      }
      else*/
        SetMotor(kTopSpeed - fabs(offset) / 3.5);  //除数为减速系数
        SetSteer((int)(kMidSteer - offset * 2.8)); //乘数为转弯系数
      }
      else
      { //直行
        SetSteer(kMidSteer);
        SetMotor(kTopSpeed);
      }
    }
    sprintf(spring_oled, "%.2f", offset);
    OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
    sprintf(spring_oled, "%d , %d", AD1, AD4);
    OLED_Show_String(8, 16, 0, 20, 1, spring_oled, 0);
    OLED_Refresh_Gram();
  }
  SetSteer((int)(kMidSteer - offset * 2.8));
}
