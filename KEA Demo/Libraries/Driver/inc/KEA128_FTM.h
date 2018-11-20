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


#define TCLK0           PTA5              // (A5 �Ǹ�λ���ţ�������)
#define TCLK1           PTE0
#define TCLK2           PTE7

#define FTM0_PULSE_PIN   PTE0        //�� TCLKn������ѡ����һ��
#define FTM1_PULSE_PIN   PTE7        //�� TCLKn������ѡ����һ��
#define FTM2_PULSE_PIN   PTA5        //�� TCLKn������ѡ����һ��
//��Ƶϵ��
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
} FTM_PS_e;     //��Ƶֵ =  (1<< FTM_PS_e) ,����  FTM_PS_2  ��Ӧ�� ��Ƶֵ = (1<<FTM_PS_2) = (1<<1) = 2




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

//FTM ���벶׽����
typedef enum
{
    FTM_Rising,               //�����ز�׽
    FTM_Falling,              //�½��ز�׽
    FTM_Rising_or_Falling     //�����ز�׽
} FTM_Input_cfg;
//------------------------ PWM ------------------------
//���ȣ����������ڵķ�ĸ��ֻҪ�������Ҫ�� mod ����ϡ�͵��Ϳ�����
//����ռ�ձȾ��ȣ�100������Ϊ1%��1000u�򾫶�Ϊ0.1%������ռ�ձ� duty �βδ��룬��ռ�ձ�Ϊ duty/FTM_PRECISON
#define FTM0_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM1_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM2_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON

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
