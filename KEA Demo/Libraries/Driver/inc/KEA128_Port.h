/*
  ******************************************************************************
  * @file    port.h
  * @author  xhJiang
  * @version V1.0
  * @date    2018.3.31
  * @mail:   blog_xhjiang@163.com
  * @brief   port configure
  ******************************************************************************
  */



#ifndef KEA128_PORT_H_
#define KEA128_PORT_H_

#include "common.h"

typedef enum //枚举端口滤波模块
{
  FLTA = 0U,//针对PTA 输入的滤波器选择
  FLTB,
  FLTC,
  FLTD,
  FLTE,
  FLTF,
  FLTG,
  FLTH,
  FLTRST,//针对RESET/IRQ 输入的滤波器选择
  FLTKBI0,//针对KBI0 输入的滤波器选择
  FLTKBI1,
  FLTNMI,//针对NMI 输入的滤波器选择
  //-----------------以上由PORT_IOFLT0控制
  FLTI,
  FLTRESERVED,//保留
  FLTIRQ,//针对IRQ 输入的滤波器选择
  FLTFTM0,//针对FTM0CH0/FTM0CH1 输入的滤波器选择
  FLTFTM1,
  FLTPWT,//针对PWT_IN1/PWT_IN0 输入的滤波器选择
  FLTI2C0,//针对SCL0/SDA0 输入的滤波器选择
  FLTI2C1,
  //-----------------以上由PORT_IOFLT1控制
}PORT_IOFLT;

typedef enum //枚举端口滤波分频比
{
  FLTDIV1 = 0,//总线BUSCLK不分频
  //-------------------
  FLTDIV2,//总线2分频
  FLTDIV4,
  FLTDIV8,
  FLTDIV16,
  //-------------FLTDIV1
  FLTDIV32,
  FLTDIV64,
  FLTDIV128,
  FLTDIV256,
  FLTDIV512,
  FLTDIV1024,
  FLTDIV2048,
  FLTDIV4096,
  //-----------FLTDIV2
  FLTLPO1,//LPOCLK不分频
  FLTLPO2,
  FLTLPO4,
  FLTLPO8,
  FLTLPO16,
  FLTLPO32,
  FLTLPO64,
  FLTLPO128,
  //-----------FLTDIV3
  
}PORT_IOFLTDIV;


//端口滤波配置
extern void Port_Ioflt(PORT_IOFLT ioflt,PORT_IOFLTDIV iofltdiv);
//上拉配置
extern void Port_Pue(PTX_n ptx_n,char cfg);
//大电流驱动配置
extern int Port_Pdrve(PTX_n ptx_n,char cfg);

#endif
