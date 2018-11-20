/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		KEA128_systick
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#include "KEA128_systick.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ����
//  @param      time            ��Ҫ��ʱ��ʱ��
//  @return     void
//  @since      v2.0
//  Sample usage:               systick_delay(1000);   //��ʱ1000���ں�ʱ������
//-------------------------------------------------------------------------------------------------------------------
void systick_delay(uint32 time)
{
    if(time == 0)   return;

    SysTick->CTRL = 0x00;//�ȹ��� systick ,���־λ
    SysTick->LOAD = time;//������ʱʱ��
    SysTick->VAL = 0x00;//��ռ�����
    SysTick->CTRL = ( 0 | SysTick_CTRL_ENABLE_Msk     //ʹ�� systick
                  //| SysTick_CTRL_TICKINT_Msk        //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                    | SysTick_CTRL_CLKSOURCE_Msk      //ʱ��Դѡ�� (core clk)
                );
    while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));//�ȴ�ʱ�䵽
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���뼶systick��ʱ����
//  @param      ms              ��ʱ���ٺ���
//  @return     void
//  @since      v2.0
//  Sample usage:               systick_delay_ms(1000);   //��ʱ1000����
//-------------------------------------------------------------------------------------------------------------------
void systick_delay_ms(uint32 ms)
{
    //get_clk();//��ȡ�ں�ʱ�ӱ��ں�������
	while(ms--) systick_delay(core_clk_khz);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ��
//  @param      time            ��ʱʱ��(0-0x00ffffff)
//  @return     void
//  @since      v2.0
//  Sample usage:               systick_timing(1000);   //��ʱ1000���ں�ʱ������
//-------------------------------------------------------------------------------------------------------------------
void systick_timing(uint32 time)
{
    time &= 0x00ffffff;     	//����ǿ�Ʊ任Ϊ24λ 
    SysTick->LOAD = time;       //������ʱʱ��
    SysTick->VAL = 0x00;       	//��ռ�����
    SysTick->CTRL = ( 0 
					| SysTick_CTRL_ENABLE_Msk       //ʹ�� systick
                    | SysTick_CTRL_TICKINT_Msk      //ʹ���ж�
                    | SysTick_CTRL_CLKSOURCE_Msk    //ʱ��Դѡ�� (core clk) 
                );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��õ�ǰSystem tick timer��ֵ
//  @return     ���ص�ǰSystem tick timer��ֵ
//  @since      v2.0
//  Sample usage:               uint32 tim = systick_getval();   
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(void)
{
    return SysTick->VAL;
}

