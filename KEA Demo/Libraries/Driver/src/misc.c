/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		misc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/



#include "misc.h"



/*!
 *  @brief      �����ж��������ַ
 *  @param      vtor    �µ��ж��������ַ
 *  @since      v5.0
 *  @author     ��˼������˾
 *  Sample usage:       write_vtor ((uint32)__VECTOR_RAM);  //�µ��ж�������ַ
 */
void write_vtor (int vtor)
{
    ASSERT(vtor % 0x200 == 0);   //Vector Table base offset field. This value must be a multiple of 0x200.

    /* Write the VTOR with the new value */
    SCB_VTOR = vtor;
}


