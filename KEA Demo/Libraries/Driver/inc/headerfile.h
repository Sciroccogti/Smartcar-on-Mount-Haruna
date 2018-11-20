/*
  ******************************************************************************
  * @file    headerfile.h
  * @author  xhJiang
  * @version v1.0
  * @date    2018.3.31
  * @mail:   blog_xhjiang@163.com
  * @brief   include all header files
  ******************************************************************************
  */
#ifndef _HEADERFILE_H_
#define _HEADERFILE_H_

#include <stdio.h>
#include "SKEAZ1284.h"
#include "system_SKEAZ1284.h"
//通用文件
#include "delay.h"

#include "OLED_0_96.h"
#include "TFT_1_8.h"
#include "font.h"
//以下为KEA专用库文件，已测试
#include "KEA128_Port.h"
#include "KEA128_GPIO.h"
#include "KEA128_ADC.h"
#include "KEA128_PIT.h"
#include "KEA128_RTC.h"
#include "KEA128_IRQ.h"
#include "KEA128_FTM.h"
#include "KEA128_UART.h"
#include "KEA128_KBI.h"
#include "KEA128_FLASH.h"

//以下为尚未测试内容
#include "KEA128_port_cfg.h"
//#include "KEA128_i2c.h"

#include "KEA128_spi.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_MPU6050.h"

#endif
