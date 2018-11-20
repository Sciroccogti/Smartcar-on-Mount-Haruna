/*
  ******************************************************************************
  * @file    KEA128_KBI.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.7.23
  * @mail:   blog_xhjiang@163.com
  * @brief   KBI configure
  * @note    参考飞卡官方库的指针函数写法用于中断函数回调，并结合其他第三方库如逐飞、山外库综合完善后得到，所有函数均已测试
  * sample usage 
      GPIO_Init(G5,GPI,LOW);            //首先初始化相应IO口配置，尤其注意是否将上拉电阻配置使能
      KBI_Init(KBI1_P21,IRQ_RISING);    //初始化对应KBI端口
      KBI_SetCallback(KBI_Interrupt);   //注册中断回调函数
      Enable_Interrupt(INT_KBI1);       //使能相应中断

      void KBI_Interrupt(uint8 ch){}    //中断执行函数
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
pt2FuncU8 KBI_Callback;//指针函数，由中断注册函数赋值

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

//KBI中断使能
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
* @brief    KBI键盘中断初始化
* @param    none
* @return   none
* Sample usage:                 GPIO_Init()     //先配置相应的GPIO为输入,用于使能输入与上拉配置
*                               KBI_Init(KBI1_P0,IRQ_RISING);		            //通道选择为KBI1_P0，上升沿触发
************************************************************************************************/
void KBI_Init(KBIx_CHn chn, TRIG_CFG cfg)
{
    uint8 kbi_x,kbi_n;
    
    kbi_x = KBIx(chn);      //获取模块号
    kbi_n = KBIn(chn);      //获取引脚号
    
    //开启KBI时钟
    if(0 == kbi_x)  SIM_SCGC |= SIM_SCGC_KBI0_MASK;
    else            SIM_SCGC |= SIM_SCGC_KBI1_MASK;
    

    //屏蔽KBI中断
    KBI_DisableInterrupts(kbi_x);
    kbi[kbi_x]->SC &= ~(uint32)(KBI_SC_KBIE_MASK | KBI_SC_KBSPEN_MASK);

    //设置触发模式
    if((IRQ_FALLING == cfg)|| (IRQ_FALLING_LOW == cfg))
      kbi[kbi_x]->ES &= ~((uint32)1<<kbi_n);
    else
      kbi[kbi_x]->ES |= ((uint32)1<<kbi_n);

    if((IRQ_FALLING == cfg) || (IRQ_RISING == cfg))
    {
        kbi[kbi_x]->SC &= ~(KBI_SC_KBMOD_MASK);
        kbi[kbi_x]->SC |= ( 0
                        //| KBI_SC_KBMOD_MASK   //边沿触发模式 0：边沿触发   1：电平触发
                        | KBI_SC_KBSPEN_MASK    //使能KBI_SP寄存器
                          ); 
    }
    else
    {
        kbi[kbi_x]->SC |= ( 0
                        | KBI_SC_KBMOD_MASK   //边沿触发模式 0：边沿触发   1：电平触发
                        | KBI_SC_KBSPEN_MASK    //使能KBI_SP寄存器   
                          );
    }    

    
    //使能KBI引脚
    kbi[kbi_x]->PE |= (1<<kbi_n);
    
//    //设置上拉
//   在GPIO中配置
    
    kbi[kbi_x]->SC |= KBI_SC_KBACK_MASK; //clear flags
    kbi[kbi_x]->SC |= KBI_SC_RSTKBSP_MASK;  //Clearing RSTKBSP bit
    KBI_EnableInterrupts(kbi_x);

}
/***********************************************************************************************
*
* @brief    KBI_Deinit - Deinitializes the KBI module
* @brief    KBI键盘中断取消初始化
* @param    none
* @return   none
************************************************************************************************/

void KBI_Deinit(KBIx_CHn chn)
{
  uint8 kbi_x,kbi_n;

  kbi_x = KBIx(chn);      //获取模块号
  kbi_n = KBIn(chn);      //获取引脚号
  kbi[kbi_x]->SC |= ( 0
                     |KBI_SC_KBACK_MASK
                     |KBI_SC_RSTKBSP_MASK /* Reset kbi sp register. */
                     );
  kbi[kbi_x]->PE &= ~(uint32)(1<<kbi_n);
  if(!kbi[kbi_x]->PE) //如果该模块所有引脚均被禁用，则关闭对应模块及其时钟
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
  if(KBI0_SC & KBI_SC_KBF_MASK)//判断是否触发 KBI 中断
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
  if(KBI1_SC & KBI_SC_KBF_MASK)//判断是否触发 KBI 中断
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

