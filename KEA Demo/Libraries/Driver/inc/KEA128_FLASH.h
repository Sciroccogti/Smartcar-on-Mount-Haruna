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

//Flash读写期间禁止复位!禁止中断!
//注:第二个扇区的前16字节为Flash配置字段，不允许使用Flash擦写
//
//总共128KB,分为256个扇区,每个扇区512字节
#define SECTOR_SIZE     	(512)                   //512字节
#define FLASH_SECTOR_NUM        (256)                   //扇区数
#define FLASH_ALIGN_ADDR        4                       //地址对齐整数倍
typedef uint32                  FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型




//-------------------------------------------------------------------------------------------------------------------
//  @brief      使用宏定义对flash进行数据读取
//  @param      SectorNum 		需要写入的扇区编号
//  @param      offset	 		地址偏移
//  @param      type		 	读取的数据类型
//  @note                               只能按字节读取，读取时间为一个总线周期
//  @return     			返回给定地址的数据
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
#define     flash_read(SectorNum,offset,type)        (*(type *)((uint32)(((SectorNum)*SECTOR_SIZE) + (offset))))

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH扇区大小
//  @return     				返回一个扇区的大小
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
