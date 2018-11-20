/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		KEA128_spi
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#include "KEA128_spi.h"

SPI_Type* SPIN[2] = SPI_BASES; //��������ָ�����鱣�� SPIx �ĵ�ַ

#define SPI_TX_WAIT(SPIn)	while( !( SPIN[SPIn]->S & SPI_S_SPTEF_MASK ) )  //�ȴ��������
#define SPI_RX_WAIT(SPIn)   while( !( SPIN[SPIn]->S & SPI_S_SPRF_MASK ) )   //�ȴ���������

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI��ʼ��
//  @param      spin            ѡ��SPIģ��   ��spi0 spi1
//  @param      pcs             ѡ��Ƭѡ����
//  @param      master          ѡ������ģʽ
//  @param      baud            ѡ��ͨ������
//  @return     				������ʵ������
//  @since      v2.0
//  Sample usage:       		uint32 baud = spi_init(spi0,USE_PCS,MASTER,1*1000*1000);              //��ʼ��SPI,����ģʽ, ������Ϊ1M ,������ʵ�����ʵ�baud����
//-------------------------------------------------------------------------------------------------------------------
uint32 spi_init(SPIn_e spin, SPI_PCSn_e pcs, SPI_CFG master,uint32 baud)
{
    uint8 i,j;
    uint8 sppr,spr;
    uint32 Scaler[] = {2,4,8,16,32,64,128,256,512};
    uint8  Prescaler[] = {1,2,3,4,5,6,7,8};
    uint32 min_diff, diff;
    
    if(spi0 == spin)
    {
        SIM->SCGC |= SIM_SCGC_SPI0_MASK;    //����ʱ��
        //���Ÿ���
        if(B2 == SPI0_SCK_PIN)
        {
            SIM->PINSEL &= ~(uint32)SIM_PINSEL_SPI0PS_MASK;
        }
        else
        {
            SIM->PINSEL |= SIM_PINSEL_SPI0PS_MASK;
        }
    }
    else
    {
        SIM->SCGC |= SIM_SCGC_SPI1_MASK;    //����ʱ��
        //���Ÿ���
        if(D0 == SPI1_SCK_PIN)
        {
            SIM->PINSEL1 &= ~(uint32)SIM_PINSEL1_SPI1PS_MASK;
        }
        else
        {
            SIM->PINSEL1 |= SIM_PINSEL1_SPI1PS_MASK;
        }
    }
    //�������ӻ�ģʽ���ù���ģʽ �������Ϊ����ʱ�ӵĶ���֮һ
    if(master == MASTER)
    {
      min_diff = ~0;
      for(i=0; i<9; i++)
      {
        for(j=0; j<8; j++)
        {
          diff = abs(bus_clk_khz*1000/(Prescaler[j]*Scaler[i]) - baud);
          if(min_diff > diff)
          {
            //������Ѳ���
            sppr = j;
            spr = i;
            min_diff = diff;
            
            if(min_diff == 0)	break;//�պ�ƥ��
          }
        }
      }
      
      SPIN[spin]->C1 = (0
                        | (pcs << SPI_C1_SSOE_SHIFT)          //���C2_MODFENλѡ��Ƭѡ���ŵĹ���
                        | SPI_C1_SPE_MASK                     //SPI ϵͳʹ��
                        | SPI_C1_MSTR_MASK                    //SPI ģ������ΪSPI ����
                        //| SPI_C1_SPIE_MASK                  //SPRF ��MODF Ϊ1 ʱ����Ӳ���ж�
                        //| SPI_C1_SPTIE_MASK                 //SPTEF Ϊ1 ʱ������Ӳ���жϡ�
                        //| SPI_C1_CPOL_MASK                  //0���ߵ�ƽ��ЧSPI ʱ�ӣ�����ʱΪ�͵�ƽ�� 1���͵�ƽ��ЧSPI ʱ�ӣ�����ʱΪ�ߵ�ƽ��
                        //| SPI_C1_CPHA_MASK                            //0:SPSCK �ϵĵ�һ�����س��������ݴ���ĵ�һ�����ڵ��м� 1:SPSCK �ϵĵ�һ�����س��������ݴ���ĵ�һ�����ڵĿ�ʼ
                        //| SPI_C1_LSBFE_MASK                 //1:SPI �������ݴ���������Чλ��ʼ
                        );    
      SPIN[spin]->C2 = pcs << SPI_C2_MODFEN_SHIFT;            //���C1_SSOEλѡ��Ƭѡ���ŵĹ���
      SPIN[spin]->BR = SPI_BR_SPR(spr) | SPI_BR_SPPR(sppr);   //���ò����ʲ���
    }
    else{}
    return (bus_clk_khz*1000/Prescaler[sppr]/Scaler[spr]);      //����ʵ�ʲ�����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI���ͽ��պ���
//  @param      spin            ѡ��SPIģ��   ��spi0 spi1
//  @param      modata          ���͵����ݻ�������ַ(����Ҫ������ NULL)
//  @param      midata          ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
//  @param      len             ���͵��ֽ���
//  @return     void				
//  @since      v2.0
//  Sample usage:       		spi_mosi(spi0,buf,buf,1);    //����buff�����ݣ������յ�buf�����Ϊ1�ֽ�
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi(SPIn_e spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len)
{
    while(len)
    {
        SPI_TX_WAIT(spin);
        SPIN[spin]->D = *modata;
        SPI_RX_WAIT(spin);
        if(midata != NULL)  *midata = SPIN[spin]->D;
        else                SPIN[spin]->D;
        len--;
        modata++;
        if(midata != NULL)  midata++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI���ͽ��պ���
//  @param      spin            ѡ��SPIģ��   ��spi0 spi1
//  @param      mocmd           ���͵����������ַ(����Ҫ������ NULL)
//  @param      micmd           ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
//  @param      modata          ���͵����ݻ�������ַ(����Ҫ������ NULL)
//  @param      midata          ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
//  @param      cmdlen          ����������ֽ���
//  @param      len             �������ݵ��ֽ���
//  @return     void				
//  @since      v2.0
//  Sample usage:       		spi_mosi_cmd(spi0,cmd,NULL,buff,buff,1,2);    //����cmd/buff�����ݣ�������cmd����ʱ�����ݣ�����buff����ʱ�����ݵ�buff����ȷֱ�Ϊ1��2�ֽ�
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi_cmd(SPIn_e spin, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len)
{
    spi_mosi(spin,pcs,mocmd,micmd,cmdlen);
    spi_mosi(spin,pcs,modata,midata,len);
}


