#include "common.h"

void PIT_Interrupt(uint8 ch)
{
  GPIO_Turn(G2);
  GPIO_Turn(G3);
}

int main(void)
{
  while ((1280 * ex_clk_khz) != (256 * ics_clk_khz))
    ; //确锟斤拷时锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
  GPIO_Init(G1, GPO, LOW);
  Soft_Delay_ms(1000);
  GPIO_Turn(G1);
  GPIO_Init(G2, GPO, LOW);
  GPIO_Init(G3, GPO, HIGH);

  PIT_Init1(pit0, 100000);
  PIT_SetCallback(PIT_Interrupt);
  Disable_Interrupt(INT_PIT_CH0);
  Enable_Interrupt(INT_PIT_CH0);
  //FTM_PWM_Init(ftm2,ftm_ch1,F1,14000,0); //PWM2 前
  FTM_PWM_Init(ftm0, ftm_ch0, A0, 300, 500);
  while (1)
  {
    FTM_PWM_Duty(ftm0, ftm_ch0, 700);
  }
}
