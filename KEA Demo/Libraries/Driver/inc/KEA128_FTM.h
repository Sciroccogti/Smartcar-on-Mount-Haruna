/*
  ******************************************************************************
  * @file    ftm.h
  * @author  xhJiang
  * @version V1.0
  * @date    2018.4.4
  * @mail:   blog_xhjiang@163.com
  * @brief   Ftm configure
  ******************************************************************************
  */

#ifndef KEA128_FTM_H_
#define KEA128_FTM_H_

#include "common.h"


#define TCLK0           PTA5              // (A5 是复位引脚，不可用)
#define TCLK1           PTE0
#define TCLK2           PTE7

#define FTM0_PULSE_PIN   PTE0        //从 TCLKn里任意选其中一个
#define FTM1_PULSE_PIN   PTE7        //从 TCLKn里任意选其中一个
#define FTM2_PULSE_PIN   PTA5        //从 TCLKn里任意选其中一个
//分频系数
typedef enum
{
    FTM_PS_1,
    FTM_PS_2,
    FTM_PS_4,
    FTM_PS_8,
    FTM_PS_16,
    FTM_PS_32,
    FTM_PS_64,
    FTM_PS_128,

    FTM_PS_MAX,
} FTM_PS_e;     //分频值 =  (1<< FTM_PS_e) ,例如  FTM_PS_2  对应的 分频值 = (1<<FTM_PS_2) = (1<<1) = 2




typedef enum
{
    ftm0,
    ftm1,
    ftm2,
} FTMn;


typedef enum
{
    ftm_ch0,
    ftm_ch1,
    ftm_ch2,
    ftm_ch3,
    ftm_ch4,
    ftm_ch5,
    ftm_ch6,
    ftm_ch7,
} FTM_CHn;

//FTM 输入捕捉配置
typedef enum
{
    FTM_Rising,               //上升沿捕捉
    FTM_Falling,              //下降沿捕捉
    FTM_Rising_or_Falling     //跳变沿捕捉
} FTM_Input_cfg;
//------------------------ PWM ------------------------
//精度，就是算周期的分母，只要这个数不要大到 mod 都被稀释掉就可以了
//定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON
#define FTM0_PRECISON 1000u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON
#define FTM1_PRECISON 1000u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON
#define FTM2_PRECISON 1000u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON

extern void FTM_Pinmux(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n);

extern void FTM_PWM_Init(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n, uint32 freq, uint32 duty);
extern void FTM_PWM_Duty(FTMn ftmn, FTM_CHn ch, uint32 duty);
extern void FTM_PWM_Freq(FTMn ftmn, uint32 freq) ;
extern void FTM_Count_Init(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n, FTM_Input_cfg cfg);
extern uint16 FTM_Count_Get(FTMn ftmn, FTM_CHn ftm_chn);
extern void FTM_Count_Clean(FTMn ftmn);
extern void FTM_Pulse_Init(FTMn ftmn, FTM_PS_e ps, PTX_n ptx_n);
extern uint16_t FTM_Pulse_Get(FTMn ftmn);



#endif
