/*
  ******************************************************************************
  * @file    port.h
  * @author  xhJiang
  * @version V1.0
  * @date    2018.3.31
  * @mail:   blog_xhjiang@163.com
  * @brief   port configure
  ******************************************************************************
  */



#ifndef KEA128_PORT_H_
#define KEA128_PORT_H_

#include "common.h"

typedef enum //ö�ٶ˿��˲�ģ��
{
  FLTA = 0U,//���PTA ������˲���ѡ��
  FLTB,
  FLTC,
  FLTD,
  FLTE,
  FLTF,
  FLTG,
  FLTH,
  FLTRST,//���RESET/IRQ ������˲���ѡ��
  FLTKBI0,//���KBI0 ������˲���ѡ��
  FLTKBI1,
  FLTNMI,//���NMI ������˲���ѡ��
  //-----------------������PORT_IOFLT0����
  FLTI,
  FLTRESERVED,//����
  FLTIRQ,//���IRQ ������˲���ѡ��
  FLTFTM0,//���FTM0CH0/FTM0CH1 ������˲���ѡ��
  FLTFTM1,
  FLTPWT,//���PWT_IN1/PWT_IN0 ������˲���ѡ��
  FLTI2C0,//���SCL0/SDA0 ������˲���ѡ��
  FLTI2C1,
  //-----------------������PORT_IOFLT1����
}PORT_IOFLT;

typedef enum //ö�ٶ˿��˲���Ƶ��
{
  FLTDIV1 = 0,//����BUSCLK����Ƶ
  //-------------------
  FLTDIV2,//����2��Ƶ
  FLTDIV4,
  FLTDIV8,
  FLTDIV16,
  //-------------FLTDIV1
  FLTDIV32,
  FLTDIV64,
  FLTDIV128,
  FLTDIV256,
  FLTDIV512,
  FLTDIV1024,
  FLTDIV2048,
  FLTDIV4096,
  //-----------FLTDIV2
  FLTLPO1,//LPOCLK����Ƶ
  FLTLPO2,
  FLTLPO4,
  FLTLPO8,
  FLTLPO16,
  FLTLPO32,
  FLTLPO64,
  FLTLPO128,
  //-----------FLTDIV3
  
}PORT_IOFLTDIV;


//�˿��˲�����
extern void Port_Ioflt(PORT_IOFLT ioflt,PORT_IOFLTDIV iofltdiv);
//��������
extern void Port_Pue(PTX_n ptx_n,char cfg);
//�������������
extern int Port_Pdrve(PTX_n ptx_n,char cfg);

#endif
