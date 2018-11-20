/*
  ******************************************************************************
  * @file    rtc.h
  * @author  xhJiang
  * @version V1.0
  * @date    2018.4.2
  * @mail:   blog_xhjiang@163.com
  * @brief   rtc configure
  ******************************************************************************
  */


#ifndef KEA128_RTC_H_
#define KEA128_RTC_H_

#include "common.h"

extern void RTC_Timeset(uint16 timeset);
extern void RTC_Init(void);
extern uint16 RTC_Read(void);
extern void RTC_Clear(void);
extern uint8 RTC_Check(void);
#endif
