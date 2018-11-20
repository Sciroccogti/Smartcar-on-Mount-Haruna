/*
  ******************************************************************************
  * @file    rtc.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.5.13
  * @mail:   blog_xhjiang@163.com
  * @brief   rtc configure,�ݲ�����
  ******************************************************************************
  */


#include "common.h"
#include "KEA128_RTC.h"

//���ö�ʱ��ʱ��
void RTC_Timeset(uint16 timeset)
{
  RTC_BASE_PTR->MOD = timeset;
}

//����100ms���ڶ�ʱ��
void RTC_Init(void)
{
  SIM->SCGC  |= SIM_SCGC_RTC_MASK;//����ʱ��
  RTC_SC = (0
           |RTC_SC_RTCLKS(1)      //ѡ��1KHzʱ��
           |RTC_SC_RTCPS(6)       //ѡ��100��Ƶ
           |RTC_SC_RTIF_MASK);    //���ж�����
                                  //RTIE = 0,�жϽ���
                                  //RTCO = 0,��ʱ���������

}

//��ȡRTC��ʱ����ֵ
uint16 RTC_Read(void)
{
  uint16 temp;
  uint16 count;
  uint8 counth;
  uint8 countl;
  temp = RTC_CNT;
  counth = temp;//�ȶ�ȡCNT[7:0]
  countl = temp >>8;//���ȡCNT[15:8]
  count = (counth << 8) + countl;
  return count;
}

void RTC_Clear(void)
{
  //�رշ�Ƶʱ�ӻ���ռ���
  RTC_SC &= (0
             |RTC_SC_RTCLKS(1)
             |RTC_SC_RTIF_MASK);
  //�ٴο�����Ƶ����
  RTC_SC = (0
           |RTC_SC_RTCLKS(1)      //ѡ��1KHzʱ��
           |RTC_SC_RTCPS(6)       //ѡ��100��Ƶ
           |RTC_SC_RTIF_MASK);    //���ж�����
                                  //RTIE = 0,�жϽ���
                                  //RTCO = 0,��ʱ���������
}

//�жϼ�ʱ���Ƿ���ɼ���
uint8 RTC_Check(void)
{
  uint8 timeover;
  timeover = BIT_GET(RTC_SC, RTC_SC_RTIF_SHIFT);
  return timeover;
}
