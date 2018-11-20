/*
  ******************************************************************************
  * @file    irq.c
  * @author  xhJiang
  * @version V1.0
  * @date    2018.4.4
  * @mail:   blog_xhjiang@163.com
  * @brief   Irq configure
  ******************************************************************************
  */

#include "common.h"
#include "KEA128_IRQ.h"
#include "delay.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      外部中断
//  @param      void            
//  @return     void
//  @since      v1.0
//  @note       引脚电压不得超过VDD！！！IRQ只能映射到一个引脚，如果重复初始化，IRQ引脚会映射到最后一次初始化配置引脚。
//  Sample usage:               irq_init();
//								set_irq_priority(IRQ_IRQn,1);					//设置优先级,根据自己的需求设置 可设置范围为 0 - 3
//								enable_irq(IRQ_IRQn);							//打开pit0的中断开关
//								EnableInterrupts;								//打开总的中断开关  
//  @note                       irq同一时刻只能对一个引脚进行检测
//                              进入中断函数后务必使用CLEAR_IRQ_FLAG宏定义清除中断标志位
//-------------------------------------------------------------------------------------------------------------------

void IRQ_Init(PTX_n ptx_n,char pullup, char edg, char mod)
{

    uint32 temp;
    uint8 ptn;
    if(ptx_n == A5)//000为A5//A5用作复位口，不可用
    {
        temp = SIM_BASE_PTR->PINSEL;
        temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
        SIM_BASE_PTR->PINSEL = temp | SIM_PINSEL_IRQPS(0);
    }
    else
    {
        ptn = PTn(ptx_n);//001-111为PTI0-PTI6获取引脚号
        
        temp = SIM_BASE_PTR->PINSEL;
         temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
         SIM_BASE_PTR->PINSEL = temp | SIM_PINSEL_IRQPS(ptn+1);
    }
    
    SIM_BASE_PTR->SCGC |= SIM_SCGC_IRQ_MASK;     //开启IRQ时钟
    IRQ_BASE_PTR->SC = (0
            | (pullup << IRQ_SC_IRQPDD_SHIFT)     //1：禁用上拉电阻 0：使能上拉电阻  注释代表0
            | (edg << IRQ_SC_IRQEDG_SHIFT)        //1：上升沿或者高电平触发 0：下降沿或者低电平触发   注释代表0
            | IRQ_SC_IRQPE_MASK         //使能IRQ
            | (mod << IRQ_SC_IRQMOD_SHIFT)      //1：检测边沿与电平触发  0：只检测边沿触发  注释代表0
            );
    Soft_Delay_us(120000/bus_clk_khz); //等待十个机器周期
    IRQ_BASE_PTR->SC =(0
                     | IRQ_SC_IRQACK_MASK      //清空中断标志位,需要在启动IRQ中断后清空
                     | IRQ_SC_IRQIE_MASK         //开启中断
                     );
}

//检查中断标志位
char IRQ_Flag_Check(void)
{
  char flag;
  flag = (IRQ_SC & IRQ_SC_IRQF_MASK) >> IRQ_SC_IRQF_SHIFT;
  return flag;
}
//清空中断标志位
void IRQ_Flag_Clean(void)
{
  IRQ_SC |= IRQ_SC_IRQACK_MASK;
}
