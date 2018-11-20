/*
  ******************************************************************************
  * @file    flash.h
  * @author  xhJiang
  * @version v1.1
  * @date    2018.6.5
  * @mail:   blog_xhjiang@163.com
  * @brief   FLASH configure
  ******************************************************************************
  */
#ifndef _KEA128_FLASH_H
#define _KEA128_FLASH_H

#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"

//Flash��д�ڼ��ֹ��λ!��ֹ�ж�!
//ע:�ڶ���������ǰ16�ֽ�ΪFlash�����ֶΣ�������ʹ��Flash��д
//
//�ܹ�128KB,��Ϊ256������,ÿ������512�ֽ�
#define SECTOR_SIZE     	(512)                   //512�ֽ�
#define FLASH_SECTOR_NUM        (256)                   //������
#define FLASH_ALIGN_ADDR        4                       //��ַ����������
typedef uint32                  FLASH_WRITE_TYPE;       //flash_write ����д�� ����������




//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ�ú궨���flash�������ݶ�ȡ
//  @param      SectorNum 		��Ҫд����������
//  @param      offset	 		��ַƫ��
//  @param      type		 	��ȡ����������
//  @note                               ֻ�ܰ��ֽڶ�ȡ����ȡʱ��Ϊһ����������
//  @return     			���ظ�����ַ������
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//��ȡ20������ƫ��0��������Ϊuint32
//-------------------------------------------------------------------------------------------------------------------
#define     flash_read(SectorNum,offset,type)        (*(type *)((uint32)(((SectorNum)*SECTOR_SIZE) + (offset))))

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH������С
//  @return     				����һ�������Ĵ�С
//  @since      v1.0
//  Sample usage:               uint32 dat = FLASH_GetSectorSize();
//-------------------------------------------------------------------------------------------------------------------
#define     FLASH_GetSectorSize()                     (uint32)SECTOR_SIZE

void FLASH_Init(void);

uint8 FLASH_EraseSector(uint32 SectorNum);
uint8 FLASH_WriteSector(uint32 SectorNum, const uint8 *buf, uint32 len, uint32 offset);
void FLASH_WriteFloat(uint32 SectorNum,const float data, uint32 offset);
void FLASH_WriteDouble(uint32 SectorNum,const double data, uint32 offset);
double flash_read_double(uint32 SectorNum,uint32 offset);

#endif
