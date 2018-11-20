/*
  ******************************************************************************
  * @file    KEA128_KBI.h
  * @author  xhJiang
  * @version v1.0
  * @date    2018.5.28
  * @mail:   blog_xhjiang@163.com
  * @brief   KBI configure
  * @note    
  ******************************************************************************
  */


#ifndef _KEA128_KBI_H
#define _KEA128_KBI_H


#include "common.h"
#include "SKEAZ1284.h"


typedef enum
{
  KBI0_P0=0,  //A0
  KBI0_P1,    //A1
  KBI0_P2,    //A2
  KBI0_P3,    //A3
  KBI0_P4,    //A4
  KBI0_P5,    //A5
  KBI0_P6,    //A6
  KBI0_P7,    //A7
  KBI0_P8,    //B0
  KBI0_P9,    //B1
  KBI0_P10,   //B2
  KBI0_P11,   //B3
  KBI0_P12,   //B4
  KBI0_P13,   //B5
  KBI0_P14,   //B6
  KBI0_P15,   //B7
  KBI0_P16,   //C0
  KBI0_P17,   //C1
  KBI0_P18,   //C2
  KBI0_P19,   //C3
  KBI0_P20,   //C4
  KBI0_P21,   //C5
  KBI0_P22,   //C6
  KBI0_P23,   //C7
  KBI0_P24,   //D0
  KBI0_P25,   //D1
  KBI0_P26,   //D2
  KBI0_P27,   //D3
  KBI0_P28,   //D4
  KBI0_P29,   //D5
  KBI0_P30,   //D6
  KBI0_P31,   //D7
              
  KBI1_P0,    //E0
  KBI1_P1,    //E1
  KBI1_P2,    //E2
  KBI1_P3,    //E3
  KBI1_P4,    //E4
  KBI1_P5,    //E5
  KBI1_P6,    //E6
  KBI1_P7,    //E7
  KBI1_P8,    //F0
  KBI1_P9,    //F1
  KBI1_P10,   //F2
  KBI1_P11,   //F3
  KBI1_P12,   //F4
  KBI1_P13,   //F5
  KBI1_P14,   //F6
  KBI1_P15,   //F7
  KBI1_P16,   //G0
  KBI1_P17,   //G1
  KBI1_P18,   //G2
  KBI1_P19,   //G3
  KBI1_P20,   //G4
  KBI1_P21,   //G5
  KBI1_P22,   //G6
  KBI1_P23,   //G7
  KBI1_P24,   //H0
  KBI1_P25,   //H1
  KBI1_P26,   //H2
  KBI1_P27,   //H3
  KBI1_P28,   //H4
  KBI1_P29,   //H5
  KBI1_P30,   //H6
  KBI1_P31,   //H7
    
}KBIx_CHn;


#define KBIx(KBIx_CHn)   (KBIx_CHn>>5)        //获取模块号
#define KBIn(KBIx_CHn)   (KBIx_CHn&0x1f)      //获取引脚号

#define CLEAN_KBI0_FLAG  (KBI0->SC |= KBI_SC_KBACK_MASK )
#define CLEAN_KBI1_FLAG  (KBI1->SC |= KBI_SC_KBACK_MASK )

#define READ_KBI0_FLAG   (KBI0->SP)
#define READ_KBI1_FLAG   (KBI1->SP)

// 枚举PORT 配置
typedef enum
{
  IRQ_FALLING,            //下降沿触发
  IRQ_RISING,             //上升沿触发
  IRQ_FALLING_LOW,        //下降沿和低电平触发
  IRQ_RISING_HIGH,        //上升沿和高电平触发
} TRIG_CFG;


void KBI_Init(KBIx_CHn chn, TRIG_CFG cfg);
void KBI_Deinit(KBIx_CHn chn);

/***********************************************************************************************
*
* @brief    KBI_SetCallback - Set a callback function to execute on a falling edge of an enabled KBI pin
* @param    pointer to function with an UINT8 argument
* @return   none
*
************************************************************************************************/
void KBI_SetCallback(pt2FuncU8 ptr);

/***********************************************************************************************
*
* @brief    KBI0_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void KBI0_IRQHandler(void);

void KBI1_IRQHandler(void);

#endif
