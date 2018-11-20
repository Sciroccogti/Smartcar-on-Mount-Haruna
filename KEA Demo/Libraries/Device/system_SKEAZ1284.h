/*
** ###################################################################
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          GNU C Compiler - CodeSourcery Sourcery G++
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    MKE06P80M48SF0RM, Rev. 1, Dec 2013
**     Version:             rev. 1.0, 2018-03-20
**     Build:               xhJiang
**     Mail:                blog_xhjiang@163.com
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright (c) 2018 SEU smartcar team, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.NXP.com
**
**     Revisions:
**     - rev. 1.0 (2013-07-30)
**         Initial version.
**     - rev. 1.1 (2013-10-29)
**         Definition of BITBAND macros updated to support peripherals with 32-bit acces disabled.
**     - rev. 1.2 (2014-01-10)
**         CAN module: corrected address of TSIDR1 register.
**         CAN module: corrected name of MSCAN_TDLR bit DLC to TDLC.
**         FTM0 module: added access macro for EXTTRIG register.
**         NVIC module: registers access macros improved.
**         SCB module: unused bits removed, mask, shift macros improved.
**         Defines of interrupt vectors aligned to RM.
**     - rev. 1.3 (2014-05-28)
**         The declaration of clock configurations has been moved to separate header file system_MKE02Z2.h
**         Module access macro {module}_BASES replaced by {module}_BASE_PTRS.
**         I2C module: renamed status register S to S1 to match RM naming.
**
** ###################################################################
*/

/*!
 * @file SKEAZ1284
 * @version 1.3
 * @date 2014-05-28
 * @brief Device specific configuration file for SKEAZ1284 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef SYSTEM_SKEAZ1284_H_
#define SYSTEM_SKEAZ1284_H_                      /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "common.h"

#define DISABLE_WDOG    1                       //禁用看门狗



//预定义ICS模块操作模式Predefined clock setups
#define ICS_WORKMODE_FEI   1    //默认模式，ICS提供一个来源于FLL的时钟，FLL时钟由内部基准时钟控制
#define ICS_WORKMODE_FEE   2    //ICS提供一个来源于FLL的时钟，该FLL时钟由外部基准时钟控制
#define ICS_WORKMODE_FBI   3    //ICS提供的输出是从内部基准时钟分频来的时钟，FLL使能但不用于ICS输出
#define ICS_WORKMODE_FBILP 4    //ICS提供的输出是从内部基准时钟分频来的时钟，FLL被禁用且不用于ICS输出
#define ICS_WORKMODE_FBE   5    //ICS提供的输出是从外部基准时钟分频来的时钟，FLL使能但不用于ICS输出
#define ICS_WORKMODE_FBELP 6    //ICS提供的输出是从外部基准时钟分频来的时钟，FLL被禁用且不用于ICS输出
#define ICS_WORKMODE_STOP  7    //FLL禁用，ICS不提供MCU时钟源
//使用外部晶振作为参考基准时钟源
#ifndef ICS_WORKMODE
  #define ICS_WORKMODE  2
#endif

//时钟频率定义，各个模块使用的时钟见P110
#ifndef EXREF_CLK_KHZ              //(定义外部参考时钟为16MHZ)
  #define EXREF_CLK_KHZ     8000u
#endif
#ifndef ICS_CLK_KHZ                //(定义ICS模块输出时钟为40MHZ)
  #define ICS_CLK_KHZ       40000u
#endif
#ifndef CORE_CLK_KHZ               //(定义内核时钟和平台NVIC时钟为40MHZ)系统时钟
  #define CORE_CLK_KHZ      40000u
#endif
#ifndef TIMER_CLK_KHZ              //(定义定时器FTM和PWT时钟为40MHZ)
  #define TIMER_CLK_KHZ     40000u
#endif
#ifndef BUS_CLK_KHZ                //(总线时钟和FLASH时钟为20MHZ)
  #define BUS_CLK_KHZ       20000u
#endif

extern uint32 ex_clk_khz;
extern uint32 ics_clk_khz;
extern uint32 timer_clk_khz;
extern uint32 core_clk_khz;
extern uint32 bus_clk_khz;

  
#ifndef CLOCK_SETUP
  #define CLOCK_SETUP   4
#endif


/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_CLK_HZ                  32768u   /* Value of the internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            20971520u /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_CLK_HZ                  32768u   /* Value of the internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            40000000u /* Default System clock value */
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_CLK_HZ                  32768u   /* Value of the internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            32768u   /* Default System clock value */
#elif (CLOCK_SETUP == 3)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_CLK_HZ                  32768u   /* Value of the internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            8000000u /* Default System clock value */
#elif (CLOCK_SETUP == 4)
    #define CPU_XTAL_CLK_HZ                 16000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_CLK_HZ                  32768u   /* Value of the internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            8000000u /* Default System clock value */
#endif /* (CLOCK_SETUP == 5) */


  
  
  
/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit (void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate (void);

/**
 * @brief Disable the wdog.
 *
 * SystemInit内部函数.用户无需调用
 */ 
void Disable_Wdog (void);

/**
 * @brief Clock Configuration
 *
 * SystemInit内部函数.用户无需调用
 */ 
void Clock_Config (void);

/**
 * @brief Clock Update & Clock Configure Check
 *
 * 时钟配置检查，在main开头调用
 */ 
void Get_Clk(void);

#ifdef __cplusplus
}
#endif

#endif  /* #if !defined(SYSTEM_SKEAZ1284_H_) */


void Enable_Interrupt(uint8 vector_number);
void Disable_Interrupt(uint8 vector_number);
