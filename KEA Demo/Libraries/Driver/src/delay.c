/*
  ******************************************************************************
  * @file    delay.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.4.2
  * @mail:   blog_xhjiang@163.com
  * @brief   Soft delay configure
  * @note    ͨ�����ԣ�16M����ʱ������10%����
  ******************************************************************************
  */

#include "delay.h"

//������ʱ
void Soft_Delay_us(uint32 us)
{
    volatile uint32_t i = us / 10;

    volatile uint32_t j ;

    while(i--)
    {
        j = core_clk_khz/1000;

        while(j--);
    }

}

void Soft_Delay_ms(uint32 ms)
{
    volatile uint32_t i = ms;

    volatile uint32_t j ;
    while(ms--)
    {
        Soft_Delay_us(1000);
    }
}



