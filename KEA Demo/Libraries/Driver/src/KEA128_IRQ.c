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
//  @brief      �ⲿ�ж�
//  @param      void            
//  @return     void
//  @since      v1.0
//  @note       ���ŵ�ѹ���ó���VDD������IRQֻ��ӳ�䵽һ�����ţ�����ظ���ʼ����IRQ���Ż�ӳ�䵽���һ�γ�ʼ���������š�
//  Sample usage:               irq_init();
//								set_irq_priority(IRQ_IRQn,1);					//�������ȼ�,�����Լ����������� �����÷�ΧΪ 0 - 3
//								enable_irq(IRQ_IRQn);							//��pit0���жϿ���
//								EnableInterrupts;								//���ܵ��жϿ���  
//  @note                       irqͬһʱ��ֻ�ܶ�һ�����Ž��м��
//                              �����жϺ��������ʹ��CLEAR_IRQ_FLAG�궨������жϱ�־λ
//-------------------------------------------------------------------------------------------------------------------

void IRQ_Init(PTX_n ptx_n,char pullup, char edg, char mod)
{

    uint32 temp;
    uint8 ptn;
    if(ptx_n == A5)//000ΪA5//A5������λ�ڣ�������
    {
        temp = SIM_BASE_PTR->PINSEL;
        temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
        SIM_BASE_PTR->PINSEL = temp | SIM_PINSEL_IRQPS(0);
    }
    else
    {
        ptn = PTn(ptx_n);//001-111ΪPTI0-PTI6��ȡ���ź�
        
        temp = SIM_BASE_PTR->PINSEL;
         temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
         SIM_BASE_PTR->PINSEL = temp | SIM_PINSEL_IRQPS(ptn+1);
    }
    
    SIM_BASE_PTR->SCGC |= SIM_SCGC_IRQ_MASK;     //����IRQʱ��
    IRQ_BASE_PTR->SC = (0
            | (pullup << IRQ_SC_IRQPDD_SHIFT)     //1�������������� 0��ʹ����������  ע�ʹ���0
            | (edg << IRQ_SC_IRQEDG_SHIFT)        //1�������ػ��߸ߵ�ƽ���� 0���½��ػ��ߵ͵�ƽ����   ע�ʹ���0
            | IRQ_SC_IRQPE_MASK         //ʹ��IRQ
            | (mod << IRQ_SC_IRQMOD_SHIFT)      //1�����������ƽ����  0��ֻ�����ش���  ע�ʹ���0
            );
    Soft_Delay_us(120000/bus_clk_khz); //�ȴ�ʮ����������
    IRQ_BASE_PTR->SC =(0
                     | IRQ_SC_IRQACK_MASK      //����жϱ�־λ,��Ҫ������IRQ�жϺ����
                     | IRQ_SC_IRQIE_MASK         //�����ж�
                     );
}

//����жϱ�־λ
char IRQ_Flag_Check(void)
{
  char flag;
  flag = (IRQ_SC & IRQ_SC_IRQF_MASK) >> IRQ_SC_IRQF_SHIFT;
  return flag;
}
//����жϱ�־λ
void IRQ_Flag_Clean(void)
{
  IRQ_SC |= IRQ_SC_IRQACK_MASK;
}
