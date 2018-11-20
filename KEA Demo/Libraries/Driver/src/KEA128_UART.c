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
//  @brief      UART�ܽŸ���
//  @param      uartn            ѡ��ģ��
//  @param      ptx_n           ѡ�����ź�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Pinmux(uart0, RXTX_B0B1);		//ftm0ģ�飬0ͨ����A0ѡ��
//  @note       ���UART0��A2A3�޷�ʹ��,�볢������A2��A3����
//-------------------------------------------------------------------------------------------------------------------
void UART_Pinmux(UARTn uartn, PTX_n ptx_n)
{
    switch(uartn)
    {
        case uart0:
        {
            SIM_BASE_PTR ->SCGC |= SIM_SCGC_UART0_MASK;               //ʹ�ܴ���0ʱ��
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
            SIM_BASE_PTR->SCGC |= SIM_SCGC_UART1_MASK;               //ʹ�ܴ���0ʱ��
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
            SIM_BASE_PTR->SCGC |= SIM_SCGC_UART2_MASK;               //ʹ�ܴ���0ʱ��
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
//  @brief      uart ��ʼ��
//  @param      uartn           ѡ�񴮿�(uart0,uart1,uart2)
//  @param      baud            ���ڲ�����
//  @return     uint32          ʵ�����õò�����
//  @since      v1.0
//  Sample usage:               UART_Init(uart0,9600,RXTX_B0B1);   //��ʼ������1Ϊ1λ��ʼλ��8λ����λ��1λֹͣλ��������9600
//  @note                       ����KEA128û�в�����΢���Ĵ�������������õò����ʽϸߵ���������Ƚϴ���˷���ʵ�ʲ���������У��
//-------------------------------------------------------------------------------------------------------------------
uint32 UART_Init(UARTn uartn, uint32 baud, PTX_n ptx_n)
{
    vuint32 uart_input_clk;
    uint32 sbr;
    
    UART_Pinmux(uartn, ptx_n);
    //���õ�ʱ��Ӧ�ý�ֹ���ͺͽ���
    uart[uartn]->C2 &= ~(0
                         | UART_C2_TE_MASK
                         | UART_C2_RE_MASK);    
    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    uart[uartn]->C1 |= (0
                        //| UART_C1_M_MASK                      //9 λ�� 8 λģʽѡ�� : 0 Ϊ 8λ ��1 Ϊ 9λ��ע���˱�ʾ0����8λ�� �������9λ��λ8��UARTx_C3�
                        //| UART_C1_PE_MASK                     //��żУ��ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C1_PT_MASK                     //У��λ���� : 0 Ϊ żУ�� ��1 Ϊ ��У��
                       );
    
    //���㲨����
    uart_input_clk = bus_clk_khz * 1000;   //busʱ��

    //UART ������ = UART ģ��ʱ�� / (16 �� SBR[12:0])
    //SBR = UART ģ��ʱ�� / (16 * UART ������)
    sbr = ((uart_input_clk>>4)*10 / baud + 5)/10;               //��������
    if(sbr > 0x1FFF)sbr = 0x1FFF;                               //SBR �� 13bit�����Ϊ 0x1FFF

    //д SBR
    uart[uartn]->BDH &= ~UART_BDH_SBR_MASK;                     //���ԭ��������
    uart[uartn]->BDH |= UART_BDH_SBR(sbr>>8);                   //��д��SBR��λ
    uart[uartn]->BDL  = UART_BDL_SBR((uint8)sbr);               //��д��SBR��λ

    // �����ͺͽ���
    uart[uartn]->C2 |= (0
                        | UART_C2_TE_MASK                       //����ʹ��
                        | UART_C2_RE_MASK                       //����ʹ��
                        //| UART_C2_TIE_MASK                    //�����жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C2_TCIE_MASK                   //��������ж�ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C2_RIE_MASK                    //�������жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                        );
    //����KEA128û�в�����΢���Ĵ�������������õò����ʽϸߵ���������Ƚϴ���˷���ʵ�ʲ���������У��
    return ((uart_input_clk>>4)/sbr);                           //����ʵ�ʲ�����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڷ���һ���ֽ�
//  @param      uartn           ѡ�񴮿�
//  @param      ch              Ҫ���͵��ַ���UARTΪ8λ�Ĵ�����ÿ��ֻ��д��һ���ֽ�����
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Putchar(uart3,0x5a);   
//      ����                    uint8 data;
//                              UART_Putchar(uart0,data);
//-------------------------------------------------------------------------------------------------------------------
void UART_Putchar(UARTn uartn, uint8 ch)
{
    while( !((uart[uartn]->S1) & UART_S1_TDRE_MASK) );  //�ȴ����ͻ�������
    uart[uartn]->D = (uint8)ch;                         //д��Ҫ��������
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڷ�������
//  @param      uartn           ѡ�񴮿�
//  @param      *buff           Ҫ���͵������ַ
//  @param      len             ���ͳ���
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
//  @brief      ���ڷ����ַ���
//  @param      uartn           ѡ�񴮿�
//  @param      *str            Ҫ���͵��ַ�����ַ
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Putstr(uart3,"i lvoe you"); 
//                         ���� uint8 data[5];
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
//  @brief      ���ڵȴ�����1���ַ�
//  @param      uartn           ѡ�񴮿�
//  @param      *str            ���յĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_Getchar(uart3,&dat);
//-------------------------------------------------------------------------------------------------------------------
void UART_Getchar (UARTn uartn, uint8 *ch)
{
    while(!(uart[uartn]->S1 & UART_S1_RDRF_MASK));  //�ȴ���������   
    *ch =  uart[uartn]->D;                          // ��ȡ���յ���8λ����
    // ��ȡ 9λ����ʱch Ӧ����uint16 *���͵ķ������ݻ������
    // *ch =   ((( UARTx_C3_REG(UARTN[uartn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uartn]);  //����9bit
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_RX_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_RX_IRQ_Enable(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_RIE_MASK;                                        //ʹ��UART�����ж�
    Enable_Interrupt((uint8)uartn + INT_UART0);                                 //ʹ��IRQ�ж�                        
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_TX_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_TX_IRQ_Enable(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_TCIE_MASK;                                       //ʹ��UART��������ж�
    Enable_Interrupt((uint8)uartn + INT_UART0);                                 //ʹ��IRQ�ж�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ֹ�����ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_RX_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_RX_IRQ_Disable(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_RIE_MASK;                                       //��ֹUART�����ж�

   //��������жϻ�û�йأ��򲻹ر�IRQ
   if(!(uart[uartn]->C2 & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
   {
       Disable_Interrupt((uint8)uartn + INT_UART0);                           //��IRQ�ж�
   }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ֹ��������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_TX_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_TX_IRQ_Disable(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_TCIE_MASK;                                      //��ֹUART��������ж�

   //��������жϻ�û�йأ��򲻹ر�IRQ
   if(!(uart[uartn]->C2 & UART_C2_RIE_MASK) )
   {
       Disable_Interrupt((uint8)uartn + INT_UART0);              //��IRQ�ж�
   }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_OR_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_OR_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_ORIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_OR_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_OR_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_ORIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ�����������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_NF_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_NF_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_NEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_NF_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_NF_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_NEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ��֡��������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_FE_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_FE_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_FEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����֡��������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_FE_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_FE_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_FEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ����żУ������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_PF_IRQ_Enable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_PF_IRQ_Enable(UARTn uartn)
{
  uart[uartn]->C3 |= UART_C3_PEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������żУ������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               UART_PF_IRQ_Disable(uart3);
//-------------------------------------------------------------------------------------------------------------------
void UART_PF_IRQ_Disable(UARTn uartn)
{
  uart[uartn]->C3 &= ~ UART_C3_PEIE_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ض���printf ������
//  @param      ch      ��Ҫ��ӡ���ֽ�
//  @param      stream  ������
//  @since      v1.0
//  @note       �˺����ɱ������Դ������printf������
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
  temp_irq = BIT_GET(temp,UART_S1_RDRF_SHIFT);//�ж��Ƿ��ǽ����ж�
  if(temp_irq)  UART_Callback(0);
//�����ж���ͬ�����ʹ���˶���жϣ�ע�ⲻͬ�ж�Ӧ���������ͬ������
//  temp_irq = BIT_GET(temp,UART_S1_OR_SHIFT);//�ж��Ƿ��ǽ���������ж�
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
  temp_irq = BIT_GET(temp,UART_S1_RDRF_SHIFT);//�ж��Ƿ��ǽ����ж�
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
  temp_irq = BIT_GET(temp,UART_S1_RDRF_SHIFT);//�ж��Ƿ��ǽ����ж�
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
  UART_Putchar(uartn, var1);//���һ��16λ��int16_t 
}









