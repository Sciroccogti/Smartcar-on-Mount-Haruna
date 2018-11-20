/*
  ******************************************************************************
  * @file    gpio.h
  * @author  xhJiang
  * @version V1.1
  * @date    2018.3.31
  * @mail:   blog_xhjiang@163.com
  * @brief   gpio configure
  ******************************************************************************
  */



#ifndef KEA128_GPIO_H_
#define KEA128_GPIO_H_


#include "common.h"


typedef enum //枚举IO端口方向
{
    GPI = 0U, //定义管脚输入方向      
    GPO = 1U, //定义管脚输出方向
}GPIO_DR;

#define HIGH 1
#define LOW  0
//如果采用FGPIO,请将GPIOx修改为FGPIOx
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8位端口地址定义
//  @param      x               x取值范围 0：对应系统板上的A0-A7八个引脚 1：对应系统板上的B0-B7八个引脚 
//                                        2：对应系统板上的C0-C7八个引脚 3：对应系统板上的D0-D7八个引脚
//  @since      v2.0
//  Sample usage:               A_PDOR8(2) = 0XFF;   //C0-C7八个引脚输出高电平
//-------------------------------------------------------------------------------------------------------------------
#define A_PDOR8(x)    *(uint8 *)(GPIOA_BASE+0x00+x) //定义A端口的8位输出地址
#define A_PSOR8(x)    *(uint8 *)(GPIOA_BASE+0x04+x) //定义A端口的8位置位地址
#define A_PCOR8(x)    *(uint8 *)(GPIOA_BASE+0x08+x) //定义A端口的8位清零地址
#define A_PTOR8(x)    *(uint8 *)(GPIOA_BASE+0x0C+x) //定义A端口的8位取反地址
#define A_PDIR8(x)    *(uint8 *)(GPIOA_BASE+0x10+x) //定义A端口的8位输入地址
#define A_PDDR8(x)    *(uint8 *)(GPIOA_BASE+0x14+x) //定义A端口的8位方向地址
#define A_PIDR8(x)    *(uint8 *)(GPIOA_BASE+0x18+x) //定义A端口的8位禁用地址                                                     
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8位端口地址定义
//  @param      x               x取值范围 0：对应系统板上的E0-E7八个引脚 1：对应系统板上的F0-F7八个引脚 
//                                        2：对应系统板上的G0-G7八个引脚 3：对应系统板上的H0-H7八个引脚
//  @since      v2.0
//  Sample usage:               B_PDOR8(3) = 0XFF;   //H0-H7八个引脚输出高电平
//-------------------------------------------------------------------------------------------------------------------                                          
#define B_PDOR8(x)    *(uint8 *)(GPIOB_BASE+0x00+x) //定义B端口的8位输出地址
#define B_PSOR8(x)    *(uint8 *)(GPIOB_BASE+0x04+x) //定义B端口的8位置位地址
#define B_PCOR8(x)    *(uint8 *)(GPIOB_BASE+0x08+x) //定义B端口的8位清零地址
#define B_PTOR8(x)    *(uint8 *)(GPIOB_BASE+0x0C+x) //定义B端口的8位取反地址
#define B_PDIR8(x)    *(uint8 *)(GPIOB_BASE+0x10+x) //定义B端口的8位输入地址
#define B_PDDR8(x)    *(uint8 *)(GPIOB_BASE+0x14+x) //定义B端口的8位方向地址
#define B_PIDR8(x)    *(uint8 *)(GPIOB_BASE+0x18+x) //定义B端口的8位禁用地址                              
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8位端口地址定义
//  @param      x               x取值范围 0：对应系统板上的I0-I7八个引脚
//  @since      v2.0
//  Sample usage:               B_PDOR8(0) = 0XFF;   //I0-I7八个引脚输出高电平
//-------------------------------------------------------------------------------------------------------------------                              
#define C_PDOR8(x)    *(uint8 *)(GPIOC_BASE+0x00+x) //定义C端口的8位输出地址
#define C_PSOR8(x)    *(uint8 *)(GPIOC_BASE+0x04+x) //定义C端口的8位置位地址
#define C_PCOR8(x)    *(uint8 *)(GPIOC_BASE+0x08+x) //定义C端口的8位清零地址
#define C_PTOR8(x)    *(uint8 *)(GPIOC_BASE+0x0C+x) //定义C端口的8位取反地址
#define C_PDIR8(x)    *(uint8 *)(GPIOC_BASE+0x10+x) //定义C端口的8位输入地址
#define C_PDDR8(x)    *(uint8 *)(GPIOC_BASE+0x14+x) //定义C端口的8位方向地址
#define C_PIDR8(x)    *(uint8 *)(GPIOC_BASE+0x18+x) //定义C端口的8位禁用地址


extern void    GPIO_Init(PTX_n ptx_n, GPIO_DR dr, uint8 data);   //初始化gpio
extern void    GPIO_Dir(PTX_n ptx_n, GPIO_DR dr);                //设置引脚方向
extern uint8   GPIO_Get(PTX_n ptx_n);                             //获取引脚状态
extern void    GPIO_Set(PTX_n ptx_n, uint8 data);                //设置引脚状态
extern void    GPIO_Turn(PTX_n ptx_n);                           //翻转引脚状态

extern void    FGPIO_Init(PTX_n ptx_n, GPIO_DR dr, uint8 data);   //初始化gpio
extern void    FGPIO_Dir(PTX_n ptx_n, GPIO_DR dr);                //设置引脚方向
extern uint8   FGPIO_Get(PTX_n ptx_n);                             //获取引脚状态
extern void    FGPIO_Set(PTX_n ptx_n, uint8 data);                //设置引脚状态
extern void    FGPIO_Turn(PTX_n ptx_n);                           //翻转引脚状态

extern uint8 Key_Pullup_Check(PTX_n ptx_n);                       //检查按键状态
extern uint8 Key_Pulldown_Check(PTX_n ptx_n);                     //检查按键状态

/* 位带操作 内存偏移计算 详见 Cortex-M4 Generic User Guide 2.25 */
/* CM4中有2块bitband区域 0x2000_0000-0x200F_FFFF 映射至 0x2200_0000-0x23FF_FFFF
                         0x4000_0000-0x4000_FFFF 映射至 0x4200_0000-0x43FF_FFFF
   实际并未使用，以下为仿位带操作
*/
#define Pout(PTxn, data)   GPIO_Set(PTxn, data)
#define Pin(PTxn)          GPIO_Get(PTxn)

////n位操作
//示例GPIO_SET_NBIT(4,PTG0,B8(1001));            //PTG0/PTG3输出1，即LED0/LED3 灭;PTG1/PTG2输出0，即LED1/LED2 亮
                                                 //B20~B23 共4bit，B8(1001) = 9，即二进制1001。
//#define GPIO_SET_NBIT(NBIT,PTxn,data)   GPIO_PDOR_REG(GPIOX_BASE(PTxn)) =   (                                                   \
//                                                                                (                                               \
//                                                                                    GPIO_PDOR_REG(GPIOX_BASE(PTxn))             \
//                                                                                    &                                           \
//                                                                                    ((uint32)( ~(((1<<NBIT)-1)<<PTn(PTxn))))    \
//                                                                                )                                               \
//                                                                                |   ( ((data)&( (1<<(NBIT))-1))<<PTn(PTxn) )    \
//                                                                            )
//
//
//#define GPIO_DDR_NBIT(NBIT,PTxn,ddr)   GPIO_PDDR_REG(GPIOX_BASE(PTxn))  =   (                                                   \
//                                                                                (                                               \
//                                                                                    GPIO_PDDR_REG(GPIOX_BASE(PTxn))             \
//                                                                                    &                                           \
//                                                                                    ((uint32)( ~(((1<<(NBIT))-1)<<PTn(PTxn))))  \
//                                                                                )                                               \
//                                                                                |   ( ( (ddr) &  ( (1<<(NBIT))-1))<<PTn(PTxn) ) \
//                                                                            )
//
//#define GPIO_T_NBIT(NBIT,PTxn,data)   GPIO_PTOR_REG(GPIOX_BASE(PTxn))  =   (                                                    \
//                                                                                (                                               \
//                                                                                    GPIO_PTOR_REG(GPIOX_BASE(PTxn))             \
//                                                                                    &                                           \
//                                                                                    ((uint32)( ~(((1<<NBIT)-1)<<PTn(PTxn))))    \
//                                                                                )                                               \
//                                                                                |   ( ((data)&( (1<<(NBIT))-1))<<PTn(PTxn) )    \
//                                                                            )
//
//
//#define GPIO_GET_NBIT(NBIT,PTxn)    (( GPIO_PDIR_REG(GPIOX_BASE(PTxn))>>PTn(PTxn) ) & ((1<<NBIT)-1))

#endif
