/*
  ******************************************************************************
  * @file    uart.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.5.6
  * @mail:   blog_xhjiang@163.com
  * @brief   UART configure
  ******************************************************************************
  */

#include "common.h"
#include "KEA128_UART.h"

/**********************************************************************************************
* Global variables
**********************************************************************************************/
pt2FuncU8 UART_Callback;

UART_MemMapPtr uart[3] = UART_BASE_PTRS;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART管脚复用
//  @param      uartn            选择模块
//  @param      ptx_n           选择引脚号
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Pinmux(uart0, RXTX_B0B1);		//ftm0模块，0通道，A0选择
//  @note       如果UART0的A2A3无法使用,请尝试上拉A2、A3引脚
//-------------------------------------------------------------------------------------------------------------------
void UART_Pinmux(UARTn uartn, PTX_n ptx_n)
{
    switch(uartn)
    {
        case uart0:
        {
            SIM_BASE_PTR ->SCGC |= SIM_SCGC_UART0_MASK;               //使能串口0时钟
            if(ptx_n == RXTX_B0B1)
            {
                SIM_BASE_PTR->PINSEL &= ~SIM_PINSEL_UART0PS_MASK;
            }
            else if(ptx_n == RXTX_A2A3)
            {
                SIM_BASE_PTR->PINSEL |= SIM_PINSEL_UART0PS_MASK;
            }
        }break;
        
        case uart1:
        {
            SIM_BASE_PTR->SCGC |= SIM_SCGC_UART1_MASK;               //使能串口0时钟
            if(ptx_n == RXTX_C6C7) 
            {
                SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_UART1PS_MASK;
            }
            else if (ptx_n == RXTX_F2F3)
            {
                SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_UART1PS_MASK;
            }
        }break;
        
        case uart2:
        {
            SIM_BASE_PTR->SCGC |= SIM_SCGC_UART2_MASK;               //使能串口0时钟
            if(ptx_n == RXTX_D6D7)
            {
                SIM_BASE_PTR->PINSEL1 &= ~SIM_PINSEL1_UART2PS_MASK;
            }
            else if(ptx_n == RXTX_I0I1)
            {
                SIM_BASE_PTR->PINSEL1 |= SIM_PINSEL1_UART2PS_MASK;
            }
        }break;
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      uart 初始化
//  @param      uartn           选择串口(uart0,uart1,uart2)
//  @param      baud            串口波特率
//  @return     uint32          实际设置得波特率
//  @since      v1.0
//  Sample usage:               UART_Init(uart0,9600,RXTX_B0B1);   //初始化串口1为1位起始位、8位数据位、1位停止位、波特率9600
//  @note                       由于KEA128没有波特率微调寄存器，因此在设置得波特率较高得情况下误差比较大，因此返回实际波特率用来校验
//-------------------------------------------------------------------------------------------------------------------
uint32 UART_Init(UARTn uartn, uint32 baud, PTX_n ptx_n)
{
    vuint32 uart_input_clk;
    uint32 sbr;
    
    UART_Pinmux(uartn, ptx_n);
    //设置的时候，应该禁止发送和接收
    uart[uartn]->C2 &= ~(0
                         | UART_C2_TE_MASK
                         | UART_C2_RE_MASK);    
    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    uart[uartn]->C1 |= (0
                        //| UART_C1_M_MASK                      //9 位或 8 位模式选择 : 0 为 8位 ，1 为 9位（注释了表示0，即8位） （如果是9位，位8在UARTx_C3里）
                        //| UART_C1_PE_MASK                     //奇偶校验使能（注释了表示禁用）
                        //| UART_C1_PT_MASK                     //校验位类型 : 0 为 偶校验 ，1 为 奇校验
                       );
    
    //计算波特率
    uart_input_clk = bus_clk_khz * 1000;   //bus时钟

    //UART 波特率 = UART 模块时钟 / (16 × SBR[12:0])
    //SBR = UART 模块时钟 / (16 * UART 波特率)
    sbr = ((uart_input_clk>>4)*10 / baud + 5)/10;               //四舍五入
    if(sbr > 0x1FFF)sbr = 0x1FFF;                               //SBR 是 13bit，最大为 0x1FFF

    //写 SBR
    uart[uartn]->BDH &= ~UART_BDH_SBR_MASK;                     //清除原来波特率
    uart[uartn]->BDH |= UART_BDH_SBR(sbr>>8);                   //先写入SBR高位
    uart[uartn]->BDL  = UART_BDL_SBR((uint8)sbr);               //再写入SBR低位

    // 允许发送和接收
    uart[uartn]->C2 |= (0
                        | UART_C2_TE_MASK                       //发送使能
                        | UART_C2_RE_MASK                       //接收使能
                        //| UART_C2_TIE_MASK                    //发送中断或DMA传输请求使能（注释了表示禁用）
                        //| UART_C2_TCIE_MASK                   //发送完成中断使能（注释了表示禁用）
                        //| UART_C2_RIE_MASK                    //接收满中断或DMA传输请求使能（注释了表示禁用）
                        );
    //由于KEA128没有波特率微调寄存器，因此在设置得波特率较高得情况下误差比较大，因此返回实际波特率用来校验
    return ((uart_input_clk>>4)/sbr);                           //返回实际波特率
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送一个字节
//  @param      uartn           选择串口
//  @param      ch              要发送的字符，UART为8位寄存器，每次只能写入一个字节数据
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Putchar(uart3,0x5a);   
//      或者                    uint8 data;
//                              UART_Putchar(uart0,data);
//-------------------------------------------------------------------------------------------------------------------
void UART_Putchar(UARTn uartn, uint8 ch)
{
    while( !((uart[uartn]->S1) & UART_S1_TDRE_MASK) );  //等待发送缓冲区空
    uart[uartn]->D = (uint8)ch;                         //写入要发送数据
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uartn           选择串口
//  @param      *buff           要发送的数组地址
//  @param      len             发送长度
//  @return     void
//  @since      v1.0
//  Sample usage:               Uart_Putbuff(uart3,&a[0],sizeof(a));  
//-------------------------------------------------------------------------------------------------------------------
void UART_Putbuff (UARTn uartn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        UART_Putchar(uartn, *buff);
        buff++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uartn           选择串口
//  @param      *str            要发送的字符串地址
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Putstr(uart3,"i lvoe you"); 
//                         或者 uint8 data[5];
//                              UART_Putstr(uart0, data);
//-------------------------------------------------------------------------------------------------------------------
void UART_Putstr (UARTn uartn, const uint8 *str)
{
    while(*str)
    {
        UART_Putchar(uartn, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口等待接收1个字符
//  @param      uartn           选择串口
//  @param      *str            接收的地址
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Getchar(uart3,&dat);
//-------------------------------------------------------------------------------------------------------------------
void UART_Getchar (UARTn uartn, uint8 *ch)
{
    while(!(uart[uartn]->S1 & UART_S1_RDRF_MASK));  //等待接收满了   
    *ch =  uart[uartn]->D;                          // 获取接收到的8位数据
    // 获取 9位数据时ch 应该是uint16 *类型的否则数据会溢出：
    // *ch =   ((( UARTx_C3_REG(UARTN[uartn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uartn]);  //返回9bit
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_RX_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_RX_IRQ_Enable(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_RIE_MASK;                                        //使能UART接收中断
    Enable_Interrupt((uint8)uartn + INT_UART0);                                 //使能IRQ中断                        
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_TX_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_TX_IRQ_Enable(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_TCIE_MASK;                                       //使能UART发送完成中断
    Enable_Interrupt((uint8)uartn + INT_UART0);                                 //使能IRQ中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_RX_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_RX_IRQ_Disable(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_RIE_MASK;                                       //禁止UART接收中断

   //如果发送中断还没有关，则不关闭IRQ
   if(!(uart[uartn]->C2 & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
   {
       Disable_Interrupt((uint8)uartn + INT_UART0);                           //关IRQ中断
   }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_TX_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_TX_IRQ_Disable(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_TCIE_MASK;                                      //禁止UART发送完成中断

   //如果接收中断还没有关，则不关闭IRQ
   if(!(uart[uartn]->C2 & UART_C2_RIE_MASK) )
   {
       Disable_Interrupt((uint8)uartn + INT_UART0);              //关IRQ中断
   }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      使能溢出中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_OR_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_OR_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_ORIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁用溢出中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_OR_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_OR_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_ORIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      使能噪声错误中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_NF_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_NF_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_NEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁用噪声错误中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_NF_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_NF_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_NEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      使能帧传输错误中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_FE_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_FE_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_FEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁用帧传输错误中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_FE_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_FE_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_FEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      使能奇偶校验错误中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_PF_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_PF_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_PEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁用奇偶校验错误中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_PF_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_PF_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_PEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      重定义printf 到串口
//  @param      ch      需要打印的字节
//  @param      stream  数据流
//  @since      v1.0
//  @note       此函数由编译器自带库里的printf所调用
//-------------------------------------------------------------------------------------------------------------------
//int fputc(int ch, FILE *stream)
//{
//    UART_Putchar(DEBUG_PORT, (char)ch);
//    return(ch);
//}


/***********************************************************************************************
*
* @brief    KBI_SetCallback - Set a callback function to execute on a falling edge of an enabled KBI pin
* @param    pointer to function with an UINT8 argument
* @return   none
*
************************************************************************************************/

void UART_SetCallback(pt2FuncU8 ptr)
{
	UART_Callback = ptr;
}


/***********************************************************************************************
*
* @brief    KBI0_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void UART0_SCI0_IRQHandler()

{

//    UART_Flag_Clear(uart0);  /*Clear flag*/
  uint8 temp,temp_irq;
  temp = UART0_S1;
  temp_irq = BIT_GET(temp,UART_S1_RDRF_SHIFT);//判断是否是接收中断
  if(temp_irq)  UART_Callback(0);
//其他中断类同，如果使能了多个中断，注意不同中断应传入参数不同以区分
//  temp_irq = BIT_GET(temp,UART_S1_OR_SHIFT);//判断是否是接收器溢出中断
//  if(temp_irq)  UART_Callback(4);

}
/***********************************************************************************************
*
* @brief    KBI0_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void UART1_SCI1_IRQHandler()

{

//	UART_Flag_Clear(uart1);	/*Clear flag*/
  uint8 temp,temp_irq;
  temp = UART1_S1;
  temp_irq = BIT_GET(temp,UART_S1_RDRF_SHIFT);//判断是否是接收中断
  if(temp_irq)  UART_Callback(1);

}

/***********************************************************************************************
*
* @brief    KBI0_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void UART2_SCI2_IRQHandler()

{

//	UART_Flag_Clear(uart2);	/*Clear flag*/
  uint8 temp,temp_irq;
  temp = UART2_S1;
  temp_irq = BIT_GET(temp,UART_S1_RDRF_SHIFT);//判断是否是接收中断
  if(temp_irq)  UART_Callback(2);

}

//divide int16 into two parts of uint8
void UART_Putint16(UARTn uartn, int16_t var)
{
  int8_t var1;
  int8_t var2;
  var1=var>>8;
  var2=var;
  UART_Putchar(uartn, var2);
  UART_Putchar(uartn, var1);//输出一个16位的int16_t 
}









