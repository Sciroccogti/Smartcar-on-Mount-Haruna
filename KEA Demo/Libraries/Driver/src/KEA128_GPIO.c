/*
  ******************************************************************************
  * @file    gpio.c
  * @author  xhJiang
  * @version v1.2
  * @date    2018.7.24
  * @mail:   blog_xhjiang@163.com
  * @brief   gpio configure
  * @ref     ��ɿƼ�KEA&����KEA&ɽ��KEA�ײ��&�ٷ���
  ******************************************************************************
  */

#include "delay.h"
#include "KEA128_port.h"
#include "KEA128_GPIO.h"

//��ѡ�����GPIO�Ĵ�������ͨGPIO�Ĵ���
static FGPIO_Type * const FGPIOX[] = FGPIO_BASES;
static GPIO_MemMapPtr const GPIOX[] = GPIO_BASE_PTRS;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpio            ���ٳ�ʼ��
//  @param      ptx_n           ѡ�������
//  @param      dir             ���ŷ���    GPI������(Ĭ����������)  GPO�����
//  @param      data             �����ʼ״̬,0=�͵�ƽ,1=�ߵ�ƽ ������ģʽ�£�0�������������
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Init(A5,GPO,0);   //��ʼ��A5Ϊgpioģʽ������Ϊ���ģʽ����ʼ������͵�ƽ
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Init(PTX_n ptx_n, GPIO_DR dir, uint8 data)
{
  vuint8 ptx,ptn;

  ptx = PTX(ptx_n);//��¼ģ���    //A,B,C......(A����D7-A0��B����H7-E0��C����I6-I0)
  ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
  if(data) data=1;//����data��ֵΪbool����
  GPIO_Dir(ptx_n, dir);//�������ŷ���

  if(dir == GPO)
  {
//    Port_Pue(ptx_n, DISABLE);   //��Ϊ���ʱ�Զ�������������
    GPIO_Set(ptx_n, data);//��������˿ڳ�ʼ״̬
  }
  else
  {
    if(data) Port_Pue(ptx_n, ENABLE);    //ʹ����������
    else Port_Pue(ptx_n, DISABLE);   //������������
  }
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ŷ���
//  @param      ptx_n           ѡ�������   
//  @param      dir             ���ŷ���    GPI������ GPO�����
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Dir(A5,GPO);   //����A5Ϊ���ģʽ
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Dir(PTX_n ptx_n, GPIO_DR dir)
{
    vuint8 ptx,ptn;
    uint32 temp;//�Ĵ����м����
//    GPIO_MemMapPtr gpiox = GPIOX_BASE(ptx_n);//���ڱ�����volatile���ͣ�������ʹ��temp�����м����
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    if(dir == GPI)
    {
        temp = GPIOX[ptx]->PIDR;
        temp &= ~((uint32)1<<ptn);
        GPIOX[ptx]->PIDR = temp;    //ȡ��������ã���Ч��GPIO_PIDR_REG(gpiox)  &=  ~(1 << ptn);
        
        temp = GPIOX[ptx]->PDDR;    
        temp &= ~((uint32)1<<ptn);
        GPIOX[ptx]->PDDR = temp;    //����Ϊ����ģʽ����Ч��GPIO_PDDR_REG(gpiox) &= ~(1 << ptn);
    }
    else
    {
        temp = GPIOX[ptx]->PIDR;
        temp |= ((uint32)1<<ptn);
        GPIOX[ptx]->PIDR = temp;    //������ã�������豣��
        
        temp = GPIOX[ptx]->PDDR;    
        temp |= ((uint32)1<<ptn);
        GPIOX[ptx]->PDDR = temp;    //����Ϊ���ģʽ
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����״̬
//  @param      ptx_n           ѡ�������   
//  @return     �ܽŵ�״̬      1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
//  @since      v1.0
//  Sample usage:               uint8 status = GPIO_Get(A5);   //��ȡA5Ϊ״̬
//-------------------------------------------------------------------------------------------------------------------
uint8 GPIO_Get(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
//    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptx_n)) , ptn) == GPI ); // ���ԣ���� ��������Ƿ�Ϊ����
    // ��ȡ GPIO PDDR �ܽź� ���Ƚ��Ƿ�Ϊ����
    //���ö˿�״̬
    return ( (GPIOX[ptx]->PDIR >> ptn) & 0x1 );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������״̬
//  @param      ptx_n           ѡ�������   
//  @param      data             1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Set(A5,0);   //����A5Ϊ�͵�ƽ
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Set(PTX_n ptx_n, uint8 data)
{
    vuint8 ptx,ptn;
    uint32 temp;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......

    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptx_n)) , ptn) == GPO); // ���ԣ���� ��������Ƿ�Ϊ���    
    // ��ȡ GPIO PDDR �ܽź� ���Ƚ��Ƿ�Ϊ���
    //���ö˿�״̬
    temp = GPIOX[ptx]->PDOR;    
    temp &= ~((uint32)1<<ptn);  //����ն˿�״̬λ
    temp |=  (uint32)data<<ptn; //�ٶԸ�λ���и�ֵ
    GPIOX[ptx]->PDOR = temp;    //���ö˿ڸߵ͵�ƽ״̬
//    //���ö˿�״̬
//    //ʹ����������λ�Ĵ�����������ֶ��������ɱ����ж���������ͬʱ���������
//    temp = ((uint32)1<<ptn);
//    if(dat) GPIOX[ptx]->PSOR =  temp;
//    else    GPIOX[ptx]->PCOR =  temp;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ת����״̬
//  @param      ptx_n           ѡ�������   
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Turn(A5);   //��תA5
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Turn(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
//    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptx_n)) , ptn) == GPO); // ���ԣ���� ��������Ƿ�Ϊ���    
    // ��ȡ GPIO PDDR �ܽź� ���Ƚ��Ƿ�Ϊ���
    //���ö˿�״̬

    GPIOX[ptx]->PTOR = (uint32)(1<<ptn);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpio            ���ٳ�ʼ��
//  @param      ptx_n           ѡ�������
//  @param      dir             ���ŷ���    GPI������(Ĭ����������)  GPO�����
//  @param      data             �����ʼ״̬,0=�͵�ƽ,1=�ߵ�ƽ ������ģʽ�£�0�������������
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Init(A5,GPO,0);   //��ʼ��A5Ϊgpioģʽ������Ϊ���ģʽ����ʼ������͵�ƽ
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Init(PTX_n ptx_n, GPIO_DR dir, uint8 data)
{
  vuint8 ptx,ptn;

  ptx = PTX(ptx_n);//��¼ģ���    //A,B,C......(A����D7-A0��B����H7-E0��C����I6-I0)
  ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
  if(data) data=1;//����data��ֵΪbool����
  FGPIO_Dir(ptx_n, dir);//�������ŷ���

  if(dir == GPO)
  {
//    Port_Pue(ptx_n, DISABLE);   //��Ϊ���ʱ�Զ�������������
    FGPIO_Set(ptx_n, data);//��������˿ڳ�ʼ״̬
  }
  else
  {
    if(data) Port_Pue(ptx_n, ENABLE);    //ʹ����������
    else Port_Pue(ptx_n, DISABLE);   //������������
  }
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ŷ���
//  @param      ptx_n           ѡ�������   
//  @param      dir             ���ŷ���    GPI������ GPO�����
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Dir(A5,GPO);   //����A5Ϊ���ģʽ
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Dir(PTX_n ptx_n, GPIO_DR dir)
{
    vuint8 ptx,ptn;
    uint32 temp;//�Ĵ����м����
//    GPIO_MemMapPtr gpiox = GPIOX_BASE(ptx_n);//���ڱ�����volatile���ͣ�������ʹ��temp�����м����
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    if(dir == GPI)
    {
        temp = FGPIOX[ptx]->PIDR;
        temp &= ~((uint32)1<<ptn);
        FGPIOX[ptx]->PIDR = temp;    //ȡ��������ã���Ч��GPIO_PIDR_REG(gpiox)  &=  ~(1 << ptn);
        
        temp = FGPIOX[ptx]->PDDR;    
        temp &= ~((uint32)1<<ptn);
        FGPIOX[ptx]->PDDR = temp;    //����Ϊ����ģʽ����Ч��GPIO_PDDR_REG(gpiox) &= ~(1 << ptn);
    }
    else
    {
        temp = FGPIOX[ptx]->PIDR;
        temp |= ((uint32)1<<ptn);
        FGPIOX[ptx]->PIDR = temp;    //������ã�������豣��
        
        temp = FGPIOX[ptx]->PDDR;    
        temp |= ((uint32)1<<ptn);
        FGPIOX[ptx]->PDDR = temp;    //����Ϊ���ģʽ
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����״̬
//  ���⴦������ֱ�ӽ��мĴ���������ʵ������DMA����
//  @param      ptx_n           ѡ�������   
//  @return     �ܽŵ�״̬      1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
//  @since      v1.0
//  Sample usage:               uint8 status = GPIO_Get(A5);   //��ȡA5Ϊ״̬
//-------------------------------------------------------------------------------------------------------------------
uint8 FGPIO_Get(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
//    ASSERT( BIT_GET( FGPIO_PDDR_REG(FGPIOX_BASE(ptx_n)) , ptn) == GPI ); // ���ԣ���� ��������Ƿ�Ϊ����
    // ��ȡ GPIO PDDR �ܽź� ���Ƚ��Ƿ�Ϊ����
    //���ö˿�״̬
    return ( (FGPIOX[ptx]->PDIR >> ptn) & 0x1 );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������״̬
//  @param      ptx_n           ѡ�������   
//  @param      data             1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Set(A5,0);   //����A5Ϊ�͵�ƽ
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Set(PTX_n ptx_n, uint8 data)
{
    vuint8 ptx,ptn;
    uint32 temp;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......

    ASSERT( BIT_GET( FGPIO_PDDR_REG(FGPIOX_BASE(ptx_n)) , ptn) == GPO); // ���ԣ���� ��������Ƿ�Ϊ���    
    // ��ȡ GPIO PDDR �ܽź� ���Ƚ��Ƿ�Ϊ���
    //���ö˿�״̬
    temp = FGPIOX[ptx]->PDOR;    
    temp &= ~((uint32)1<<ptn);  //����ն˿�״̬λ
    temp |=  (uint32)data<<ptn; //�ٶԸ�λ���и�ֵ
    FGPIOX[ptx]->PDOR = temp;    //���ö˿ڸߵ͵�ƽ״̬
//    //���ö˿�״̬
//    //ʹ����������λ�Ĵ�����������ֶ��������ɱ����ж���������ͬʱ���������
//    temp = ((uint32)1<<ptn);
//    if(dat) GPIOX[ptx]->PSOR =  temp;
//    else    GPIOX[ptx]->PCOR =  temp;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ת����״̬
//  @param      ptx_n           ѡ�������   
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Turn(A5);   //��תA5
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Turn(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
//    ASSERT( BIT_GET( FGPIO_PDDR_REG(FGPIOX_BASE(ptx_n)) , ptn) == GPO); // ���ԣ���� ��������Ƿ�Ϊ���    
    // ��ȡ GPIO PDDR �ܽź� ���Ƚ��Ƿ�Ϊ���
    //���ö˿�״̬
    FGPIOX[ptx]->PTOR = (uint32)(1<<ptn);
}

/*!
 *  @brief      �����������key��״̬������ʱ���������ɷ����ж��ڣ�5ms��ʱ�����Ῠ�ж�
 *  @param      ptn_n           gpio���
 *  @return     uint8    KEY״̬��KEY_UP��KEY_DOWN��
 *  @since      v1.0
 *  Sample usage:
                    if(Key_Check(A1) ==  KEY_DOWN)
                    {
                        printf("\n��������")
                    }
 */
uint8 Key_Pullup_Check(PTX_n ptx_n)
{
    if(GPIO_Get(ptx_n) == 0)
    {
        Soft_Delay_ms(5);
        if(GPIO_Get(ptx_n) == 0)
        {
            return KEY_DOWN;
        }
    }
    return KEY_UP;
}

/*!
 *  @brief      �����������key��״̬������ʱ���������ɷ����ж��ڣ�5ms��ʱ�����Ῠ�ж�
 *  @param      ptn_n           gpio���
 *  @return     uint8    KEY״̬��KEY_UP��KEY_DOWN��
 *  @since      v1.0
 *  Sample usage:
                    if(Key_Check(A1) ==  KEY_DOWN)
                    {
                        printf("\n��������")
                    }
 */
uint8 Key_Pulldown_Check(PTX_n ptx_n)
{
    if(GPIO_Get(ptx_n) == 1)
    {
        Soft_Delay_ms(5);
        if(GPIO_Get(ptx_n) == 1)
        {
            return KEY_DOWN;
        }
    }
    return KEY_UP;
}

//���´���ժ��ɽ��⣬�Լ��޸ĺ���ʹ��
//
////�����Ƕ��尴����ʱ�䣬��λΪ �� 10ms���ж�ʱ�䣩
//#define KEY_DOWN_TIME           1       //����ȷ�ϰ���ʱ��
//#define KEY_HOLD_TIME           50      //����holdȷ��ʱ�䣬���253��������Ҫ�޸� keytime ������
////�������һֱ����ȥ����ÿ�� KEY_HOLD_TIME - KEY_DOWN_TIME ʱ��ᷢ��һ�� KEY_HOLD ��Ϣ
//
////���尴����ϢFIFO��С
//#define KEY_MSG_FIFO_SIZE       20      //��� 255��������Ҫ�޸�key_msg_front/key_msg_rear����
///*********************  ���´�����ʵ�ְ�����ʱɨ�裬������Ϣ��FIFO  ********************/
///*
// * ���尴����ϢFIFO״̬
// */
//typedef enum
//{
//    KEY_MSG_EMPTY,      //û�а�����Ϣ
//    KEY_MSG_NORMAL,     //�������а�����Ϣ��������
//    KEY_MSG_FULL,       //������Ϣ��
//} key_msg_e;
//
///*
// * ���尴����ϢFIFO��صı���
// */
//KEY_MSG_t           key_msg[KEY_MSG_FIFO_SIZE];             //������ϢFIFO
//volatile uint8      key_msg_front = 0, key_msg_rear = 0;    //����FIFO��ָ��
//volatile uint8      key_msg_flag = KEY_MSG_EMPTY;           //������ϢFIFO״̬
//
//
///*!
// *  @brief      ���Ͱ�����Ϣ��FIFO
// *  @param      KEY_MSG_t       ������Ϣ
// *  @since      v5.0
// *  Sample usage:
//                    KEY_MSG_t *keymsg;
//                    keymsg.key      = KEY_U;
//                    keymsg.status   = KEY_HOLD;
//                    send_key_msg(keymsg);                   //����
// */
//void send_key_msg(KEY_MSG_t keymsg)
//{
//    uint8 tmp;
//    //������FIFO��
//    if(key_msg_flag == KEY_MSG_FULL)
//    {
//        //����ֱ�Ӳ�����
//        return ;
//    }
//    key_msg[key_msg_rear].key = keymsg.key;
//    key_msg[key_msg_rear].status = keymsg.status;
//
//    key_msg_rear++;
//
//    if(key_msg_rear >= KEY_MSG_FIFO_SIZE)
//    {
//        key_msg_rear = 0;                       //��ͷ��ʼ
//    }
//
//    tmp = key_msg_rear;
//    if(tmp == key_msg_front)                   //׷��ƨ���ˣ�����
//    {
//        key_msg_flag = KEY_MSG_FULL;
//    }
//    else
//    {
//        key_msg_flag = KEY_MSG_NORMAL;
//    }
//}
//
//
///*!
// *  @brief      ��FIFO���ȡ������Ϣ
// *  @param      KEY_MSG_t       ������Ϣ
// *  @return     �Ƿ��ȡ������Ϣ��1Ϊ��ȡ�ɹ���0Ϊû��ȡ��������Ϣ��
// *  @since      v5.0
// *  Sample usage:
//                    KEY_MSG_t keymsg;
//                    if(get_key_msg(&keymsg) == 1)
//                    {
//                        printf("\n���°���KEY%d,����Ϊ%d��0Ϊ���£�1Ϊ����2Ϊ������",keymsg.key,keymsg.status);
//                    }
// */
//uint8 get_key_msg(KEY_MSG_t *keymsg)
//{
//    uint8 tmp;
//
//    if(key_msg_flag == KEY_MSG_EMPTY)               //������ϢFIFOΪ�գ�ֱ�ӷ���0
//    {
//        return 0;
//    }
//
//    keymsg->key = key_msg[key_msg_front].key;       //��FIFO�����л�ȡ����ֵ
//    keymsg->status = key_msg[key_msg_front].status; //��FIFO�����л�ȡ��������
//
//    key_msg_front++;                                //FIFO����ָ���1��ָ����һ����Ϣ
//
//    if(key_msg_front >= KEY_MSG_FIFO_SIZE)          //FIFOָ�����������0��ʼ����
//    {
//        key_msg_front = 0;                          //��ͷ��ʼ������ѭ���������飩
//    }
//
//    tmp = key_msg_rear;
//    if(key_msg_front == tmp)                        //�Ƚ϶��׺Ͷ�β�Ƿ�һ����һ�����ʾFIFO�ѿ���
//    {
//        key_msg_flag = KEY_MSG_EMPTY;
//    }
//    else
//    {
//        key_msg_flag = KEY_MSG_NORMAL;
//    }
//
//    return 1;
//}
//
///*!
// *  @brief      ��ʱ���key״̬
// *  @since      v5.0
// *  @note       �˺�����Ҫ���� ��ʱ�жϷ��������ʱ10msִ��һ��
// */
//void key_IRQHandler(void)
//{
//
//    KEY_e   keynum;
//    static uint8 keytime[KEY_MAX];                          //��̬���飬��������鰴��ʱ��
//
//    KEY_MSG_t keymsg;                                       //������Ϣ
//
//    for(keynum = (KEY_e)0 ; keynum < KEY_MAX; keynum ++)    //ÿ��������ѯ
//    {
//        if(key_get(keynum) == KEY_DOWN)                     //�жϰ����Ƿ���(������)
//        {
//            keytime[keynum]++;                              //����ʱ���ۼ�
//
//            if(keytime[keynum] <= KEY_DOWN_TIME)            //�ж�ʱ���Ƿ�û��������ȷ�ϰ���ʱ��
//            {
//                continue;                                   //û�ﵽ��������ȴ�
//            }
//            else if(keytime[keynum] == KEY_DOWN_TIME + 1 )  //�ж�ʱ���Ƿ�Ϊ����ȷ�ϰ���ʱ��
//            {
//                //ȷ�ϰ�������
//                keymsg.key = keynum;
//                keymsg.status = KEY_DOWN;
//                send_key_msg(keymsg);                       //�Ѱ���ֵ�Ͱ������ͷ�����Ϣ��FIFO
//            }
//            else if(keytime[keynum] <= KEY_HOLD_TIME)       //�Ƿ�û��������HOLD����ȷ��ʱ��
//            {
//                continue;                                   //û������������ȴ�
//            }
//            else if(keytime[keynum]  == KEY_HOLD_TIME + 1)  //�Ƿ�Ϊ����holdȷ��ʱ��
//            {
//                //ȷ�ϳ���HOLD
//                keymsg.key = keynum;
//                keymsg.status = KEY_HOLD;
//                send_key_msg(keymsg);                       //����
//                keytime[keynum] = KEY_DOWN_TIME + 1;
//            }
//            else
//            {
//                keytime[keynum] = KEY_DOWN_TIME + 1;        //�����ظ���� hold ״̬
//            }
//        }
//        else
//        {
//            if(keytime[keynum] > KEY_DOWN_TIME)             //���ȷ�Ϲ����°���
//            {
//                keymsg.key = keynum;
//                keymsg.status = KEY_UP;
//                send_key_msg(keymsg);                       //���Ͱ���������Ϣ
//            }
//
//            keytime[keynum] = 0;                            //ʱ���ۼ���0
//        }
//    }
//}

