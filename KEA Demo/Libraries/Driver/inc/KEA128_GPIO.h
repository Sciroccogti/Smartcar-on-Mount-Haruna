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


typedef enum //ö��IO�˿ڷ���
{
    GPI = 0U, //����ܽ����뷽��      
    GPO = 1U, //����ܽ��������
}GPIO_DR;

#define HIGH 1
#define LOW  0
//�������FGPIO,�뽫GPIOx�޸�ΪFGPIOx
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8λ�˿ڵ�ַ����
//  @param      x               xȡֵ��Χ 0����Ӧϵͳ���ϵ�A0-A7�˸����� 1����Ӧϵͳ���ϵ�B0-B7�˸����� 
//                                        2����Ӧϵͳ���ϵ�C0-C7�˸����� 3����Ӧϵͳ���ϵ�D0-D7�˸�����
//  @since      v2.0
//  Sample usage:               A_PDOR8(2) = 0XFF;   //C0-C7�˸���������ߵ�ƽ
//-------------------------------------------------------------------------------------------------------------------
#define A_PDOR8(x)    *(uint8 *)(GPIOA_BASE+0x00+x) //����A�˿ڵ�8λ�����ַ
#define A_PSOR8(x)    *(uint8 *)(GPIOA_BASE+0x04+x) //����A�˿ڵ�8λ��λ��ַ
#define A_PCOR8(x)    *(uint8 *)(GPIOA_BASE+0x08+x) //����A�˿ڵ�8λ�����ַ
#define A_PTOR8(x)    *(uint8 *)(GPIOA_BASE+0x0C+x) //����A�˿ڵ�8λȡ����ַ
#define A_PDIR8(x)    *(uint8 *)(GPIOA_BASE+0x10+x) //����A�˿ڵ�8λ�����ַ
#define A_PDDR8(x)    *(uint8 *)(GPIOA_BASE+0x14+x) //����A�˿ڵ�8λ�����ַ
#define A_PIDR8(x)    *(uint8 *)(GPIOA_BASE+0x18+x) //����A�˿ڵ�8λ���õ�ַ                                                     
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8λ�˿ڵ�ַ����
//  @param      x               xȡֵ��Χ 0����Ӧϵͳ���ϵ�E0-E7�˸����� 1����Ӧϵͳ���ϵ�F0-F7�˸����� 
//                                        2����Ӧϵͳ���ϵ�G0-G7�˸����� 3����Ӧϵͳ���ϵ�H0-H7�˸�����
//  @since      v2.0
//  Sample usage:               B_PDOR8(3) = 0XFF;   //H0-H7�˸���������ߵ�ƽ
//-------------------------------------------------------------------------------------------------------------------                                          
#define B_PDOR8(x)    *(uint8 *)(GPIOB_BASE+0x00+x) //����B�˿ڵ�8λ�����ַ
#define B_PSOR8(x)    *(uint8 *)(GPIOB_BASE+0x04+x) //����B�˿ڵ�8λ��λ��ַ
#define B_PCOR8(x)    *(uint8 *)(GPIOB_BASE+0x08+x) //����B�˿ڵ�8λ�����ַ
#define B_PTOR8(x)    *(uint8 *)(GPIOB_BASE+0x0C+x) //����B�˿ڵ�8λȡ����ַ
#define B_PDIR8(x)    *(uint8 *)(GPIOB_BASE+0x10+x) //����B�˿ڵ�8λ�����ַ
#define B_PDDR8(x)    *(uint8 *)(GPIOB_BASE+0x14+x) //����B�˿ڵ�8λ�����ַ
#define B_PIDR8(x)    *(uint8 *)(GPIOB_BASE+0x18+x) //����B�˿ڵ�8λ���õ�ַ                              
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8λ�˿ڵ�ַ����
//  @param      x               xȡֵ��Χ 0����Ӧϵͳ���ϵ�I0-I7�˸�����
//  @since      v2.0
//  Sample usage:               B_PDOR8(0) = 0XFF;   //I0-I7�˸���������ߵ�ƽ
//-------------------------------------------------------------------------------------------------------------------                              
#define C_PDOR8(x)    *(uint8 *)(GPIOC_BASE+0x00+x) //����C�˿ڵ�8λ�����ַ
#define C_PSOR8(x)    *(uint8 *)(GPIOC_BASE+0x04+x) //����C�˿ڵ�8λ��λ��ַ
#define C_PCOR8(x)    *(uint8 *)(GPIOC_BASE+0x08+x) //����C�˿ڵ�8λ�����ַ
#define C_PTOR8(x)    *(uint8 *)(GPIOC_BASE+0x0C+x) //����C�˿ڵ�8λȡ����ַ
#define C_PDIR8(x)    *(uint8 *)(GPIOC_BASE+0x10+x) //����C�˿ڵ�8λ�����ַ
#define C_PDDR8(x)    *(uint8 *)(GPIOC_BASE+0x14+x) //����C�˿ڵ�8λ�����ַ
#define C_PIDR8(x)    *(uint8 *)(GPIOC_BASE+0x18+x) //����C�˿ڵ�8λ���õ�ַ


extern void    GPIO_Init(PTX_n ptx_n, GPIO_DR dr, uint8 data);   //��ʼ��gpio
extern void    GPIO_Dir(PTX_n ptx_n, GPIO_DR dr);                //�������ŷ���
extern uint8   GPIO_Get(PTX_n ptx_n);                             //��ȡ����״̬
extern void    GPIO_Set(PTX_n ptx_n, uint8 data);                //��������״̬
extern void    GPIO_Turn(PTX_n ptx_n);                           //��ת����״̬

extern void    FGPIO_Init(PTX_n ptx_n, GPIO_DR dr, uint8 data);   //��ʼ��gpio
extern void    FGPIO_Dir(PTX_n ptx_n, GPIO_DR dr);                //�������ŷ���
extern uint8   FGPIO_Get(PTX_n ptx_n);                             //��ȡ����״̬
extern void    FGPIO_Set(PTX_n ptx_n, uint8 data);                //��������״̬
extern void    FGPIO_Turn(PTX_n ptx_n);                           //��ת����״̬

extern uint8 Key_Pullup_Check(PTX_n ptx_n);                       //��鰴��״̬
extern uint8 Key_Pulldown_Check(PTX_n ptx_n);                     //��鰴��״̬

/* λ������ �ڴ�ƫ�Ƽ��� ��� Cortex-M4 Generic User Guide 2.25 */
/* CM4����2��bitband���� 0x2000_0000-0x200F_FFFF ӳ���� 0x2200_0000-0x23FF_FFFF
                         0x4000_0000-0x4000_FFFF ӳ���� 0x4200_0000-0x43FF_FFFF
   ʵ�ʲ�δʹ�ã�����Ϊ��λ������
*/
#define Pout(PTxn, data)   GPIO_Set(PTxn, data)
#define Pin(PTxn)          GPIO_Get(PTxn)

////nλ����
//ʾ��GPIO_SET_NBIT(4,PTG0,B8(1001));            //PTG0/PTG3���1����LED0/LED3 ��;PTG1/PTG2���0����LED1/LED2 ��
                                                 //B20~B23 ��4bit��B8(1001) = 9����������1001��
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
