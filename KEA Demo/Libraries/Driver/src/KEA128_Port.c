/*
  ******************************************************************************
  * @file    port.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.7.25
  * @mail:   blog_xhjiang@163.com
  * @brief   port configure
  * @note    ��������ͨ�����ԣ��������δ����
  ******************************************************************************
  */

#include "KEA128_Port.h"

//�˿��˲�����
//�������ѡʱ�����ڸ��̵�ë�̽����˳���
//�������ѡʱ�����ڳ��������ϵ�ë�̽����ᱻ�˳������һ�����ڲ���·��
void Port_Ioflt(PORT_IOFLT ioflt,PORT_IOFLTDIV iofltdiv)
{
  uint8 iofltx,iofltn;
  iofltx = (uint8)ioflt/24;
  iofltn = (uint8)ioflt-iofltx*24;
  
  if(iofltdiv == FLTDIV1)
  {
    if(iofltx == 0)
    {
      PORT_IOFLT0 &= ~(uint32)(3<<(2*iofltn));
    }
    else
    {
      PORT_IOFLT1 &= ~(uint32)(3<<(2*iofltn));
    }
  }
  else if((iofltdiv>=FLTDIV2) && (iofltdiv<=FLTDIV16))
  {
    PORT_IOFLT0 &= ~(uint32)(3<<24);//�����
    PORT_IOFLT0 |= (uint32)((iofltdiv-1)<<24);
    if(iofltx == 0)
    {
      PORT_IOFLT0 &= ~(uint32)(3<<(2*iofltn));//�����
      PORT_IOFLT0 |= (uint32)(1<<(2*iofltn));
    }
    else
    {
      PORT_IOFLT1 &= ~(uint32)(3<<(2*iofltn));
      PORT_IOFLT1 |= (uint32)(1<<(2*iofltn));
    }
  }
  else if((iofltdiv>=FLTDIV32) && (iofltdiv<=FLTDIV4096))
  {
    PORT_IOFLT0 &= ~(uint32)(7<<26);//�����
    PORT_IOFLT0 |= (uint32)((iofltdiv-5)<<26);
    if(iofltx == 0)
    {
      PORT_IOFLT0 &= ~(uint32)(3<<(2*iofltn));//�����
      PORT_IOFLT0 |= (uint32)(2<<(2*iofltn));
    }
    else
    {
      PORT_IOFLT1 &= ~(uint32)(3<<(2*iofltn));
      PORT_IOFLT1 |= (uint32)(2<<(2*iofltn));
    }
  }
  else if((iofltdiv>=FLTLPO1) && (iofltdiv<=FLTLPO128))
  {
    PORT_IOFLT0 &= ~(uint32)(7<<29);//�����
    PORT_IOFLT0 |= (uint32)((iofltdiv-13)<<29);
    if(iofltx == 0)
    {
      PORT_IOFLT0 |= (uint32)(3<<(2*iofltn));
    }
    else
    {
      PORT_IOFLT1 |= (uint32)(3<<(2*iofltn));
    }
  }
}

//�������ã��ڲ��������跶Χ��30-50k��,PTA2��PTA3��Ҫ�ⲿ����ʹ��
//��ʹ������ʱ�����Ÿ��յ�ѹ������������ΪGPIO����ʱĬ���������á�
//���齫δʹ�õ���������
//�޷���
void Port_Pue(PTX_n ptx_n,char cfg)
{
  uint8 ptx,ptn;
  ptx = PTX(ptx_n);//��¼�Ĵ������    //PORT_PUE0,1,2
  ptn = PTn(ptx_n);//��¼λ��          //0,1,2,3......,32
  uint32_t volatile *port_pue[] = {&PORT_PUE0, &PORT_PUE1, &PORT_PUE2};
  if(cfg == DISABLE)
  {
      *port_pue[ptx] &= ~(1 << ptn);
  }
  else if(cfg == ENABLE)
  {
      *port_pue[ptx] |= (1 << ptn);
  }
}

//���������,��׼�������ŵ���������Ϊ2.5mA�����������Ϊ10mA������������
//����H1,H0,E1,E0,D1,D0,B5,B4�˸����ſ�����
//�ɹ����÷���1��ʧ�ܷ���0
int Port_Pdrve(PTX_n ptx_n,char cfg)
{
  char ptxns[] = {PTB4, PTB5, PTD0, PTD1, PTE0, PTE1, PTH0, PTH1};

  char i = 0;
  //����ö�ٱ�...���Ч�Ĳ���...���Ż�
  for(i = 0; i < sizeof(ptxns); i++)
  {
    if(ptxns[i] == ptx_n)
    {
      if(cfg == DISABLE)
      {
          PORT_HDRVE &= ~(1 << i);
      }
      else if(cfg == ENABLE)
      {
          PORT_HDRVE |= (1 << i);
      }
      return TRUE;
    }
  }
  return FALSE;
}
