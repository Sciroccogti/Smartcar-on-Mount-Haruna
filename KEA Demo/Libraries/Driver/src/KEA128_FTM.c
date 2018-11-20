/*
  ******************************************************************************
  * @file    ftm.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.4.4
  * @mail:   blog_xhjiang@163.com
  * @brief   FTM configure
  ******************************************************************************
  */
#include "common.h"
#include "KEA128_FTM.h"

#define FTMn(ftmn)     ( (uint32_t)(((ftmn)-ftm0)/(ftm1-ftm0) ) )
static FTM_MemMapPtr const FTMX[] = FTM_BASE_PTRS;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      FTM管脚复用
//  @param      ftmn            选择模块
//  @param      ftm_chn         选择通道
//  @param      ptx_n           选择引脚号
//  @return     void
//  @since      v1.0
//  Sample usage:               FTM_Pinmux(ftm0,ftm_ch0,A0);		//ftm0模块，0通道，A0选择
//-------------------------------------------------------------------------------------------------------------------
void FTM_Pinmux(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n)
{
    switch((uint32_t)ftmn)
    {
        case (uint32_t)ftm0:
        {
            SIM_BASE_PTR->SCGC |= SIM_SCGC_FTM0_MASK;                //开启FTM外设时钟
            switch(ftm_chn)
            {
                case ftm_ch0:
                {
                    if(ptx_n == A0)
                    {
                        SIM_BASE_PTR->PINSEL &= ~SIM_PINSEL_FTM0PS0_MASK;
                    }
                    else if(ptx_n == B2)
                    {
                        SIM_BASE_PTR->PINSEL |= SIM_PINSEL_FTM0PS0_MASK;
                    }
                }break;
                
                case ftm_ch1:
                {
                    if(ptx_n == A1)
                    {
                        SIM_BASE_PTR->PINSEL &= ~SIM_PINSEL_FTM0PS1_MASK;
                    }
                    else if(ptx_n == B3)
                    {
                        SIM_BASE_PTR->PINSEL |= SIM_PINSEL_FTM0PS1_MASK;
                    }
                }break;
            }
        }break;
        
        case (uint32_t)ftm1:
        {
            SIM_BASE_PTR->SCGC |= SIM_SCGC_FTM1_MASK;                //开启FTM外设时钟
            switch(ftm_chn)
            {
                case ftm_ch0:
                {
                    if(ptx_n == C4)
                    {
                        SIM_BASE_PTR->PINSEL &= ~SIM_PINSEL_FTM1PS0_MASK;
                    }
                    else if(ptx_n == H2)
                    {
                        SIM_BASE_PTR->PINSEL |= SIM_PINSEL_FTM1PS0_MASK;
                    }
                }break;

                case ftm_ch1:
                {
                    if(ptx_n == C5)
                    {
                        SIM_BASE_PTR->PINSEL &= ~SIM_PINSEL_FTM1PS1_MASK;
                    }
                    else if(ptx_n == E7)
                    {
                        SIM_BASE_PTR->PINSEL |= SIM_PINSEL_FTM1PS1_MASK;
                    }
                }break;
            }
        }break;
        
        case (uint32_t)ftm2:
        {
            SIM_BASE_PTR->SCGC |= SIM_SCGC_FTM2_MASK;                //开启FTM外设时钟
            switch(ftm_chn)
            {
                case ftm_ch0:
                {
                    SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_FTM2PS0_MASK;       //先清空后再配置
                    if(ptx_n == C0)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS0(0);
                    }
                    else if(ptx_n == H0)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS0(1);
                    }
                    else if(ptx_n == F0)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS0(2);
                    }
                }break;
                
                case ftm_ch1:
                {
                    SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_FTM2PS1_MASK;       
                    if(ptx_n == C1)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS1(0);
                    }
                    else if(ptx_n == H1)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS1(1);
                    }
                    else if(ptx_n == F1)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS1(2);
                    }
                }break;
                
                case ftm_ch2:
                {
                    SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_FTM2PS2_MASK;       
                    if(ptx_n == C2)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS2(0);
                    }
                    else if(ptx_n == D0)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS2(1);
                    }
                    else if(ptx_n == G4)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS2(2);
                    }
                }break;
                
                case ftm_ch3:
                {
                    SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_FTM2PS3_MASK;       
                    if(ptx_n == C3)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS3(0);
                    }
                    else if(ptx_n == D1)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS3(1);
                    }
                    else if(ptx_n == G5)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS3(2);
                    }
                }break;
                
                case ftm_ch4:
                { 
                    if(ptx_n == B4)
                    {
                        SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_FTM2PS4_MASK; 
                    }
                    else if(ptx_n == G6)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS4_MASK;
                    }
                }break;
                
                case ftm_ch5:
                {
                    if(ptx_n == B5)
                    {
                        SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_FTM2PS5_MASK;     
                    }
                    else if(ptx_n == G7)
                    {
                        SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_FTM2PS5_MASK;
                    }
                }break;
            }
 
        }break;
    }
}


uint16 period[3];
//-------------------------------------------------------------------------------------------------------------------
//  @brief      FTM初始化
//  @param      ftmn            选择模块
//  @param      ftm_chn         选择通道
//  @param      ptx_n           选择引脚号
//  @param      freq            需要设置的频率（单位HZ）
//  @param      duty            需要设置的占空比
//  @return     void
//  @since      v1.0
//  Sample usage:               FTM_PWM_Init(ftm0,ftm_ch0,A0,50,500);		//ftm0模块，0通道，50HZ，百分之50的占空比
//-------------------------------------------------------------------------------------------------------------------
void FTM_PWM_Init(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n, uint32 freq, uint32 duty)
{
    uint32 clk_hz ;
    uint16 mod;
    uint8  ps;
    uint16 cv;
    
    FTM_Pinmux(ftmn,ftm_chn,ptx_n);
    
    
    /*       计算频率设置        */
    //  若 CPWMS = 1 ，即双边捕捉脉冲，则 PMW频率 =  bus频率 /2 /(2^预分频因子)/模数
    //  若 CPWMS = 0 ，即单边捕捉脉冲，则 PMW频率 =  bus频率    /(2^预分频因子)/模数
    //  EPWM的周期 ：MOD - CNTIN + 0x0001   (CNTIN 设为0)
    //  脉冲宽度：CnV - CNTIN

    //  模数 MOD < 0x10000
    //  预分频因子 PS  < 0x07
    //  预分频因子 PS 越小时，模数 mod 就越大，计数就越精准，PWM输出更为准确
    //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
    //  即 (((clk_hz/0x10000 )/ freq ) >> PS ) < 1

    
    // 以 CPWMS = 0 ，边沿对齐

    clk_hz = timer_clk_khz*1000;
    //clk_hz/(ps*mod) = freq        =>>     clk_hz/freq = (ps*mod)      =>>
    //clk_hz/freq < ((1<<n)*65536)  =>>    (clk_hz/freq)/65536 < (1>>n) =>> ((clk_hz/freq)/65536)>>n < 1
    mod = (clk_hz >> 16 ) / freq ;          // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)                 // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
        ps++;
    if(ps>0x07) return ;                    //超出设置范围，直接不设置跳出本函数，原因一般是由于PWM频率太低，或者总线频率太高导致
    
    mod = (clk_hz >> ps) / freq;            // 求 MOD 的值
    period[FTMn(ftmn)] = mod;
    switch((uint32_t)ftmn)                            // 初值 CNTIN 设为0 ，脉冲宽度：CnV - CNTIN ，即 CnV 就是 脉冲宽度了。
    {
        // EPWM的周期 ： MOD - CNTIN + 0x0001 == MOD - 0 + 1
        // 则 CnV = (MOD - 0 + 1) * 占空比 = (MOD - 0 + 1) * duty/ FTM_PRECISON
    case (uint32_t)ftm0:
        cv = (duty * (mod - 0 + 1)) / FTM0_PRECISON;
        break;

    case (uint32_t)ftm1:
        cv = (duty * (mod - 0 + 1)) / FTM1_PRECISON;
        break;

    case (uint32_t)ftm2:
        cv = (duty * (mod - 0 + 1)) / FTM2_PRECISON;
        break;
    default:
        break;
    }
    
    FTMX[ftmn]->MOD = mod;                  //设置PWM周期
    /******************** 选择输出模式为 边沿对齐PWM *******************/
    //通道状态控制，根据模式来选择 边沿或电平      
    FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~FTM_CnSC_ELSA_MASK;
    FTMX[ftmn]->CONTROLS[ftm_chn].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
//    FTM_CnSC_REG(ftmn, ftm_chn) &= ~FTM_CnSC_ELSA_MASK;
//    FTM_CnSC_REG(ftmn, ftm_chn)  = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    // MSnB:MSnA = 1x       边沿对齐PWM
    // ELSnB:ELSnA = 10     先高后低
    // ELSnB:ELSnA = 11     先低后高

    
    // set FTM clock to system clock 
    FTMX[FTMn(ftmn)]->SC = ( 0
                       //| FTM_SC_CPWMS_MASK       //0：边沿对齐 ，1： 中心对齐 （注释了表示 0）
                       | FTM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                       | FTM_SC_CLKS(1)            //时钟选择， 0：没选择时钟，禁用； 1：bus 时钟； 2：MCGFFCLK； 3：EXTCLK（ 由SIM_SOPT4 选择输入管脚 FTM_CLKINx）
                       //| FTM_SC_TOIE_MASK        //溢出中断使能（注释了表示 禁止溢出中断）
                     );

    
    if(ftmn == ftm2) FTMX[FTMn(ftmn)]->CNTIN = 0;         //设置计数器初值，一般没特殊用途就清零
    FTMX[FTMn(ftmn)]->CONTROLS[ftm_chn].CnV = cv;             //设置占空比
    FTMX[FTMn(ftmn)]->CNT = 0;                           //计数器，写任意值都会加载CNTIN的值
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      占空比设置
//  @param      ftmn            选择模块
//  @param      ch              选择通道
//  @param      duty            需要设置的占空比
//  @return     void
//  @since      v2.0
//  Sample usage:               FTM_PWM_Duty(ftm0,ftm_ch0,500);		//ftm0模块，0通道，百分之50的占空比
//-------------------------------------------------------------------------------------------------------------------
void FTM_PWM_Duty(FTMn ftmn, FTM_CHn ch, uint32 duty)
{
    uint32 cv;
    switch((uint32_t)ftmn)
    {
        //占空比大于100%时以100计
    case (uint32_t)ftm0:
        if(duty > FTM0_PRECISON)
            duty = FTM0_PRECISON;
        break;

    case (uint32_t)ftm1:
        if(duty > FTM2_PRECISON)
            duty = FTM1_PRECISON;
        break;
    case (uint32_t)ftm2:
        if(duty > FTM2_PRECISON)
            duty = FTM2_PRECISON;
        break;

    default:
        ASSERT(0);
        break;
    }
    //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)
    switch((uint32_t)ftmn)
    {
    case (uint32_t)ftm0:
        cv = (duty * (period[FTMn(ftmn)] - 0 + 1)) / FTM0_PRECISON;
        break;

    case (uint32_t)ftm1:
        cv = (duty * (period[FTMn(ftmn)] - 0 + 1)) / FTM1_PRECISON;
        break;

    case (uint32_t)ftm2:
        cv = (duty * (period[FTMn(ftmn)] - 0 + 1)) / FTM2_PRECISON;
        break;

    default:
        break;
    }
    
    FTMX[FTMn(ftmn)]->CONTROLS[ch].CnV = cv;      //设置占空比
}

/*!
 *  @brief      设置FTM 的PWM 通道占空比
 *  @param      FTMn    模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn     通道号（CH0~CH7）
 *  @param      duty    占空比分子，占空比 = duty %
 *  @since      v1.0
 *  @note       同一个FTM，PWM频率是必须一样的，但占空比可不一样。共3个FTM，即可以输出3个不同频率PWM
 *  Sample usage:       FTM_PWM_Duty_Float(FTM0, FTM_CH6, 10);    //设置 FTM0_CH6占空比为 10%
 */
void FTM_PWM_Duty_Float(FTM_MemMapPtr ftmn, FTM_CHn ch, float duty)
{
    uint32 cv;
    uint32 mod = 0;

    //防止出现占空比超过100%
    if(duty > 100.0)
    {
        duty = 100.0;
    }

    duty = duty/100.0;
    //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1
    switch((uint32_t)ftmn)
    {
    case (uint32_t)ftm0:
        mod = period[0];
        cv = (uint32_t)(duty * (mod - 0 + 1)) ;
        break;

    case (uint32_t)ftm1:
        mod = period[1];
        cv = (uint32_t)(duty * (mod - 0 + 1)) ;
        break;

    case (uint32_t)ftm2:
        mod = period[2];
        cv = (uint32_t)(duty * (mod - 0 + 1)) ;
        break;

    default:
        ASSERT(0);
        break;
    }

    // 配置FTM通道值
    FTM_CnV_REG(ftmn, ch) = cv;

    //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      频率设置
//  @param      ftmn            选择模块
//  @param      freq            需要设置的频率（单位HZ）
//  @return     void
//  @note       修改PWM频率后，必须调用 FTM_PWM_Duty 重新配置占空比。同一个模块，PWM频率必须相同。
//  @since      v1.0
//  Sample usage:               FTM_PWM_Freq(ftm0,50);		//ftm0模块，50HZ
//-------------------------------------------------------------------------------------------------------------------
void FTM_PWM_Freq(FTMn ftmn, uint32 freq)
{
    uint32 clk_hz;        //bus频率/2
    uint32 mod;
    uint8 ps;

    // 以 CPWMS = 0 ，边沿对齐
    clk_hz = timer_clk_khz * 1000 ;
    
    //clk_hz/(ps*mod) = freq        =>>     clk_hz/freq = (ps*mod)      =>>
    //clk_hz/freq < ((1<<n)*65536)  =>>    (clk_hz/freq)/65536 < (1>>n) =>> ((clk_hz/freq)/65536)>>n < 1
    mod = (clk_hz >> 16 ) / freq ;          // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)                 // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
        ps++;
    if(ps>0x07) return ;                    //超出设置范围，直接不设置跳出本函数，原因一般是由于PWM频率太低，或者总线频率太高导致
    
    mod = (clk_hz >> ps) / freq;            // 求 MOD 的值
    period[FTMn(ftmn)] = mod;
    
    
    FTMX[FTMn(ftmn)]->MOD = mod;                  //设置PWM周期
    
    // set FTM clock to system clock 
    FTMX[FTMn(ftmn)]->SC = ( 0
                       //| FTM_SC_CPWMS_MASK       //0：边沿对齐 ，1： 中心对齐 （注释了表示 0）
                       | FTM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                       | FTM_SC_CLKS(1)            //时钟选择， 0：没选择时钟，禁用； 1：bus 时钟； 2：MCGFFCLK； 3：EXTCLK（ 由SIM_SOPT4 选择输入管脚 FTM_CLKINx）
                       //| FTM_SC_TOIE_MASK        //溢出中断使能（注释了表示 禁止溢出中断）
                     );

    
    if(ftmn == ftm2) FTMX[FTMn(ftmn)]->CNTIN = 0;  //设置计数器初值，一般没特殊用途就清零
    FTMX[FTMn(ftmn)]->CNT = 0;                    //计数器，写任意值都会加载CNTIN的值
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      外部计数初始化
//  @param      ftmn            选择模块
//  @return     void
//  @since      v2.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void FTM_Count_Init(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n, FTM_Input_cfg cfg)
{
    ASSERT(  (ftmn == ftm2) );  //检查传递进来的通道是否正确 (寄存器配置，貌似只有FTM2才能做输入捕捉的功能)

    FTM_Pinmux(ftmn,ftm_chn,ptx_n);                                    //引脚复用 开启上拉 开启对应外设时钟

    switch(cfg)
    {
        //输入捕捉模式下：DECAPEN = 0 ， DECAPEN = 0 ，CPWMS = 0， MSnB:MSnA = 0

        // ELSnB:ELSnA         1          10          11
        // 配置             上升沿      下降沿      跳变沿

    case FTM_Rising:    //上升沿触发
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC |=  ( FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK );                   //置1
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~( FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case FTM_Falling:   //下降沿触发
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_CHIE_MASK );                    //置1
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~( FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case FTM_Rising_or_Falling: //上升沿、下降沿都触发
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC |=  ( FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK ); //置1
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~( FTM_CnSC_MSB_MASK  | FTM_CnSC_MSA_MASK); //清0
        break;
    }

    FTMX[FTMn(ftmn)]->SC = 0;
    FTMX[FTMn(ftmn)]->MODE  =    (0
                              | FTM_MODE_WPDIS_MASK  //写保护禁止
                              //| FTM_MODE_FTMEN_MASK   //使能 FTM
                             );


    FTMX[FTMn(ftmn)]->COMBINE = 0;
    //FTM_MODE_REG(FTMN[ftmn])    &= ~FTM_MODE_FTMEN_MASK;    //使能FTM
    FTMX[FTMn(ftmn)]->CNTIN = 0;

    FTMX[FTMn(ftmn)]->STATUS  = 0x00;               //清中断标志位

    FTMX[FTMn(ftmn)]->MOD  = FTM_MOD_MOD_MASK;
    FTMX[FTMn(ftmn)]->SC |= FTM_SC_PS(0);	                        //分频系数	
    FTMX[FTMn(ftmn)]->SC |= FTM_SC_CLKS(1);                       //选择BUS时钟作为FTM输入时钟
    FTMX[FTMn(ftmn)]->MODE |=    (0
                               //| FTM_MODE_WPDIS_MASK  //写保护禁止
                               | FTM_MODE_FTMEN_MASK   //使能 FTM
                              );            
    FTMX[FTMn(ftmn)]->CNT = 0;                                    //加载初始化值
    //开启输入捕捉中断
    //enable_irq(FTM0_IRQn + ftmn);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取计数值
//  @param      ftmn            选择模块
//  @return     uint16          返回计数值
//  @since      v2.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
uint16 FTM_Count_Get(FTMn ftmn, FTM_CHn ftm_chn)
{
  return (uint16)FTMX[ftmn]->CONTROLS[ftm_chn].CnV;   //保存
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      清除计数值
//  @param      ftmn            选择模块
//  @since      v2.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void FTM_Count_Clean(FTMn ftmn)
{
    FTMX[FTMn(ftmn)]->CNT = 0;
}

//////////////////////////////// 以上为输入捕捉  //////////////////////////////////////////
//使能对应模块的时钟
static inline void FTM_Enable_CLK(FTMn ftmn)
{
    /******************* 开启时钟*******************/
    BIT_SET(SIM_SCGC, (SIM_SCGC_FTM0_SHIFT + FTMn(ftmn)));
}//对于FTM0等价于SIM->SCGC |= SIM_SCC_FTM0_MASK;
//////////////////////////////// 以下为外部引脚脉冲计数 //////////////////////////////////////////
//脉冲频率较高时，可用分频功能
//如果是欧姆龙那种开漏输出的，需要配置上拉：port_pull(FTM0_PULSE_PIN,PULLUP_ENBLE);
void FTM_Pulse_Pinmux(FTMn ftmn, PTX_n ptx_n)
{
    int n = FTMn(ftmn);

    FTM_Enable_CLK(ftmn);//开启FTM外设时钟
    SIM_BASE_PTR->PINSEL &= ~(SIM_PINSEL_FTM0CLKPS_MASK << (n * 2) );//清除外部时钟引脚选择
    if(n==2) FTM2->CNTIN = 0;
    switch(ptx_n)
    {
    case  TCLK0:
        SIM_BASE_PTR->PINSEL |= (SIM_PINSEL_FTM0CLKPS(0) << (n * 2) );
        break;
    case  TCLK1:
        SIM_BASE_PTR->PINSEL |= (SIM_PINSEL_FTM0CLKPS(1) << (n * 2) );
        break;
    case  TCLK2:
        SIM_BASE_PTR->PINSEL |= (SIM_PINSEL_FTM0CLKPS(2) << (n * 2) );
        break;
    default:
        break;

    }
}

void FTM_Pulse_Init(FTMn ftmn, FTM_PS_e ps, PTX_n ptx_n)
{

    FTM_Pulse_Pinmux(ftmn, ptx_n);


    FTMX[FTMn(ftmn)]->CNT = 0;
    FTMX[FTMn(ftmn)]->SC &=  ~(FTM_SC_PS_MASK | FTM_SC_CLKS_MASK);
    FTMX[FTMn(ftmn)]->SC |=   ( 0
                    | FTM_SC_PS(ps)
                    | FTM_SC_CLKS(3)      //外部时钟
                  );



}

uint16_t FTM_Pulse_Get(FTMn ftmn)
{
    return FTMX[FTMn(ftmn)]->CNT ;
}
