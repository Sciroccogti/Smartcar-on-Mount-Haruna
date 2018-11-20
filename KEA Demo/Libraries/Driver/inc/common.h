/*
  ******************************************************************************
  * @file    common.h
  * @author  xhJiang
  * @version V1.0
  * @date    2018.3.31
  * @mail:   blog_xhjiang@163.com
  * @brief   predefine
  ******************************************************************************
  */

 
 
#ifndef COMMON_H_
#define COMMON_H_

//数据类型定义
typedef unsigned char       uint8;   //  8 bits 
typedef unsigned short int  uint16;  // 16 bits 
typedef unsigned long int   uint32;  // 32 bits 
typedef unsigned long long  uint64;  // 64 bits 

typedef unsigned int        uint;

typedef char                int8;    //  8 bits 
typedef short int           int16;   // 16 bits 
typedef long  int           int32;   // 32 bits 
typedef long  long          int64;   // 64 bits 

typedef volatile int8       vint8;   //  8 bits 
typedef volatile int16      vint16;  // 16 bits 
typedef volatile int32      vint32;  // 32 bits 
typedef volatile int64      vint64;  // 64 bits 

typedef volatile uint8      vuint8;  //  8 bits 
typedef volatile uint16     vuint16; // 16 bits 
typedef volatile uint32     vuint32; // 32 bits 
typedef volatile uint64     vuint64; // 64 bits 

typedef void(*pt2Func)(void);		  /* Pointer to Functions, void argument */
typedef void(*pt2FuncU8)(uint8);	  /* Pointer to Functions, UINT8 argument */

// Compiler Related Definitions 
#ifdef __CC_ARM                         // ARM Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#elif defined (__IAR_SYSTEMS_ICC__)     // for IAR Compiler 
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
#elif defined (__GNUC__)                // GNU GCC Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#endif // Compiler Related Definitions 

/**
*  @brief 变量的位清0和置1
*/
#define BIT_CLEAN(var,n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var,n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var,n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）
/**
*  @brief 仿二进制赋值，二进制转十进制
*/
#define  HEX__(n)   0x##n##UL
#define  B8__(x)   ( (x & 0x0000000FUL) ? 1:0 )\
    +( (x & 0x000000F0UL) ? 2:0 )\
    +( (x & 0x00000F00UL) ? 4:0 )\
    +( (x & 0x0000F000UL) ? 8:0 )\
    +( (x & 0x000F0000UL) ? 16:0 )\
    +( (x & 0x00F00000UL) ? 32:0 )\
    +( (x & 0x0F000000UL) ? 64:0 )\
    +( (x & 0xF0000000UL) ? 128:0 )
#define  B8(x)                                     ((unsigned char)B8__(HEX__(x)))
#define  B16(x_msb,x_lsb)                (((unsigned int)B8(x_msb)<<8) + B8(x_lsb))
#define  B32(x_msb,x_2,x_3,x_lsb)   (((unsigned long)B8(x_msb)<<24) + ((unsigned long)B8(x_2)<<16) + ((unsigned long)B8(x_3)<<8) + B8(x_lsb))
/* Sample usage:
  B8(01010101) = 85
  B16(10101010,01010101) = 43605
  B32(10000000,11111111,10101010,01010101) = 2164238933
*/


/*
 * 配置断言和其实现函数
 */
void assert_failed(char *, int);

#if defined( DEBUG_MODE )
#define ASSERT(expr) \
    if (!(expr)) \
        assert_failed(__FILE__, __LINE__)
#else
#define ASSERT(expr)
#endif

/*
 * 配置调试输出函数
 */
#if( defined(DEBUG_MODE) && defined(DEBUG_PRINT))
#define DEBUG_PRINTF(FORMAT,...)        do{printf(FORMAT,##__VA_ARGS__);}while(0)	/*无需打印调试信息时，请将宏内容注释掉*/
#else
#define DEBUG_PRINTF(FORMAT,...)
#endif


typedef enum //端口号枚举
{
    //  A 
    A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,
    //  B
    B0,  B1,  B2,  B3,  B4,  B5,  B6,  B7,
    //  C
    C0,  C1,  C2,  C3,  C4,  C5,  C6,  C7,
    //  D
    D0,  D1,  D2,  D3,  D4,  D5,  D6,  D7,
    //  E
    E0,  E1,  E2,  E3,  E4,  E5,  E6,  E7,
    //  F
    F0,  F1,  F2,  F3,  F4,  F5,  F6,  F7,
    //  G
    G0,  G1,  G2,  G3,  G4,  G5,  G6,  G7,
    //  H
    H0,  H1,  H2,  H3,  H4,  H5,  H6,  H7,
    //  I,I7 Reserved     
    I0,  I1,  I2,  I3,  I4,  I5,  I6,  I7,
    //  Reserved
    Xn_MAX
} PTX_n;


#define PTX(PTxn)  (PTxn >> 5)  //寄存器号A,B,C,D,PTX_n/32
#define PTn(PTxn)  (PTxn & 0x1f)//引脚号1,2,3,PTX_n%32
#define GPIOX_BASE(PTxn)     GPIOX[PTX(PTxn)]       //PORT模块的地址
////上列部分，和下面是有所不同的，专门针对PORT、GPIO寄存器
////根据以下的定义，可以得出：PTx = PTxn / 8 ; PTn = PTxn & 7
//#define PTX(PTxn)           ((PTxn)>>3)
//#define PTn(PTxn)           ((PTxn)&0x7)
//#define PORTX_BASE(PTxn)     PORTX[PTX(PTxn)]       //PORT模块的地址
//----------------------------------------------------------------------重命名PTXn
#define PTA0 A0
#define PTA1 A1
#define PTA2 A2
#define PTA3 A3
#define PTA4 A4
#define PTA5 A5
#define PTA6 A6
#define PTA7 A7

#define PTB0 B0
#define PTB1 B1
#define PTB2 B2
#define PTB3 B3
#define PTB4 B4
#define PTB5 B5
#define PTB6 B6
#define PTB7 B7

#define PTC0 C0
#define PTC1 C1
#define PTC2 C2
#define PTC3 C3
#define PTC4 C4
#define PTC5 C5
#define PTC6 C6
#define PTC7 C7

#define PTD0 D0
#define PTD1 D1
#define PTD2 D2
#define PTD3 D3
#define PTD4 D4
#define PTD5 D5
#define PTD6 D6
#define PTD7 D7

#define PTE0 E0
#define PTE1 E1
#define PTE2 E2
#define PTE3 E3
#define PTE4 E4
#define PTE5 E5
#define PTE6 E6
#define PTE7 E7

#define PTF0 F0
#define PTF1 F1
#define PTF2 F2
#define PTF3 F3
#define PTF4 F4
#define PTF5 F5
#define PTF6 F6
#define PTF7 F7

#define PTG0 G0
#define PTG1 G1
#define PTG2 G2
#define PTG3 G3
#define PTG4 G4
#define PTG5 G5
#define PTG6 G6
#define PTG7 G7

#define PTH0 H0
#define PTH1 H1
#define PTH2 H2
#define PTH3 H3
#define PTH4 H4
#define PTH5 H5
#define PTH6 H6
#define PTH7 H7

#define PTI0 I0
#define PTI1 I1
#define PTI2 I2
#define PTI3 I3
#define PTI4 I4
#define PTI5 I5
#define PTI6 I6
#define PTI7 I7
//--------------------------------------------------------------------------
//------------------------------------------------------------------重命名PXn
#define PA0 A0
#define PA1 A1
#define PA2 A2
#define PA3 A3
#define PA4 A4
#define PA5 A5
#define PA6 A6
#define PA7 A7

#define PB0 B0
#define PB1 B1
#define PB2 B2
#define PB3 B3
#define PB4 B4
#define PB5 B5
#define PB6 B6
#define PB7 B7

#define PC0 C0
#define PC1 C1
#define PC2 C2
#define PC3 C3
#define PC4 C4
#define PC5 C5
#define PC6 C6
#define PC7 C7

#define PD0 D0
#define PD1 D1
#define PD2 D2
#define PD3 D3
#define PD4 D4
#define PD5 D5
#define PD6 D6
#define PD7 D7

#define PE0 E0
#define PE1 E1
#define PE2 E2
#define PE3 E3
#define PE4 E4
#define PE5 E5
#define PE6 E6
#define PE7 E7

#define PF0 F0
#define PF1 F1
#define PF2 F2
#define PF3 F3
#define PF4 F4
#define PF5 F5
#define PF6 F6
#define PF7 F7

#define PG0 G0
#define PG1 G1
#define PG2 G2
#define PG3 G3
#define PG4 G4
#define PG5 G5
#define PG6 G6
#define PG7 G7

#define PH0 H0
#define PH1 H1
#define PH2 H2
#define PH3 H3
#define PH4 H4
#define PH5 H5
#define PH6 H6
#define PH7 H7

#define PI0 I0
#define PI1 I1
#define PI2 I2
#define PI3 I3
#define PI4 I4
#define PI5 I5
#define PI6 I6
#define PI7 I7
//---------------------------------------------------

#define DISABLE 0
#define ENABLE  1

#define TRUE    1
#define FALSE   0

#define KEY_DOWN 0      //按键按下时对应电平
#define KEY_UP   1      //按键弹起时对应电平


//**********************************************
//include all header files
//**********************************************
#include "headerfile.h"




#endif
