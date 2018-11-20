/*
  ******************************************************************************
  * @file    port.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.7.25
  * @mail:   blog_xhjiang@163.com
  * @brief   port configure
  * @note    上拉配置通过测试，大电流尚未测试
  ******************************************************************************
  */

#include "KEA128_Port.h"

//端口滤波配置
//脉宽比所选时钟周期更短的毛刺将被滤除；
//脉宽比所选时钟周期长两倍以上的毛刺将不会被滤除，并且会进入内部电路。
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
    PORT_IOFLT0 &= ~(uint32)(3<<24);//先清空
    PORT_IOFLT0 |= (uint32)((iofltdiv-1)<<24);
    if(iofltx == 0)
    {
      PORT_IOFLT0 &= ~(uint32)(3<<(2*iofltn));//先清空
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
    PORT_IOFLT0 &= ~(uint32)(7<<26);//先清空
    PORT_IOFLT0 |= (uint32)((iofltdiv-5)<<26);
    if(iofltx == 0)
    {
      PORT_IOFLT0 &= ~(uint32)(3<<(2*iofltn));//先清空
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
    PORT_IOFLT0 &= ~(uint32)(7<<29);//先清空
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

//上拉配置，内部上拉电阻范围在30-50kΩ,PTA2和PTA3需要外部上拉使用
//不使能上拉时，引脚浮空电压随机，因此配置为GPIO输入时默认上拉配置。
//建议将未使用的引脚上拉
//无返回
void Port_Pue(PTX_n ptx_n,char cfg)
{
  uint8 ptx,ptn;
  ptx = PTX(ptx_n);//记录寄存器序号    //PORT_PUE0,1,2
  ptn = PTn(ptx_n);//记录位号          //0,1,2,3......,32
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

//大电流驱动,标准单个引脚的驱动能力为2.5mA，大电流驱动为10mA的驱动能力。
//仅限H1,H0,E1,E0,D1,D0,B5,B4八个引脚可配置
//成功配置返回1，失败返回0
int Port_Pdrve(PTX_n ptx_n,char cfg)
{
  char ptxns[] = {PTB4, PTB5, PTD0, PTD1, PTE0, PTE1, PTH0, PTH1};

  char i = 0;
  //遍历枚举表...最低效的查找...待优化
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
