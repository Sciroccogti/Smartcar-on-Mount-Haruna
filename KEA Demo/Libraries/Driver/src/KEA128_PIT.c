/*
  ******************************************************************************
  * @file    pit.c
  * @author  xhJiang
  * @version V1.0
  * @date    2018.4.2
  * @mail:   blog_xhjiang@163.com
  * @brief   Pit configure
  ******************************************************************************
  */
#include "common.h"
#include "KEA128_PIT.h"

/**********************************************************************************************
* Global variables
**********************************************************************************************/
pt2FuncU8 PIT_Callback;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����pitͨ����ʼ��
//  @param      pitn            ѡ��pitģ��
//  @param      us             ��ʱ�жϵ�ʱ��(��λus)
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Init1(pit0,1000);								//��ʱ1000��busʱ�Ӻ��ж�
//				set_irq_priority(PIT_CH0_IRQn,1);					//�������ȼ�,�����Լ����������� �����÷�ΧΪ 0 - 3
//PIT �õ��� Bus Clock ����Ƶ�� EnableInterrupts;
//������� = ����Ƶ�� * ʱ��
//����ܳ���32λ:4294967296000/bus_clk_khz(ns),����20M����ʱ�ӣ��ʱ��Ϊ214748us,���Ҫʹ�ø�����ʱ�䣬��ʹ��PIT_Init2										//���ܵ��жϿ���
//-------------------------------------------------------------------------------------------------------------------
void PIT_Init1(PITn pitn, uint32 us)
{
  ASSERT( us > 0 );                                               //�ö��Լ�� ʱ����벻��Ϊ 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//ʹ��PITʱ��
//  PIT_BASE_PTR->MCR         = (vuint32)PIT_BASE_PTR->MCR;                   //��ʱ��֤�ɹ�����PIT
  PIT_BASE_PTR->MCR         = 0;						//MDIS = 0 ʹ��PITʱ��;FRZ = 0��Debugģʽ�¶�ʱ��ʹ��			//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_LDVAL(pitn)  = us * bus_clk_khz/1000 - 1;                    //��Ч��PIT->CHANNEL[pitn].LDVAL ��������ж�ʱ��
  PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;				//����жϱ�־λ
//  PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
                                | PIT_TCTRL_TIE_MASK        //��PITn�ж�
                                );
  //enable_irq( (IRQn_Type)((int)pitn + PIT_CH0_IRQn) );       //���ж�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����pit���ӳ�ʼ��
//  @note       �������ͨ��ѡ����Ϊֻ������ͨ������ʹ�ã��ж���PIT1������
//  @param      ms             ��ʱ��0��ʱ��ʱ��ʱ��(��λms)
//  @param      times             ��ʱ��1��ѭ��������ÿһ����ʱ��0���ڴ���һ�Σ�
//  @note       �ܵ���ʱʱ��Ϊ  ms*times
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Init2(10,1000);								//��ʱ1000��busʱ�Ӻ��ж�
//				set_irq_priority(PIT_CH1_IRQn,1);					//�������ȼ�,�����Լ����������� �����÷�ΧΪ 0 - 3
//                              EnableInterrupts;
//-------------------------------------------------------------------------------------------------------------------
void PIT_Init2(uint32 ms, uint32 times)
{
  ASSERT( (ms > 0) && (times > 0));                                               //�ö��Լ�� ʱ����벻��Ϊ 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//ʹ��PITʱ��
  PIT_BASE_PTR->MCR         = 0;						//MDIS = 0 ʹ��PITʱ��;FRZ = 0��Debugģʽ�¶�ʱ��ʹ��

  PIT_BASE_PTR->CHANNEL[pit1].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_LDVAL(pit1)  = times - 1;                    //��Ч��PIT->CHANNEL[pitn].LDVAL ��������ж�ʱ��
  PIT_TFLG(pit1)  |= PIT_TFLG_TIF_MASK;				//����жϱ�־λ
  PIT_BASE_PTR->CHANNEL[pit1].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //ʹ�� PIT1��ʱ��
                                | PIT_TCTRL_CHN_MASK        //����TIMER1��TIMER0
                                | PIT_TCTRL_TIE_MASK
                                );
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_LDVAL(pit0)  = ms * bus_clk_khz - 1;                    //��Ч��PIT->CHANNEL[pitn].LDVAL ��������ж�ʱ��
  PIT_TFLG(pit0)  |= PIT_TFLG_TIF_MASK;				//����жϱ�־λ
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //ʹ�� PIT0��ʱ��
                                );

//  enable_irq( (IRQn_Type)((int)pitn + PIT_CH0_IRQn) );       //���ж�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit��ʱ
//  @param      pitn            ѡ��pitģ��
//  @param      us             ��ʱ��ʱ��ʱ��(��λus)ͬpit��ʼ����������Ҫ�ȳ�ʼ��PIT
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Delay1(pit0,1000);		//��ʱ1000��busʱ��
//-------------------------------------------------------------------------------------------------------------------
void PIT_Delay1(PITn pitn, uint32 us)
{
  ASSERT( us > 0 );                                             //�ö��Լ�� ʱ����벻��Ϊ 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//ʹ��PITʱ��
//  PIT->MCR         = (vuint32)PIT->MCR;                   //��ʱ��֤�ɹ�����PIT
  PIT_BASE_PTR->MCR         = 0;						//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;               //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = us * bus_clk_khz/1000 - 1;              			//��������ж�ʱ��
  PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;							//����жϱ�־λ
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = PIT_TCTRL_TEN_MASK;        //ʹ�� PITn��ʱ��
  while( !((PIT_BASE_PTR->CHANNEL[pitn].TFLG) & PIT_TFLG_TIF_MASK) ){}//�ȴ�ʱ�䵽

  PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;                         //���жϱ�־λ
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����pit�����ӳ�
//  @note       �������ͨ��ѡ����Ϊֻ������ͨ������ʹ�ã���־λ��PIT1������
//  @param      ms             ��ʱ��0��ʱ��ʱ��ʱ��(��λms)
//  @param      times             ��ʱ��1��ѭ��������ÿһ����ʱ��0���ڴ���һ�Σ�
//  @note       �ܵ���ʱʱ��Ϊ  ms*times
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Delay2(10,1000);			//��ʱ1000��busʱ�Ӻ��ж�
//-------------------------------------------------------------------------------------------------------------------
void PIT_Delay2(uint32 ms, uint32 times)
{
  ASSERT( (ms > 0) && (times > 0));                                               //�ö��Լ�� ʱ����벻��Ϊ 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//ʹ��PITʱ��
  PIT_BASE_PTR->MCR         = 0;						//MDIS = 0 ʹ��PITʱ��;FRZ = 0��Debugģʽ�¶�ʱ��ʹ��

  PIT_BASE_PTR->CHANNEL[pit1].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_LDVAL(pit1)  = times - 1;                    //��Ч��PIT->CHANNEL[pitn].LDVAL ��������ж�ʱ��
  PIT_TFLG(pit1)  |= PIT_TFLG_TIF_MASK;				//����жϱ�־λ
  PIT_BASE_PTR->CHANNEL[pit1].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //ʹ�� PIT1��ʱ��
                                | PIT_TCTRL_CHN_MASK        //����TIMER1��TIMER0
                                );
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_LDVAL(pit0)  = ms * bus_clk_khz - 1;                    //��Ч��PIT->CHANNEL[pitn].LDVAL ��������ж�ʱ��
  PIT_TFLG(pit0)  |= PIT_TFLG_TIF_MASK;				//����жϱ�־λ
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //ʹ�� PIT0��ʱ��
                                );
  while( !((PIT_BASE_PTR->CHANNEL[pit1].TFLG) & PIT_TFLG_TIF_MASK) ){}//�ȴ�ʱ�䵽

  PIT_TFLG(pit1)  |= PIT_TFLG_TIF_MASK;                         //���жϱ�־λ

//  enable_irq( (IRQn_Type)((int)pitn + PIT_CH0_IRQn) );       //���ж�
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit��ʼ��ʱ
//  @param      pitn            ѡ��pitģ��
//  @return     void
//  @since      v2.0
//  Sample usage:               PIT_Time_Start(pit0);		//pit0��ʼ��ʱ
//-------------------------------------------------------------------------------------------------------------------
void PIT_Time_Start(PITn pitn)
{
	SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//ʹ��PITʱ��
//        PIT->MCR         = (vuint32)PIT->MCR;                   //��ʱ��֤�ɹ�����PIT
	PIT_BASE_PTR->MCR         = 0;						//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
	PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
	PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = ~0;              			//��������ж�ʱ��
	PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;				//����жϱ�־λ
        PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = PIT_TCTRL_TEN_MASK;        //ʹ�� PITn��ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��ǰPITn��ʱʱ��
//  @param      pitn            ѡ��pitģ��
//  @return     void
//  @since      v2.0
//  Sample usage:               PIT_Time_Get(pit0);			//��ȡ��ǰpit0��ʱʱ��
//-------------------------------------------------------------------------------------------------------------------
uint32 PIT_Time_Get(PITn pitn)
{
	uint32 val;
	val = (uint32)(~0) - PIT_BASE_PTR->CHANNEL[pitn].CVAL;
	if(PIT_BASE_PTR->CHANNEL[pitn].TFLG &  PIT_TFLG_TIF_MASK)		//�ж�ʱ���Ƿ�ʱ
	{
		PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;			//����жϱ�־λ
		PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;   //��ֹPITn��ʱ����������ռ���ֵ��
		return 0XFFFFFFFF;									//��ʱ�������ֵ
	}
	return val;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit�ر�
//  @param      pitn            ѡ��pitģ��
//  @return     void
//  @since      v2.0
//  Sample usage:               PIT_Close(pit0);			//pit0ֹͣ��ʱ
//-------------------------------------------------------------------------------------------------------------------
void PIT_Close(PITn pitn)
{
	PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;				//����жϱ�־λ
	PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
}


/***********************************************************************************************
*
* @brief    KBI_SetCallback - Set a callback function to execute on a falling edge of an enabled KBI pin
* @param    pointer to function with an UINT8 argument
* @return   none
*
************************************************************************************************/

void PIT_SetCallback(pt2FuncU8 ptr)
{
	PIT_Callback = ptr;
}


/***********************************************************************************************
*
* @brief    KBI0_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void PIT_CH0_IRQHandler()

{

	PIT_Flag_Clear(pit0);	/*Clear flag*/


        PIT_Callback(0);

}
/***********************************************************************************************
*
* @brief    KBI0_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void PIT_CH1_IRQHandler()

{

	PIT_Flag_Clear(pit1);	/*Clear flag*/


        PIT_Callback(1);

}

