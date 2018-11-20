/*
  ******************************************************************************
  * @file    irq.h
  * @author  xhJiang
  * @version V1.0
  * @date    2018.4.4
  * @mail:   blog_xhjiang@163.com
  * @brief   Irq configure
  ******************************************************************************
  */

#ifndef KEA128_IRQ_H_
#define KEA128_IRQ_H_


#include "common.h"



#define IRQ_PULLUP_EN     1
#define IRQ_PULLUP_DIS    0

#define IRQ_RISING_EDGE   1
#define IRQ_FALLING_EDGE  0

#define IRQ_EDGEONLY      0
#define IRQ_EDGE_LEVEL    1

extern void IRQ_Init(PTX_n ptx_n,char pullup, char edg, char mod);
extern char IRQ_Flag_Check(void);
extern void IRQ_Flag_Clean(void);




#endif
