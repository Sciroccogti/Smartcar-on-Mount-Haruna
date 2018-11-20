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


//定义PIT模块号
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
//------------------------------------以下代码用于PIT中断------------------------------------
#define     PIT_Init_ms(PITn,ms)          PIT_Init1(PITn,ms*1000);        //初始化PITn，并设置定时时间(单位为 ms)
#define     PIT_Init_us(PITn,us)          PIT_Init1(PITn,us);             //初始化PITn，并设置定时时间(单位为 us)
#define     PIT_Init_ns(PITn,ns)          PIT_Init1(PITn,ns/1000);        //初始化PITn，并设置定时时间(单位为 ns)


//------------------------------------以下代码用于PIT延时------------------------------------
#define     PIT_Delay_ms(PITn,ms)          PIT_Delay1(PITn,ms*1000);       //PIT延时 ms
#define     PIT_Delay_us(PITn,us)          PIT_Delay1(PITn,us);            //PIT延时 us
#define     PIT_Delay_ns(PITn,ns)          PIT_Delay1(PITn,ns/1000);       //PIT延时 ns(100M总线频率时最小单位为10ns)


#define     PIT_Flag_Clear(PITn)          PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK        //清中断标志（写1 清空标志位）


#endif
