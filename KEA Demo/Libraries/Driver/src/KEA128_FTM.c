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
//  @brief      FTM�ܽŸ���
//  @param      ftmn            ѡ��ģ��
//  @param      ftm_chn         ѡ��ͨ��
//  @param      ptx_n           ѡ�����ź�
//  @return     void
//  @since      v1.0
//  Sample usage:               FTM_Pinmux(ftm0,ftm_ch0,A0);		//ftm0ģ�飬0ͨ����A0ѡ��
//-------------------------------------------------------------------------------------------------------------------
void FTM_Pinmux(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n)
{
    switch((uint32_t)ftmn)
    {
        case (uint32_t)ftm0:
        {
            SIM_BASE_PTR->SCGC |= SIM_SCGC_FTM0_MASK;                //����FTM����ʱ��
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
            SIM_BASE_PTR->SCGC |= SIM_SCGC_FTM1_MASK;                //����FTM����ʱ��
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
            SIM_BASE_PTR->SCGC |= SIM_SCGC_FTM2_MASK;                //����FTM����ʱ��
            switch(ftm_chn)
            {
                case ftm_ch0:
                {
                    SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_FTM2PS0_MASK;       //����պ�������
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
//  @brief      FTM��ʼ��
//  @param      ftmn            ѡ��ģ��
//  @param      ftm_chn         ѡ��ͨ��
//  @param      ptx_n           ѡ�����ź�
//  @param      freq            ��Ҫ���õ�Ƶ�ʣ���λHZ��
//  @param      duty            ��Ҫ���õ�ռ�ձ�
//  @return     void
//  @since      v1.0
//  Sample usage:               FTM_PWM_Init(ftm0,ftm_ch0,A0,50,500);		//ftm0ģ�飬0ͨ����50HZ���ٷ�֮50��ռ�ձ�
//-------------------------------------------------------------------------------------------------------------------
void FTM_PWM_Init(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n, uint32 freq, uint32 duty)
{
    uint32 clk_hz ;
    uint16 mod;
    uint8  ps;
    uint16 cv;
    
    FTM_Pinmux(ftmn,ftm_chn,ptx_n);
    
    
    /*       ����Ƶ������        */
    //  �� CPWMS = 1 ����˫�߲�׽���壬�� PMWƵ�� =  busƵ�� /2 /(2^Ԥ��Ƶ����)/ģ��
    //  �� CPWMS = 0 �������߲�׽���壬�� PMWƵ�� =  busƵ��    /(2^Ԥ��Ƶ����)/ģ��
    //  EPWM������ ��MOD - CNTIN + 0x0001   (CNTIN ��Ϊ0)
    //  �����ȣ�CnV - CNTIN

    //  ģ�� MOD < 0x10000
    //  Ԥ��Ƶ���� PS  < 0x07
    //  Ԥ��Ƶ���� PS ԽСʱ��ģ�� mod ��Խ�󣬼�����Խ��׼��PWM�����Ϊ׼ȷ
    //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
    //  �� (((clk_hz/0x10000 )/ freq ) >> PS ) < 1

    
    // �� CPWMS = 0 �����ض���

    clk_hz = timer_clk_khz*1000;
    //clk_hz/(ps*mod) = freq        =>>     clk_hz/freq = (ps*mod)      =>>
    //clk_hz/freq < ((1<<n)*65536)  =>>    (clk_hz/freq)/65536 < (1>>n) =>> ((clk_hz/freq)/65536)>>n < 1
    mod = (clk_hz >> 16 ) / freq ;          // ��ʱ�� mod ����һ��
    ps = 0;
    while((mod >> ps) >= 1)                 // �� (mod >> ps) < 1 ���˳� while ѭ�� ������ PS ����Сֵ
        ps++;
    if(ps>0x07) return ;                    //�������÷�Χ��ֱ�Ӳ�����������������ԭ��һ��������PWMƵ��̫�ͣ���������Ƶ��̫�ߵ���
    
    mod = (clk_hz >> ps) / freq;            // �� MOD ��ֵ
    period[FTMn(ftmn)] = mod;
    switch((uint32_t)ftmn)                            // ��ֵ CNTIN ��Ϊ0 �������ȣ�CnV - CNTIN ���� CnV ���� �������ˡ�
    {
        // EPWM������ �� MOD - CNTIN + 0x0001 == MOD - 0 + 1
        // �� CnV = (MOD - 0 + 1) * ռ�ձ� = (MOD - 0 + 1) * duty/ FTM_PRECISON
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
    
    FTMX[ftmn]->MOD = mod;                  //����PWM����
    /******************** ѡ�����ģʽΪ ���ض���PWM *******************/
    //ͨ��״̬���ƣ�����ģʽ��ѡ�� ���ػ��ƽ      
    FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~FTM_CnSC_ELSA_MASK;
    FTMX[ftmn]->CONTROLS[ftm_chn].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
//    FTM_CnSC_REG(ftmn, ftm_chn) &= ~FTM_CnSC_ELSA_MASK;
//    FTM_CnSC_REG(ftmn, ftm_chn)  = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    // MSnB:MSnA = 1x       ���ض���PWM
    // ELSnB:ELSnA = 10     �ȸߺ��
    // ELSnB:ELSnA = 11     �ȵͺ��

    
    // set FTM clock to system clock 
    FTMX[FTMn(ftmn)]->SC = ( 0
                       //| FTM_SC_CPWMS_MASK       //0�����ض��� ��1�� ���Ķ��� ��ע���˱�ʾ 0��
                       | FTM_SC_PS(ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                       | FTM_SC_CLKS(1)            //ʱ��ѡ�� 0��ûѡ��ʱ�ӣ����ã� 1��bus ʱ�ӣ� 2��MCGFFCLK�� 3��EXTCLK�� ��SIM_SOPT4 ѡ������ܽ� FTM_CLKINx��
                       //| FTM_SC_TOIE_MASK        //����ж�ʹ�ܣ�ע���˱�ʾ ��ֹ����жϣ�
                     );

    
    if(ftmn == ftm2) FTMX[FTMn(ftmn)]->CNTIN = 0;         //���ü�������ֵ��һ��û������;������
    FTMX[FTMn(ftmn)]->CONTROLS[ftm_chn].CnV = cv;             //����ռ�ձ�
    FTMX[FTMn(ftmn)]->CNT = 0;                           //��������д����ֵ�������CNTIN��ֵ
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ռ�ձ�����
//  @param      ftmn            ѡ��ģ��
//  @param      ch              ѡ��ͨ��
//  @param      duty            ��Ҫ���õ�ռ�ձ�
//  @return     void
//  @since      v2.0
//  Sample usage:               FTM_PWM_Duty(ftm0,ftm_ch0,500);		//ftm0ģ�飬0ͨ�����ٷ�֮50��ռ�ձ�
//-------------------------------------------------------------------------------------------------------------------
void FTM_PWM_Duty(FTMn ftmn, FTM_CHn ch, uint32 duty)
{
    uint32 cv;
    switch((uint32_t)ftmn)
    {
        //ռ�ձȴ���100%ʱ��100��
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
    //ռ�ձ� = (CnV-CNTIN)/(MOD-CNTIN+1)
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
    
    FTMX[FTMn(ftmn)]->CONTROLS[ch].CnV = cv;      //����ռ�ձ�
}

/*!
 *  @brief      ����FTM ��PWM ͨ��ռ�ձ�
 *  @param      FTMn    ģ��ţ�FTM0��  FTM1��  FTM2��
 *  @param      FTM_CHn     ͨ���ţ�CH0~CH7��
 *  @param      duty    ռ�ձȷ��ӣ�ռ�ձ� = duty %
 *  @since      v1.0
 *  @note       ͬһ��FTM��PWMƵ���Ǳ���һ���ģ���ռ�ձȿɲ�һ������3��FTM�����������3����ͬƵ��PWM
 *  Sample usage:       FTM_PWM_Duty_Float(FTM0, FTM_CH6, 10);    //���� FTM0_CH6ռ�ձ�Ϊ 10%
 */
void FTM_PWM_Duty_Float(FTM_MemMapPtr ftmn, FTM_CHn ch, float duty)
{
    uint32 cv;
    uint32 mod = 0;

    //��ֹ����ռ�ձȳ���100%
    if(duty > 100.0)
    {
        duty = 100.0;
    }

    duty = duty/100.0;
    //ռ�ձ� = (CnV-CNTIN)/(MOD-CNTIN+1
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

    // ����FTMͨ��ֵ
    FTM_CnV_REG(ftmn, ch) = cv;

    //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ������������ CNTIN ��ֵ��
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      Ƶ������
//  @param      ftmn            ѡ��ģ��
//  @param      freq            ��Ҫ���õ�Ƶ�ʣ���λHZ��
//  @return     void
//  @note       �޸�PWMƵ�ʺ󣬱������ FTM_PWM_Duty ��������ռ�ձȡ�ͬһ��ģ�飬PWMƵ�ʱ�����ͬ��
//  @since      v1.0
//  Sample usage:               FTM_PWM_Freq(ftm0,50);		//ftm0ģ�飬50HZ
//-------------------------------------------------------------------------------------------------------------------
void FTM_PWM_Freq(FTMn ftmn, uint32 freq)
{
    uint32 clk_hz;        //busƵ��/2
    uint32 mod;
    uint8 ps;

    // �� CPWMS = 0 �����ض���
    clk_hz = timer_clk_khz * 1000 ;
    
    //clk_hz/(ps*mod) = freq        =>>     clk_hz/freq = (ps*mod)      =>>
    //clk_hz/freq < ((1<<n)*65536)  =>>    (clk_hz/freq)/65536 < (1>>n) =>> ((clk_hz/freq)/65536)>>n < 1
    mod = (clk_hz >> 16 ) / freq ;          // ��ʱ�� mod ����һ��
    ps = 0;
    while((mod >> ps) >= 1)                 // �� (mod >> ps) < 1 ���˳� while ѭ�� ������ PS ����Сֵ
        ps++;
    if(ps>0x07) return ;                    //�������÷�Χ��ֱ�Ӳ�����������������ԭ��һ��������PWMƵ��̫�ͣ���������Ƶ��̫�ߵ���
    
    mod = (clk_hz >> ps) / freq;            // �� MOD ��ֵ
    period[FTMn(ftmn)] = mod;
    
    
    FTMX[FTMn(ftmn)]->MOD = mod;                  //����PWM����
    
    // set FTM clock to system clock 
    FTMX[FTMn(ftmn)]->SC = ( 0
                       //| FTM_SC_CPWMS_MASK       //0�����ض��� ��1�� ���Ķ��� ��ע���˱�ʾ 0��
                       | FTM_SC_PS(ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                       | FTM_SC_CLKS(1)            //ʱ��ѡ�� 0��ûѡ��ʱ�ӣ����ã� 1��bus ʱ�ӣ� 2��MCGFFCLK�� 3��EXTCLK�� ��SIM_SOPT4 ѡ������ܽ� FTM_CLKINx��
                       //| FTM_SC_TOIE_MASK        //����ж�ʹ�ܣ�ע���˱�ʾ ��ֹ����жϣ�
                     );

    
    if(ftmn == ftm2) FTMX[FTMn(ftmn)]->CNTIN = 0;  //���ü�������ֵ��һ��û������;������
    FTMX[FTMn(ftmn)]->CNT = 0;                    //��������д����ֵ�������CNTIN��ֵ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ⲿ������ʼ��
//  @param      ftmn            ѡ��ģ��
//  @return     void
//  @since      v2.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void FTM_Count_Init(FTMn ftmn, FTM_CHn ftm_chn, PTX_n ptx_n, FTM_Input_cfg cfg)
{
    ASSERT(  (ftmn == ftm2) );  //��鴫�ݽ�����ͨ���Ƿ���ȷ (�Ĵ������ã�ò��ֻ��FTM2���������벶׽�Ĺ���)

    FTM_Pinmux(ftmn,ftm_chn,ptx_n);                                    //���Ÿ��� �������� ������Ӧ����ʱ��

    switch(cfg)
    {
        //���벶׽ģʽ�£�DECAPEN = 0 �� DECAPEN = 0 ��CPWMS = 0�� MSnB:MSnA = 0

        // ELSnB:ELSnA         1          10          11
        // ����             ������      �½���      ������

    case FTM_Rising:    //�����ش���
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC |=  ( FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK );                   //��1
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~( FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //��0
        break;

    case FTM_Falling:   //�½��ش���
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_CHIE_MASK );                    //��1
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~( FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //��0
        break;

    case FTM_Rising_or_Falling: //�����ء��½��ض�����
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC |=  ( FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK ); //��1
        FTMX[ftmn]->CONTROLS[ftm_chn].CnSC &= ~( FTM_CnSC_MSB_MASK  | FTM_CnSC_MSA_MASK); //��0
        break;
    }

    FTMX[FTMn(ftmn)]->SC = 0;
    FTMX[FTMn(ftmn)]->MODE  =    (0
                              | FTM_MODE_WPDIS_MASK  //д������ֹ
                              //| FTM_MODE_FTMEN_MASK   //ʹ�� FTM
                             );


    FTMX[FTMn(ftmn)]->COMBINE = 0;
    //FTM_MODE_REG(FTMN[ftmn])    &= ~FTM_MODE_FTMEN_MASK;    //ʹ��FTM
    FTMX[FTMn(ftmn)]->CNTIN = 0;

    FTMX[FTMn(ftmn)]->STATUS  = 0x00;               //���жϱ�־λ

    FTMX[FTMn(ftmn)]->MOD  = FTM_MOD_MOD_MASK;
    FTMX[FTMn(ftmn)]->SC |= FTM_SC_PS(0);	                        //��Ƶϵ��	
    FTMX[FTMn(ftmn)]->SC |= FTM_SC_CLKS(1);                       //ѡ��BUSʱ����ΪFTM����ʱ��
    FTMX[FTMn(ftmn)]->MODE |=    (0
                               //| FTM_MODE_WPDIS_MASK  //д������ֹ
                               | FTM_MODE_FTMEN_MASK   //ʹ�� FTM
                              );            
    FTMX[FTMn(ftmn)]->CNT = 0;                                    //���س�ʼ��ֵ
    //�������벶׽�ж�
    //enable_irq(FTM0_IRQn + ftmn);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ֵ
//  @param      ftmn            ѡ��ģ��
//  @return     uint16          ���ؼ���ֵ
//  @since      v2.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
uint16 FTM_Count_Get(FTMn ftmn, FTM_CHn ftm_chn)
{
  return (uint16)FTMX[ftmn]->CONTROLS[ftm_chn].CnV;   //����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ֵ
//  @param      ftmn            ѡ��ģ��
//  @since      v2.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void FTM_Count_Clean(FTMn ftmn)
{
    FTMX[FTMn(ftmn)]->CNT = 0;
}

//////////////////////////////// ����Ϊ���벶׽  //////////////////////////////////////////
//ʹ�ܶ�Ӧģ���ʱ��
static inline void FTM_Enable_CLK(FTMn ftmn)
{
    /******************* ����ʱ��*******************/
    BIT_SET(SIM_SCGC, (SIM_SCGC_FTM0_SHIFT + FTMn(ftmn)));
}//����FTM0�ȼ���SIM->SCGC |= SIM_SCC_FTM0_MASK;
//////////////////////////////// ����Ϊ�ⲿ����������� //////////////////////////////////////////
//����Ƶ�ʽϸ�ʱ�����÷�Ƶ����
//�����ŷķ�����ֿ�©����ģ���Ҫ����������port_pull(FTM0_PULSE_PIN,PULLUP_ENBLE);
void FTM_Pulse_Pinmux(FTMn ftmn, PTX_n ptx_n)
{
    int n = FTMn(ftmn);

    FTM_Enable_CLK(ftmn);//����FTM����ʱ��
    SIM_BASE_PTR->PINSEL &= ~(SIM_PINSEL_FTM0CLKPS_MASK << (n * 2) );//����ⲿʱ������ѡ��
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
                    | FTM_SC_CLKS(3)      //�ⲿʱ��
                  );



}

uint16_t FTM_Pulse_Get(FTMn ftmn)
{
    return FTMX[FTMn(ftmn)]->CNT ;
}
