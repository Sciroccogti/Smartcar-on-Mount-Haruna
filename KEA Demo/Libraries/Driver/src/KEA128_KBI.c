/*
  ******************************************************************************
  * @file    KEA128_KBI.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.7.23
  * @mail:   blog_xhjiang@163.com
  * @brief   KBI configure
  * @note    �ο��ɿ��ٷ����ָ�뺯��д�������жϺ����ص������������������������ɡ�ɽ����ۺ����ƺ�õ������к������Ѳ���
  * sample usage 
      GPIO_Init(G5,GPI,LOW);            //���ȳ�ʼ����ӦIO�����ã�����ע���Ƿ�������������ʹ��
      KBI_Init(KBI1_P21,IRQ_RISING);    //��ʼ����ӦKBI�˿�
      KBI_SetCallback(KBI_Interrupt);   //ע���жϻص�����
      Enable_Interrupt(INT_KBI1);       //ʹ����Ӧ�ж�

      void KBI_Interrupt(uint8 ch){}    //�ж�ִ�к���
  ******************************************************************************
  */


#include "KEA128_KBI.h"


KBI_Type * kbi[2] = KBI_BASES;
/**********************************************************************************************
* External objects
**********************************************************************************************/


/**********************************************************************************************
* Global variables
**********************************************************************************************/
pt2FuncU8 KBI_Callback;//ָ�뺯�������ж�ע�ắ����ֵ

/**********************************************************************************************
* Constants and macros
**********************************************************************************************/


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/


/**********************************************************************************************
* Local variables
**********************************************************************************************/


/**********************************************************************************************
* Local functions
**********************************************************************************************/


/**********************************************************************************************
* Global functions
**********************************************************************************************/

//KBI�ж�ʹ��
static inline void KBI_EnableInterrupts(uint8 kbi_x)
{
  kbi[kbi_x]->SC |= KBI_SC_KBIE_MASK;
}
static inline void KBI_DisableInterrupts(uint8 kbi_x)
{
  kbi[kbi_x]->SC &= ~KBI_SC_KBIE_MASK;
}

/***********************************************************************************************
*
* @brief    KBI_Init - Enable Keyboard interrupts on channels
* @brief    KBI�����жϳ�ʼ��
* @param    none
* @return   none
* Sample usage:                 GPIO_Init()     //��������Ӧ��GPIOΪ����,����ʹ����������������
*                               KBI_Init(KBI1_P0,IRQ_RISING);		            //ͨ��ѡ��ΪKBI1_P0�������ش���
************************************************************************************************/
void KBI_Init(KBIx_CHn chn, TRIG_CFG cfg)
{
    uint8 kbi_x,kbi_n;
    
    kbi_x = KBIx(chn);      //��ȡģ���
    kbi_n = KBIn(chn);      //��ȡ���ź�
    
    //����KBIʱ��
    if(0 == kbi_x)  SIM_SCGC |= SIM_SCGC_KBI0_MASK;
    else            SIM_SCGC |= SIM_SCGC_KBI1_MASK;
    

    //����KBI�ж�
    KBI_DisableInterrupts(kbi_x);
    kbi[kbi_x]->SC &= ~(uint32)(KBI_SC_KBIE_MASK | KBI_SC_KBSPEN_MASK);

    //���ô���ģʽ
    if((IRQ_FALLING == cfg)|| (IRQ_FALLING_LOW == cfg))
      kbi[kbi_x]->ES &= ~((uint32)1<<kbi_n);
    else
      kbi[kbi_x]->ES |= ((uint32)1<<kbi_n);

    if((IRQ_FALLING == cfg) || (IRQ_RISING == cfg))
    {
        kbi[kbi_x]->SC &= ~(KBI_SC_KBMOD_MASK);
        kbi[kbi_x]->SC |= ( 0
                        //| KBI_SC_KBMOD_MASK   //���ش���ģʽ 0�����ش���   1����ƽ����
                        | KBI_SC_KBSPEN_MASK    //ʹ��KBI_SP�Ĵ���
                          ); 
    }
    else
    {
        kbi[kbi_x]->SC |= ( 0
                        | KBI_SC_KBMOD_MASK   //���ش���ģʽ 0�����ش���   1����ƽ����
                        | KBI_SC_KBSPEN_MASK    //ʹ��KBI_SP�Ĵ���   
                          );
    }    

    
    //ʹ��KBI����
    kbi[kbi_x]->PE |= (1<<kbi_n);
    
//    //��������
//   ��GPIO������
    
    kbi[kbi_x]->SC |= KBI_SC_KBACK_MASK; //clear flags
    kbi[kbi_x]->SC |= KBI_SC_RSTKBSP_MASK;  //Clearing RSTKBSP bit
    KBI_EnableInterrupts(kbi_x);

}
/***********************************************************************************************
*
* @brief    KBI_Deinit - Deinitializes the KBI module
* @brief    KBI�����ж�ȡ����ʼ��
* @param    none
* @return   none
************************************************************************************************/

void KBI_Deinit(KBIx_CHn chn)
{
  uint8 kbi_x,kbi_n;

  kbi_x = KBIx(chn);      //��ȡģ���
  kbi_n = KBIn(chn);      //��ȡ���ź�
  kbi[kbi_x]->SC |= ( 0
                     |KBI_SC_KBACK_MASK
                     |KBI_SC_RSTKBSP_MASK /* Reset kbi sp register. */
                     );
  kbi[kbi_x]->PE &= ~(uint32)(1<<kbi_n);
  if(!kbi[kbi_x]->PE) //�����ģ���������ž������ã���رն�Ӧģ�鼰��ʱ��
  {
    kbi[kbi_x]->SC &= ~KBI_SC_KBSPEN_MASK;
    if(0 == kbi_x)  SIM_SCGC &= ~SIM_SCGC_KBI0_MASK;
    else            SIM_SCGC &= ~SIM_SCGC_KBI1_MASK;
  }
}

/***********************************************************************************************
*
* @brief    KBI_SetCallback - Set a callback function to execute on a falling edge of an enabled KBI pin
* @param    pointer to function with an UINT8 argument
* @return   none
*
************************************************************************************************/

void KBI_SetCallback(pt2FuncU8 ptr)
{
  KBI_Callback = ptr;
}

/***********************************************************************************************
*
* @brief    KBI0_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void KBI0_IRQHandler()
{
  if(KBI0_SC & KBI_SC_KBF_MASK)//�ж��Ƿ񴥷� KBI �ж�
  {
    uint8 chn = 0;
    while (chn<32)
    {
      if((KBI0_SP & KBI_SP_SP(1<<chn))>>chn)
      {
        KBI_Callback(chn);
      }
      chn++;
    }
  }
  KBI0_SC |= KBI_SC_KBACK_MASK;
  KBI0_SC |= KBI_SC_RSTKBSP_MASK;	/*Clear flag*/
}

void KBI1_IRQHandler()
{
  if(KBI1_SC & KBI_SC_KBF_MASK)//�ж��Ƿ񴥷� KBI �ж�
  {
    uint8 chn = 0;
    uint8 temp = 0;
    while (chn<32)
    {
      if(BIT_GET(KBI1_SP,chn))//(KBI1_SP & KBI_SP_SP(1<<chn))>>chn
      {
        temp = chn+32;
        KBI_Callback(temp);
      }
      chn++;
    }
  }
  KBI1_SC |= KBI_SC_KBACK_MASK;
  KBI1_SC |= KBI_SC_RSTKBSP_MASK;  /*Clear flag*/
}

