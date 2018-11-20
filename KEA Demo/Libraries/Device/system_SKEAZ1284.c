/*
** ###################################################################
**
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
**
**     Version:             rev. 1.0, 2018-03-20
**     Build:               xhJiang
**     Mail:                blog_xhjiang@163.com
**
**     Revisions:
**     - rev. 1.0 (2013-07-30)
**         Initial version.
**
**
** ###################################################################
*/

/*!
 * @file system_SKEAZ1284
 * @version 1.0
 * @date 2018-03-20
 * @brief Device specific configuration file for SKEAZ1284 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "SKEAZ1284.h"
#include "system_SKEAZ1284.h"
#include "common.h"

/* ----------------------------------------------------------------------------
   -- Core clock  时钟频率预定义
   ---------------------------------------------------------------------------- */
  
uint32 ex_clk_khz = 8000;
uint32 ics_clk_khz = 40000;
uint32 core_clk_khz = 40000;
uint32 timer_clk_khz = 40000;
uint32 bus_clk_khz = 20000;


uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

//中断向量表
extern uint32_t __Vectors[];
/* ----------------------------------------------------------------------------
 * @brief  Keil的系统初始化配置函数，IAR工程使用low_level_init
 * ---------------------------------------------------------------------------- */

void SystemInit (void) 
{
	SCB_VTOR = (uint32_t)__Vectors; /* Set the interrupt vector table position */
	ics_clk_khz = 0;//清空等待get clk赋值
  Disable_Wdog();
  Clock_Config();
	Get_Clk();                           /* Get the clock */
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {

 uint32_t ICSOUTClock;                                                        /* Variable to store output clock frequency of the ICS module */
 uint8_t Divider;

 if ((ICS->C1 & ICS_C1_CLKS_MASK) == 0x0u) {
   /* Output of FLL is selected */
   if ((ICS->C1 & ICS_C1_IREFS_MASK) == 0x0u) {
     /* External reference clock is selected */
     ICSOUTClock = CPU_XTAL_CLK_HZ;                                         /* System oscillator drives ICS clock */
     Divider = (uint8_t)(1u << ((ICS->C1 & ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT));
     ICSOUTClock = (ICSOUTClock / Divider);  /* Calculate the divided FLL reference clock */
     if ((OSC->CR & OSC_CR_RANGE_MASK) != 0x0u) {
       ICSOUTClock /= 32u;                                                  /* If high range is enabled, additional 32 divider is active */
     }
   } else {
     ICSOUTClock = CPU_INT_CLK_HZ;                                          /* The internal reference clock is selected */
   }
   ICSOUTClock *= 1280u;                                                    /* Apply 1280 FLL multiplier */
 } else if ((ICS->C1 & ICS_C1_CLKS_MASK) == 0x40u) {
   /* Internal reference clock is selected */
   ICSOUTClock = CPU_INT_CLK_HZ;
 } else if ((ICS->C1 & ICS_C1_CLKS_MASK) == 0x80u) {
   /* External reference clock is selected */
   ICSOUTClock = CPU_XTAL_CLK_HZ;
 } else {
   /* Reserved value */
   return;
 }
 ICSOUTClock = ICSOUTClock >> ((ICS->C2 & ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT);
 SystemCoreClock = (ICSOUTClock / (1u + ((SIM->CLKDIV & SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV1_SHIFT)));

}

/* ----------------------------------------------------------------------------
 * @brief  禁用看门狗
 * ---------------------------------------------------------------------------- */

void Disable_Wdog (void)
{
  #if (DISABLE_WDOG)
    /* WDOG->TOVAL: TOVAL=0xE803 */
    WDOG_BASE_PTR->TOVAL = WDOG_TOVAL_TOVAL(0xE803); /*Set Timeout value = 1000(03E8转置16位字节)*/
    /* WDOG->CS2: WIN=0,FLG=0,??=0,PRES=0,??=0,??=0,CLK=1 */
    WDOG_BASE_PTR->CS2 = WDOG_CS2_CLK(0x01);       /* 1kHz clock source */
    /* WDOG->CS1: EN=0,INT=0,UPDATE=1,TST=0,DBG=0,WAIT=1,STOP=1 除配置位外其余位均为0，EN位为0禁用WDOG*/
    WDOG_BASE_PTR->CS1 = WDOG_CS1_UPDATE_MASK |    //Enable update允许更新
               WDOG_CS1_TST(0x00) |       //Disable test禁用测试模式
               WDOG_CS1_WAIT_MASK |       //Enable Wait使能WDOG在Wait模式下
               WDOG_CS1_STOP_MASK;        //Enable Stop使能WDOG在Stop模式下

  #endif /* (DISABLE_WDOG) */
}

/* ----------------------------------------------------------------------------
 * @brief  时钟配置
 * ---------------------------------------------------------------------------- */
void Clock_Config (void)
{
  #if (ICS_WORKMODE == 1)
//                                             Internal Clock Source (ICS) in FLL Engaged Internal (FEI) mode
//                                             Default  part configuration.
//                                             Reference clock source for ICS module is the slow internal clock source 32.768kHz
//                                             Core clock = 20.97MHz, BusClock = 20.97MHz
    /* ICS->C2: BDIV|=1 */
    ICS->C2 |= ICS_C2_BDIV(0x01);         /* Update system prescalers FLL输出2分频*/
    /* SIM->CLKDIV: ??=0,??=0,OUTDIV1=0,??=0,??=0,??=0,OUTDIV2=0,??=0,??=0,??=0,OUTDIV3=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
    SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00); /* Update system prescalers */
    /* Switch to FEI Mode */
    /* ICS_C1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 使用内部参考基准时钟32.768kHz，由于Range位为0，FLL输出分频取低档不分频(保证频率在31.25kHz-39.0625kHz之间)，故FLL输出频率为32.768*1280*/
              ICS->C1 = ICS_C1_CLKS(0x00) |
              ICS_C1_RDIV(0x00) |
              ICS_C1_IREFS_MASK |
              ICS_C1_IRCLKEN_MASK;
    /* ICS->C2: BDIV=1,LP=0 FLL输出后2分频作为系统总线时钟*/
              ICS->C2 = (uint8_t)((ICS->C2 & (uint8_t)~(uint8_t)(
                ICS_C2_BDIV(0x06) |
                ICS_C2_LP_MASK
                )) | (uint8_t)(
              ICS_C2_BDIV(0x01)
              ));
    /* OSC->CR: OSCEN=0,??=0,OSCSTEN=0,OSCOS=0,??=0,RANGE=0,HGO=0,OSCINIT=0 */
              OSC->CR = 0x00U;
    while((ICS->S & ICS_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
            }
    while((ICS->S & 0x0CU) != 0x00U) {    /* Wait until output of the FLL is selected */
          }
  #elif (ICS_WORKMODE == 2)
//         Internal Clock Source (ICS) in FLL Engaged External (FEE) mode
//         Maximum achievable clock frequency configuration.
//         Reference clock source for ICS module is an external 16MHz crystal
//         Core clock = 40MHz, BusClock = 40MHz
    //更新时钟分频值
    #if ((CORE_CLK_KHZ == ICS_CLK_KHZ) && (BUS_CLK_KHZ == CORE_CLK_KHZ/2) && (TIMER_CLK_KHZ == ICS_CLK_KHZ))
      /* SIM->CLKDIV: ??=0,??=0,OUTDIV1=0,??=0,??=0,??=0,OUTDIV2=1,??=0,??=0,??=0,OUTDIV3=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
      SIM_BASE_PTR->CLKDIV = ( SIM_CLKDIV_OUTDIV1(0x00)  // 将ICSOUTCLK作为系统时钟
                     |SIM_CLKDIV_OUTDIV2_MASK); // 将系统时钟的1/2作为总线时钟
                                                // 将ICSOUTCLK作为定时器时钟
    #elif ((CORE_CLK_KHZ == ICS_CLK_KHZ) && (BUS_CLK_KHZ == CORE_CLK_KHZ/2) && (TIMER_CLK_KHZ == ICS_CLK_KHZ/2))
      /* SIM->CLKDIV: ??=0,??=0,OUTDIV1=0,??=0,??=0,??=0,OUTDIV2=1,??=0,??=0,??=0,OUTDIV3=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
      SIM_BASE_PTR->CLKDIV = ( SIM_CLKDIV_OUTDIV1(0x00)  // 将ICSOUTCLK作为系统时钟
                     |SIM_CLKDIV_OUTDIV2_MASK   // 将系统时钟的1/2作为总线时钟
                     |SIM_CLKDIV_OUTDIV3_MASK); // 将ICSOUTCLK的1/2作为定时器时钟
                                                /* Update system prescalers */
    #endif
      /* Switch to FEE Mode */
      /* ICS->C2: BDIV=0,LP=0 */
      ICS_BASE_PTR->C2 &= (uint8_t)~(uint8_t)((ICS_C2_BDIV(0x07) | ICS_C2_LP_MASK));//等效于ICS->C2 = ICS_C2_BDIV(0x00);
      /* OSC->CR: OSCEN=1,??=0,OSCSTEN=0,OSCOS=1,??=0,RANGE=1,HGO=1,OSCINIT=0 */
      OSC_BASE_PTR->CR = (OSC_CR_OSCEN_MASK |            //OSC使能
                 OSC_CR_OSCOS_MASK |            //OSC输出选择为外部晶振（0为外部时钟，1为振荡器）
                 OSC_CR_RANGE_MASK |            //频率范围选择（0为32kHz低频，1为4-24MHz高频范围）
                 OSC_CR_HGO_MASK);              //高增益振荡器选择  (0 低功耗模式,1 高增益模式)
      #if (EXREF_CLK_KHZ == 16000u)
        /* ICS->C1: CLKS=0,RDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 IREF选择外部基准时钟，CLKS选择输出时钟为FLL，1280倍频后，Range位高且RDIV位为4，分频512*/
      ICS_BASE_PTR->C1 = (ICS_C1_CLKS(0x00) | ICS_C1_RDIV(0x04) | ICS_C1_IRCLKEN_MASK);
      #elif (EXREF_CLK_KHZ == 8000u)
        /* ICS->C1: CLKS=0,RDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 1280倍频后，Range位高且RDIV位为3，分频256*/
      ICS_BASE_PTR->C1 = (ICS_C1_CLKS(0x00) | ICS_C1_RDIV(0x03) | ICS_C1_IRCLKEN_MASK);
      #else
      #error "时钟频率配置失败"
      #endif
      while((ICS_BASE_PTR->S & ICS_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock.等待时钟选择外部时钟(时钟域同步) */
    }
      //while((ICS->S & ICS_S_LOCK_MASK) == 0x00U){};       //等待FLL锁定
      while((ICS_BASE_PTR->S & 0x0CU) != 0x00U) {    /* Wait until output of the FLL is selected 等待时钟模式状态选择 FLL 输出(时钟域同步)*/
  }
  #elif (ICS_WORKMODE == 4)
//         Internal Clock Source (ICS) in Bypassed Low Power Internal (FBILP) mode
//         Core clock/Bus clock derived directly from an  internal clock 32.769kHz with no multiplication
//         The clock settings is ready for Very Low Power Run mode.
//         Core clock = 32.769kHz, BusClock = 32.769kHz
    /* SIM->CLKDIV: ??=0,??=0,OUTDIV1=0,??=0,??=0,??=0,OUTDIV2=0,??=0,??=0,??=0,OUTDIV3=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
    SIM_BASE_PTR->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00); /* Update system prescalers */
    /* Switch to FBI Mode */
    /* ICS->C1: CLKS=1,RDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
  ICS_BASE_PTR->C1 = ICS_C1_CLKS(0x01) |
  ICS_C1_RDIV(0x00) |
  ICS_C1_IREFS_MASK |
  ICS_C1_IRCLKEN_MASK;
    /* ICS->C2: BDIV=0,LP=0 */
  ICS_BASE_PTR->C2 &= (uint8_t)~(uint8_t)((ICS_C2_BDIV(0x07) | ICS_C2_LP_MASK));
    /* OSC->CR: OSCEN=0,??=0,OSCSTEN=0,OSCOS=0,??=0,RANGE=0,HGO=0,OSCINIT=0 */
  OSC_BASE_PTR->CR = 0x00U;
    while((ICS_BASE_PTR->S & ICS_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
}
    while((ICS_BASE_PTR->S & 0x0CU) != 0x04U) {    /* Wait until internal reference clock is selected as ICS output */
}
    /* Switch to BLPI Mode */
    /* ICS->C2: BDIV=0,LP=1 */
ICS_BASE_PTR->C2 = (uint8_t)((ICS->C2 & (uint8_t)~(uint8_t)(
  ICS_C2_BDIV(0x07)
  )) | (uint8_t)(
ICS_C2_LP_MASK
));
    while((ICS->S & ICS_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
}
  #elif (ICS_WORKMODE == 6)
//         Internal Clock Source (ICS) in Bypassed Low Power External (BLPE) mode
//         Core clock/Bus clock derived directly from the external 8MHz crystal
//         The clock settings is ready for Very Low Power Run mode.
//         Core clock = 8MHz, BusClock = 8MHz
    /* SIM->CLKDIV: ??=0,??=0,OUTDIV1=0,??=0,??=0,??=0,OUTDIV2=0,??=0,??=0,??=0,OUTDIV3=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
    SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00); /* Update system prescalers */
    /* Switch to FBE Mode */
    /* ICS->C2: BDIV=0,LP=0 */
ICS->C2 &= (uint8_t)~(uint8_t)((ICS_C2_BDIV(0x07) | ICS_C2_LP_MASK));
    /* OSC->CR: OSCEN=1,??=0,OSCSTEN=0,OSCOS=1,??=0,RANGE=1,HGO=0,OSCINIT=0 */
OSC->CR = (OSC_CR_OSCEN_MASK | OSC_CR_OSCOS_MASK | OSC_CR_RANGE_MASK);
    /* ICS->C1: CLKS=2,RDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
ICS->C1 = (ICS_C1_CLKS(0x02) | ICS_C1_RDIV(0x03) | ICS_C1_IRCLKEN_MASK);
    while((ICS->S & ICS_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
}
    while((ICS->S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as ICS output */
}
    /* Switch to BLPE Mode */
    /* ICS->C2: BDIV=0,LP=1 */
ICS->C2 = (uint8_t)((ICS->C2 & (uint8_t)~(uint8_t)(
  ICS_C2_BDIV(0x07)
  )) | (uint8_t)(
ICS_C2_LP_MASK
));
    while((ICS->S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as ICS output */
}
  #endif


}

/* ----------------------------------------------------------------------------
 * @brief  时钟配置检查
 * ---------------------------------------------------------------------------- */
void Get_Clk(void)
{
  uint16 prescaler_factor[]={1,2,4,8,16,32,64,128,256,512,1024};
  
  ics_clk_khz  = ex_clk_khz * 1280 ;
    ics_clk_khz = ics_clk_khz / prescaler_factor[((ICS_BASE_PTR->C1&ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT) + 5];    //计算FLL输出频率
    ics_clk_khz = ics_clk_khz / prescaler_factor[(ICS_BASE_PTR->C2&ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT];          //计算ICSCLKOUT
    
    core_clk_khz = ics_clk_khz  / (((SIM_BASE_PTR->CLKDIV&SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV1_SHIFT) + 1);
    bus_clk_khz  = core_clk_khz / (((SIM_BASE_PTR->CLKDIV&SIM_CLKDIV_OUTDIV2_MASK) >> SIM_CLKDIV_OUTDIV2_SHIFT) + 1);
    timer_clk_khz  = ics_clk_khz  / (((SIM_BASE_PTR->CLKDIV&SIM_CLKDIV_OUTDIV3_MASK) >> SIM_CLKDIV_OUTDIV3_SHIFT) + 1);
    
    //uart_init(DEBUG_PORT,DEBUG_BAUD);   //初始化调试串口，如果不使用printf可以屏蔽
  }

/***********************************************************************************************
*
* @brief    Enable_Interrupt(UINT8 vector_number). Enable interrupts from desired module.
* @param    Module interrupt number from the interrupts vector table//Same to NVIC_EnableIRQ
* @return   none
*
************************************************************************************************/
void Enable_Interrupt(uint8 vector_number)
{

  vector_number= vector_number-16;


  /* Set the ICPR and ISER registers accordingly */
  NVIC_ICPR |= 1 << (vector_number%32);//clear pending
  NVIC_ISER |= 1 << (vector_number%32);//enable interrupt

}
/***********************************************************************************************
*
* @brief    Disable_Interrupt(UINT8 vector_number). A disabled interrupt sets the state of that interrupt to pending.
* @param    Module interrupt number from the interrupts vector table//Same to NVIC_DisableIRQ
* @return   none
*
************************************************************************************************/
void Disable_Interrupt(uint8 vector_number)
{

  vector_number= vector_number-16;

  NVIC_ICER |= 1 << (vector_number%32);//disable

}
/***********************************************************************************************
*
* @brief    Pending_Interrupt(UINT8 vector_number). An interrupt that is pending has no effect
* @param    Module interrupt number from the interrupts vector table//Same to NVIC_SetPendingIRQ
* @return   none
*
************************************************************************************************/
void Pending_Interrupt(uint8 vector_number)
{

  vector_number= vector_number-16;

  NVIC_ISPR |= 1 << (vector_number%32);//set pending

}
	