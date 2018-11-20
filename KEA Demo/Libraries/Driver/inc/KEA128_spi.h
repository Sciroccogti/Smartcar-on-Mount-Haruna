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


#ifndef _KEA128_spi_h
#define _KEA128_spi_h


#include <math.h>      //c���Եĸ���������ֵ������(c++�ĸ�������������������)
#include <stdlib.h>    //c���Ե�����������ֵ 
#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"


//���ӻ�ģʽ

typedef enum
{
    MASTER,    //����ģʽ
    SLAVE      //�ӻ�ģʽ
} SPI_CFG;


//ģ���
typedef enum
{
    spi0,
    spi1,
} SPIn_e;


//SPIģ��Ƭѡ��
typedef enum
{
    NOT_PCS,
    USE_PCS,
} SPI_PCSn_e;


uint32 spi_init(SPIn_e spin , SPI_PCSn_e pcs, SPI_CFG master,uint32 baud);
void spi_mosi(SPIn_e spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len);
void spi_mosi_cmd(SPIn_e spin, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len);

#endif 
