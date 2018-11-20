/*
  ******************************************************************************
  * @file    KEA128_ADC.h
  * @author  xhJiang
  * @version v1.1
  * @date    2018.4.2
  * @mail:   blog_xhjiang@163.com
  * @brief   ADC configure
  ******************************************************************************
  */


#ifndef KEA128_ADC_H_
#define KEA128_ADC_H_

#include "common.h"

//ADC端口枚举，KEA的ADC无多路复用
typedef enum
{
    ADC0_SE0=0,  //A0,00000
    ADC0_SE1,    //A1,00001
    ADC0_SE2,    //A6,00010
    ADC0_SE3,    //A7,00011
    ADC0_SE4,    //B0,00100
    ADC0_SE5,    //B1,00101
    ADC0_SE6,    //B2,00110
    ADC0_SE7,    //B3,00111
    ADC0_SE8,    //C0,01000
    ADC0_SE9,    //C1,01001
    ADC0_SE10,   //C2,01010
    ADC0_SE11,   //C3,01011
    ADC0_SE12,   //F4,01100
    ADC0_SE13,   //F5,01101
    ADC0_SE14,   //F6,01110
    ADC0_SE15,   //F7,01111
    VSS,         //10000-10011均为Vss
                 //RESERVED
    Temp_Sensor = B8(10110),   //温度传感器
    Bandgap = B8(10111),       //带隙基准
                 //RESERVED
    VREFH = B8(11101),        // 参考高电压
    VREFL = B8(11110),        // 参考低电压
    ADC0_DISABLE = B8(11111),
    ADCn_CHn_MAX //RESERVED
} ADCn_Chn;

////ADC模块RESERVED，KEA仅ADC0
//typedef enum  
//{
//    ADC0,
//    ADC1
//} ADCn;

//ADC转换精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x01,
    ADC_12bit  = 0x02
} ADC_nbit;


extern void ADC_Init(ADCn_Chn adcn_chn, ADC_nbit bit);
extern uint16 ADC_Read(ADCn_Chn adcn_chn);
extern void ADC_Stop(void);
extern uint8 ADC_Isacting(void);



#endif
