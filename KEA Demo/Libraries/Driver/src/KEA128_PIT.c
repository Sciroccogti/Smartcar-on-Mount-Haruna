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
//  @brief      单个pit通道初始化
//  @param      pitn            选择pit模块
//  @param      us             定时中断的时间(单位us)
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Init1(pit0,1000);								//定时1000个bus时钟后中断
//				set_irq_priority(PIT_CH0_IRQn,1);					//设置优先级,根据自己的需求设置 可设置范围为 0 - 3
//PIT 用的是 Bus Clock 总线频率 EnableInterrupts;
//溢出计数 = 总线频率 * 时间
//最大不能超过32位:4294967296000/bus_clk_khz(ns),对于20M总线时钟，最长时间为214748us,如果要使用更长的时间，请使用PIT_Init2										//打开总的中断开关
//-------------------------------------------------------------------------------------------------------------------
void PIT_Init1(PITn pitn, uint32 us)
{
  ASSERT( us > 0 );                                               //用断言检测 时间必须不能为 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
//  PIT_BASE_PTR->MCR         = (vuint32)PIT_BASE_PTR->MCR;                   //延时保证成功开启PIT
  PIT_BASE_PTR->MCR         = 0;						//MDIS = 0 使能PIT时钟;FRZ = 0，Debug模式下定时器使能			//使能PIT定时器时钟 ，调试模式下继续运行
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //禁止PITn定时器（用于清空计数值）
  PIT_LDVAL(pitn)  = us * bus_clk_khz/1000 - 1;                    //等效于PIT->CHANNEL[pitn].LDVAL 设置溢出中断时间
  PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;				//清除中断标志位
//  PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //使能 PITn定时器
                                | PIT_TCTRL_TIE_MASK        //开PITn中断
                                );
  //enable_irq( (IRQn_Type)((int)pitn + PIT_CH0_IRQn) );       //开中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      两个pit链接初始化
//  @note       无需进行通道选择，因为只有两个通道都被使用，中断由PIT1触发。
//  @param      ms             定时器0定时延时的时间(单位ms)
//  @param      times             定时器1的循环次数（每一个定时器0周期触发一次）
//  @note       总的延时时间为  ms*times
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Init2(10,1000);								//定时1000个bus时钟后中断
//				set_irq_priority(PIT_CH1_IRQn,1);					//设置优先级,根据自己的需求设置 可设置范围为 0 - 3
//                              EnableInterrupts;
//-------------------------------------------------------------------------------------------------------------------
void PIT_Init2(uint32 ms, uint32 times)
{
  ASSERT( (ms > 0) && (times > 0));                                               //用断言检测 时间必须不能为 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
  PIT_BASE_PTR->MCR         = 0;						//MDIS = 0 使能PIT时钟;FRZ = 0，Debug模式下定时器使能

  PIT_BASE_PTR->CHANNEL[pit1].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //禁止PITn定时器（用于清空计数值）
  PIT_LDVAL(pit1)  = times - 1;                    //等效于PIT->CHANNEL[pitn].LDVAL 设置溢出中断时间
  PIT_TFLG(pit1)  |= PIT_TFLG_TIF_MASK;				//清除中断标志位
  PIT_BASE_PTR->CHANNEL[pit1].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //使能 PIT1定时器
                                | PIT_TCTRL_CHN_MASK        //链接TIMER1到TIMER0
                                | PIT_TCTRL_TIE_MASK
                                );
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //禁止PITn定时器（用于清空计数值）
  PIT_LDVAL(pit0)  = ms * bus_clk_khz - 1;                    //等效于PIT->CHANNEL[pitn].LDVAL 设置溢出中断时间
  PIT_TFLG(pit0)  |= PIT_TFLG_TIF_MASK;				//清除中断标志位
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //使能 PIT0定时器
                                );

//  enable_irq( (IRQn_Type)((int)pitn + PIT_CH0_IRQn) );       //开中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit延时
//  @param      pitn            选择pit模块
//  @param      us             定时延时的时间(单位us)同pit初始化，但不需要先初始化PIT
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Delay1(pit0,1000);		//延时1000个bus时钟
//-------------------------------------------------------------------------------------------------------------------
void PIT_Delay1(PITn pitn, uint32 us)
{
  ASSERT( us > 0 );                                             //用断言检测 时间必须不能为 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
//  PIT->MCR         = (vuint32)PIT->MCR;                   //延时保证成功开启PIT
  PIT_BASE_PTR->MCR         = 0;						//使能PIT定时器时钟 ，调试模式下继续运行
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;               //禁止PITn定时器（用于清空计数值）
  PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = us * bus_clk_khz/1000 - 1;              			//设置溢出中断时间
  PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;							//清除中断标志位
  PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = PIT_TCTRL_TEN_MASK;        //使能 PITn定时器
  while( !((PIT_BASE_PTR->CHANNEL[pitn].TFLG) & PIT_TFLG_TIF_MASK) ){}//等待时间到

  PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;                         //清中断标志位
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      两个pit链接延迟
//  @note       无需进行通道选择，因为只有两个通道都被使用，标志位由PIT1触发。
//  @param      ms             定时器0定时延时的时间(单位ms)
//  @param      times             定时器1的循环次数（每一个定时器0周期触发一次）
//  @note       总的延时时间为  ms*times
//  @return     void
//  @since      v1.0
//  Sample usage:               PIT_Delay2(10,1000);			//定时1000个bus时钟后中断
//-------------------------------------------------------------------------------------------------------------------
void PIT_Delay2(uint32 ms, uint32 times)
{
  ASSERT( (ms > 0) && (times > 0));                                               //用断言检测 时间必须不能为 0
  SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
  PIT_BASE_PTR->MCR         = 0;						//MDIS = 0 使能PIT时钟;FRZ = 0，Debug模式下定时器使能

  PIT_BASE_PTR->CHANNEL[pit1].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //禁止PITn定时器（用于清空计数值）
  PIT_LDVAL(pit1)  = times - 1;                    //等效于PIT->CHANNEL[pitn].LDVAL 设置溢出中断时间
  PIT_TFLG(pit1)  |= PIT_TFLG_TIF_MASK;				//清除中断标志位
  PIT_BASE_PTR->CHANNEL[pit1].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //使能 PIT1定时器
                                | PIT_TCTRL_CHN_MASK        //链接TIMER1到TIMER0
                                );
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL &= ~ PIT_TCTRL_TEN_MASK;              //禁止PITn定时器（用于清空计数值）
  PIT_LDVAL(pit0)  = ms * bus_clk_khz - 1;                    //等效于PIT->CHANNEL[pitn].LDVAL 设置溢出中断时间
  PIT_TFLG(pit0)  |= PIT_TFLG_TIF_MASK;				//清除中断标志位
  PIT_BASE_PTR->CHANNEL[pit0].TCTRL  = ( 0
                                | PIT_TCTRL_TEN_MASK        //使能 PIT0定时器
                                );
  while( !((PIT_BASE_PTR->CHANNEL[pit1].TFLG) & PIT_TFLG_TIF_MASK) ){}//等待时间到

  PIT_TFLG(pit1)  |= PIT_TFLG_TIF_MASK;                         //清中断标志位

//  enable_irq( (IRQn_Type)((int)pitn + PIT_CH0_IRQn) );       //开中断
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit开始计时
//  @param      pitn            选择pit模块
//  @return     void
//  @since      v2.0
//  Sample usage:               PIT_Time_Start(pit0);		//pit0开始计时
//-------------------------------------------------------------------------------------------------------------------
void PIT_Time_Start(PITn pitn)
{
	SIM_BASE_PTR->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
//        PIT->MCR         = (vuint32)PIT->MCR;                   //延时保证成功开启PIT
	PIT_BASE_PTR->MCR         = 0;						//使能PIT定时器时钟 ，调试模式下继续运行
	PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
	PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = ~0;              			//设置溢出中断时间
	PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;				//清除中断标志位
        PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = PIT_TCTRL_TEN_MASK;        //使能 PITn定时器
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取当前PITn计时时间
//  @param      pitn            选择pit模块
//  @return     void
//  @since      v2.0
//  Sample usage:               PIT_Time_Get(pit0);			//获取当前pit0计时时间
//-------------------------------------------------------------------------------------------------------------------
uint32 PIT_Time_Get(PITn pitn)
{
	uint32 val;
	val = (uint32)(~0) - PIT_BASE_PTR->CHANNEL[pitn].CVAL;
	if(PIT_BASE_PTR->CHANNEL[pitn].TFLG &  PIT_TFLG_TIF_MASK)		//判断时间是否超时
	{
		PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;			//清除中断标志位
		PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;   //禁止PITn定时器（用于清空计数值）
		return 0XFFFFFFFF;									//超时返回最大值
	}
	return val;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit关闭
//  @param      pitn            选择pit模块
//  @return     void
//  @since      v2.0
//  Sample usage:               PIT_Close(pit0);			//pit0停止计时
//-------------------------------------------------------------------------------------------------------------------
void PIT_Close(PITn pitn)
{
	PIT_TFLG(pitn)  |= PIT_TFLG_TIF_MASK;				//清除中断标志位
	PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
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

