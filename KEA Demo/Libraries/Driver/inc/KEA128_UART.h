/*
  ******************************************************************************
  * @file    uart.h
  * @author  xhJiang
  * @version v1.1
  * @date    2018.5.6
  * @mail:   blog_xhjiang@163.com
  * @brief   UART configure
  ******************************************************************************
  */

#ifndef _KEA128_UART_H
#define _KEA128_UART_H


#include "common.h"
#include "stdio.h"




typedef enum
{
    uart0,
    uart1,
    uart2,
} UARTn;

//自制JLINK可加入UART接口用于无线下载和调试
//#define DEBUG_PORT  uart2
//#define DEBUG_BAUD  115200

//RXTX两个引脚相互绑定，为了简单初始化，直接重定义到RX单引脚上
#define RXTX_B0B1 B0    //uart0
#define RXTX_A2A3 A2    //uart0
#define RXTX_C6C7 C6    //uart1
#define RXTX_F2F3 F2    //uart1
#define RXTX_D6D7 D6    //uart2
#define RXTX_I0I1 I0    //uart2

extern uint32 UART_Init(UARTn uartn, uint32 baud, PTX_n ptx_n);
extern void UART_Putchar (UARTn uartn, uint8 ch);
extern void UART_Putbuff (UARTn uartn, uint8 *buff, uint32 len);
extern void UART_Putstr (UARTn uartn, const uint8 *str);
extern void UART_Getchar (UARTn uartn, uint8 *ch);
extern void UART_RX_IRQ_Enable(UARTn uartn);
extern void UART_TX_IRQ_Enable(UARTn uartn);
extern void UART_RX_IRQ_Disable(UARTn uartn);
extern void UART_TX_IRQ_Disable(UARTn uartn);
extern void UART_SetCallback(pt2FuncU8 ptr);
extern void UART_Putint16(UARTn uartn, int16_t var);
#endif
