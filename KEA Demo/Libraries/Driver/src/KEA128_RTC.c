/*
  ******************************************************************************
  * @file    rtc.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.5.13
  * @mail:   blog_xhjiang@163.com
  * @brief   rtc configure,暂不可用
  ******************************************************************************
  */


#include "common.h"
#include "KEA128_RTC.h"

//设置定时器时间
void RTC_Timeset(uint16 timeset)
{
  RTC_BASE_PTR->MOD = timeset;
}

//配置100ms周期定时器
void RTC_Init(void)
{
  SIM->SCGC  |= SIM_SCGC_RTC_MASK;//开启时钟
  RTC_SC = (0
           |RTC_SC_RTCLKS(1)      //选择1KHz时钟
           |RTC_SC_RTCPS(6)       //选择100分频
           |RTC_SC_RTIF_MASK);    //清中断请求
                                  //RTIE = 0,中断禁用
                                  //RTCO = 0,计时器输出禁用

}

//读取RTC计时器数值
uint16 RTC_Read(void)
{
  uint16 temp;
  uint16 count;
  uint8 counth;
  uint8 countl;
  temp = RTC_CNT;
  counth = temp;//先读取CNT[7:0]
  countl = temp >>8;//后读取CNT[15:8]
  count = (counth << 8) + countl;
  return count;
}

void RTC_Clear(void)
{
  //关闭分频时钟会清空计数
  RTC_SC &= (0
             |RTC_SC_RTCLKS(1)
             |RTC_SC_RTIF_MASK);
  //再次开启分频计数
  RTC_SC = (0
           |RTC_SC_RTCLKS(1)      //选择1KHz时钟
           |RTC_SC_RTCPS(6)       //选择100分频
           |RTC_SC_RTIF_MASK);    //清中断请求
                                  //RTIE = 0,中断禁用
                                  //RTCO = 0,计时器输出禁用
}

//判断计时器是否完成计数
uint8 RTC_Check(void)
{
  uint8 timeover;
  timeover = BIT_GET(RTC_SC, RTC_SC_RTIF_SHIFT);
  return timeover;
}
