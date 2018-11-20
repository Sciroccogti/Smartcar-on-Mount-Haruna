/*
  ******************************************************************************
  * @file    pit.h
  * @author  xhJiang
  * @version V1.0
  * @date    2018.4.2
  * @mail:   blog_xhjiang@163.com
  * @brief   Pit configure
  ******************************************************************************
  */

#ifndef KEA128_PIT_H_
#define KEA128_PIT_H_


#include "common.h"


//����PITģ���
typedef enum
{
    pit0,
    pit1,
} PITn;


void PIT_Init1(PITn pitn, uint32 cnt);
void PIT_Init2(uint32 ms, uint32 times);
void PIT_Delay1(PITn pitn, uint32 cnt);
void PIT_Delay2(uint32 ms, uint32 times);
void PIT_Time_Start(PITn pitn);
uint32 PIT_Time_Get(PITn pitn);
void PIT_Close(PITn pitn);
void PIT_SetCallback(pt2FuncU8 ptr);
//------------------------------------���´�������PIT�ж�------------------------------------
#define     PIT_Init_ms(PITn,ms)          PIT_Init1(PITn,ms*1000);        //��ʼ��PITn�������ö�ʱʱ��(��λΪ ms)
#define     PIT_Init_us(PITn,us)          PIT_Init1(PITn,us);             //��ʼ��PITn�������ö�ʱʱ��(��λΪ us)
#define     PIT_Init_ns(PITn,ns)          PIT_Init1(PITn,ns/1000);        //��ʼ��PITn�������ö�ʱʱ��(��λΪ ns)


//------------------------------------���´�������PIT��ʱ------------------------------------
#define     PIT_Delay_ms(PITn,ms)          PIT_Delay1(PITn,ms*1000);       //PIT��ʱ ms
#define     PIT_Delay_us(PITn,us)          PIT_Delay1(PITn,us);            //PIT��ʱ us
#define     PIT_Delay_ns(PITn,ns)          PIT_Delay1(PITn,ns/1000);       //PIT��ʱ ns(100M����Ƶ��ʱ��С��λΪ10ns)


#define     PIT_Flag_Clear(PITn)          PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK        //���жϱ�־��д1 ��ձ�־λ��


#endif
