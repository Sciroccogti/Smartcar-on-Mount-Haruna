/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		KEA128_i2c
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#ifndef _KEA128_i2c_h
#define _KEA128_i2c_h


#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"
#include "stdlib.h"

typedef enum
{
    i2c0  = 0,
    i2c1  = 1
} I2Cn_e;

typedef enum MSmode
{
    MWSR =   0x00,  // ����дģʽ  
    MRSW =   0x01   // ������ģʽ  
} MSmode;

//��֧�� I2C����ģʽ
extern uint32  i2c_init(I2Cn_e i2cn, uint32 baud);                               //��ʼ��I2C
extern void    i2c_write_reg(I2Cn_e, uint8 SlaveID, uint8 reg, uint8 Data);      //д�����ݵ��Ĵ���
extern uint8   i2c_read_reg (I2Cn_e, uint8 SlaveID, uint8 reg);                  //��ȡ�Ĵ���������
extern uint8   i2c_read_reg_bytes(I2Cn_e i2cn, uint8 SlaveID, uint8 reg, uint8 num, uint8 * addr);







#endif
