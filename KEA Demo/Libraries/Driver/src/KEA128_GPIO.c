/*
  ******************************************************************************
  * @file    gpio.c
  * @author  xhJiang
  * @version v1.2
  * @date    2018.7.24
  * @mail:   blog_xhjiang@163.com
  * @brief   gpio configure
  * @ref     逐飞科技KEA&龙邱KEA&山外KEA底层库&官方库
  ******************************************************************************
  */

#include "delay.h"
#include "KEA128_port.h"
#include "KEA128_GPIO.h"

//可选择快速GPIO寄存器或普通GPIO寄存器
static FGPIO_Type * const FGPIOX[] = FGPIO_BASES;
static GPIO_MemMapPtr const GPIOX[] = GPIO_BASE_PTRS;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpio            快速初始化
//  @param      ptx_n           选择的引脚
//  @param      dir             引脚方向    GPI：输入(默认上拉输入)  GPO：输出
//  @param      data             输出初始状态,0=低电平,1=高电平 （输入模式下，0代表禁用上拉）
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Init(A5,GPO,0);   //初始化A5为gpio模式，设置为输出模式，初始化输出低电平
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Init(PTX_n ptx_n, GPIO_DR dir, uint8 data)
{
  vuint8 ptx,ptn;

  ptx = PTX(ptx_n);//记录模块号    //A,B,C......(A包括D7-A0，B包括H7-E0，C包括I6-I0)
  ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
  if(data) data=1;//修正data的值为bool类型
  GPIO_Dir(ptx_n, dir);//设置引脚方向

  if(dir == GPO)
  {
//    Port_Pue(ptx_n, DISABLE);   //作为输出时自动禁用上拉输入
    GPIO_Set(ptx_n, data);//设置输出端口初始状态
  }
  else
  {
    if(data) Port_Pue(ptx_n, ENABLE);    //使能上拉输入
    else Port_Pue(ptx_n, DISABLE);   //禁用上拉输入
  }
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置引脚方向
//  @param      ptx_n           选择的引脚   
//  @param      dir             引脚方向    GPI：输入 GPO：输出
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Dir(A5,GPO);   //设置A5为输出模式
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Dir(PTX_n ptx_n, GPIO_DR dir)
{
    vuint8 ptx,ptn;
    uint32 temp;//寄存器中间变量
//    GPIO_MemMapPtr gpiox = GPIOX_BASE(ptx_n);//由于变量是volatile类型，更建议使用temp进行中间操作
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    if(dir == GPI)
    {
        temp = GPIOX[ptx]->PIDR;
        temp &= ~((uint32)1<<ptn);
        GPIOX[ptx]->PIDR = temp;    //取消输入禁用，等效于GPIO_PIDR_REG(gpiox)  &=  ~(1 << ptn);
        
        temp = GPIOX[ptx]->PDDR;    
        temp &= ~((uint32)1<<ptn);
        GPIOX[ptx]->PDDR = temp;    //设置为输入模式，等效于GPIO_PDDR_REG(gpiox) &= ~(1 << ptn);
    }
    else
    {
        temp = GPIOX[ptx]->PIDR;
        temp |= ((uint32)1<<ptn);
        GPIOX[ptx]->PIDR = temp;    //输入禁用，输入高阻保护
        
        temp = GPIOX[ptx]->PDDR;    
        temp |= ((uint32)1<<ptn);
        GPIOX[ptx]->PDDR = temp;    //设置为输出模式
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取引脚状态
//  @param      ptx_n           选择的引脚   
//  @return     管脚的状态      1为高电平，0为低电平
//  @since      v1.0
//  Sample usage:               uint8 status = GPIO_Get(A5);   //读取A5为状态
//-------------------------------------------------------------------------------------------------------------------
uint8 GPIO_Get(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
//    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptx_n)) , ptn) == GPI ); // 断言，检测 输出方向是否为输入
    // 获取 GPIO PDDR 管脚号 ，比较是否为输入
    //设置端口状态
    return ( (GPIOX[ptx]->PDIR >> ptn) & 0x1 );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置引脚状态
//  @param      ptx_n           选择的引脚   
//  @param      data             1为高电平，0为低电平
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Set(A5,0);   //设置A5为低电平
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Set(PTX_n ptx_n, uint8 data)
{
    vuint8 ptx,ptn;
    uint32 temp;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......

    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptx_n)) , ptn) == GPO); // 断言，检测 输出方向是否为输出    
    // 获取 GPIO PDDR 管脚号 ，比较是否为输出
    //设置端口状态
    temp = GPIOX[ptx]->PDOR;    
    temp &= ~((uint32)1<<ptn);  //先清空端口状态位
    temp |=  (uint32)data<<ptn; //再对该位进行赋值
    GPIOX[ptx]->PDOR = temp;    //设置端口高低电平状态
//    //设置端口状态
//    //使用清零与置位寄存器，避免出现读操作，可避免中断与主函数同时对外设访问
//    temp = ((uint32)1<<ptn);
//    if(dat) GPIOX[ptx]->PSOR =  temp;
//    else    GPIOX[ptx]->PCOR =  temp;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      翻转引脚状态
//  @param      ptx_n           选择的引脚   
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Turn(A5);   //翻转A5
//-------------------------------------------------------------------------------------------------------------------
void GPIO_Turn(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
//    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptx_n)) , ptn) == GPO); // 断言，检测 输出方向是否为输出    
    // 获取 GPIO PDDR 管脚号 ，比较是否为输出
    //设置端口状态

    GPIOX[ptx]->PTOR = (uint32)(1<<ptn);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpio            快速初始化
//  @param      ptx_n           选择的引脚
//  @param      dir             引脚方向    GPI：输入(默认上拉输入)  GPO：输出
//  @param      data             输出初始状态,0=低电平,1=高电平 （输入模式下，0代表禁用上拉）
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Init(A5,GPO,0);   //初始化A5为gpio模式，设置为输出模式，初始化输出低电平
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Init(PTX_n ptx_n, GPIO_DR dir, uint8 data)
{
  vuint8 ptx,ptn;

  ptx = PTX(ptx_n);//记录模块号    //A,B,C......(A包括D7-A0，B包括H7-E0，C包括I6-I0)
  ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
  if(data) data=1;//修正data的值为bool类型
  FGPIO_Dir(ptx_n, dir);//设置引脚方向

  if(dir == GPO)
  {
//    Port_Pue(ptx_n, DISABLE);   //作为输出时自动禁用上拉输入
    FGPIO_Set(ptx_n, data);//设置输出端口初始状态
  }
  else
  {
    if(data) Port_Pue(ptx_n, ENABLE);    //使能上拉输入
    else Port_Pue(ptx_n, DISABLE);   //禁用上拉输入
  }
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置引脚方向
//  @param      ptx_n           选择的引脚   
//  @param      dir             引脚方向    GPI：输入 GPO：输出
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Dir(A5,GPO);   //设置A5为输出模式
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Dir(PTX_n ptx_n, GPIO_DR dir)
{
    vuint8 ptx,ptn;
    uint32 temp;//寄存器中间变量
//    GPIO_MemMapPtr gpiox = GPIOX_BASE(ptx_n);//由于变量是volatile类型，更建议使用temp进行中间操作
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    if(dir == GPI)
    {
        temp = FGPIOX[ptx]->PIDR;
        temp &= ~((uint32)1<<ptn);
        FGPIOX[ptx]->PIDR = temp;    //取消输入禁用，等效于GPIO_PIDR_REG(gpiox)  &=  ~(1 << ptn);
        
        temp = FGPIOX[ptx]->PDDR;    
        temp &= ~((uint32)1<<ptn);
        FGPIOX[ptx]->PDDR = temp;    //设置为输入模式，等效于GPIO_PDDR_REG(gpiox) &= ~(1 << ptn);
    }
    else
    {
        temp = FGPIOX[ptx]->PIDR;
        temp |= ((uint32)1<<ptn);
        FGPIOX[ptx]->PIDR = temp;    //输入禁用，输入高阻保护
        
        temp = FGPIOX[ptx]->PDDR;    
        temp |= ((uint32)1<<ptn);
        FGPIOX[ptx]->PDDR = temp;    //设置为输出模式
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取引脚状态
//  特殊处理后可以直接进行寄存器操作，实现类似DMA操作
//  @param      ptx_n           选择的引脚   
//  @return     管脚的状态      1为高电平，0为低电平
//  @since      v1.0
//  Sample usage:               uint8 status = GPIO_Get(A5);   //读取A5为状态
//-------------------------------------------------------------------------------------------------------------------
uint8 FGPIO_Get(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
//    ASSERT( BIT_GET( FGPIO_PDDR_REG(FGPIOX_BASE(ptx_n)) , ptn) == GPI ); // 断言，检测 输出方向是否为输入
    // 获取 GPIO PDDR 管脚号 ，比较是否为输入
    //设置端口状态
    return ( (FGPIOX[ptx]->PDIR >> ptn) & 0x1 );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置引脚状态
//  @param      ptx_n           选择的引脚   
//  @param      data             1为高电平，0为低电平
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Set(A5,0);   //设置A5为低电平
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Set(PTX_n ptx_n, uint8 data)
{
    vuint8 ptx,ptn;
    uint32 temp;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......

    ASSERT( BIT_GET( FGPIO_PDDR_REG(FGPIOX_BASE(ptx_n)) , ptn) == GPO); // 断言，检测 输出方向是否为输出    
    // 获取 GPIO PDDR 管脚号 ，比较是否为输出
    //设置端口状态
    temp = FGPIOX[ptx]->PDOR;    
    temp &= ~((uint32)1<<ptn);  //先清空端口状态位
    temp |=  (uint32)data<<ptn; //再对该位进行赋值
    FGPIOX[ptx]->PDOR = temp;    //设置端口高低电平状态
//    //设置端口状态
//    //使用清零与置位寄存器，避免出现读操作，可避免中断与主函数同时对外设访问
//    temp = ((uint32)1<<ptn);
//    if(dat) GPIOX[ptx]->PSOR =  temp;
//    else    GPIOX[ptx]->PCOR =  temp;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      翻转引脚状态
//  @param      ptx_n           选择的引脚   
//  @return     void
//  @since      v1.0
//  Sample usage:               GPIO_Turn(A5);   //翻转A5
//-------------------------------------------------------------------------------------------------------------------
void FGPIO_Turn(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
//    ASSERT( BIT_GET( FGPIO_PDDR_REG(FGPIOX_BASE(ptx_n)) , ptn) == GPO); // 断言，检测 输出方向是否为输出    
    // 获取 GPIO PDDR 管脚号 ，比较是否为输出
    //设置端口状态
    FGPIOX[ptx]->PTOR = (uint32)(1<<ptn);
}

/*!
 *  @brief      检测上拉按键key的状态（带延时消抖）不可放在中断内，5ms延时消抖会卡中断
 *  @param      ptn_n           gpio编号
 *  @return     uint8    KEY状态（KEY_UP、KEY_DOWN）
 *  @since      v1.0
 *  Sample usage:
                    if(Key_Check(A1) ==  KEY_DOWN)
                    {
                        printf("\n按键按下")
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
 *  @brief      检测下拉按键key的状态（带延时消抖）不可放在中断内，5ms延时消抖会卡中断
 *  @param      ptn_n           gpio编号
 *  @return     uint8    KEY状态（KEY_UP、KEY_DOWN）
 *  @since      v1.0
 *  Sample usage:
                    if(Key_Check(A1) ==  KEY_DOWN)
                    {
                        printf("\n按键按下")
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

//以下代码摘自山外库，稍加修改后便可使用
//
////下面是定义按键的时间，单位为 ： 10ms（中断时间）
//#define KEY_DOWN_TIME           1       //消抖确认按下时间
//#define KEY_HOLD_TIME           50      //长按hold确认时间，最多253，否则需要修改 keytime 的类型
////如果按键一直按下去，则每隔 KEY_HOLD_TIME - KEY_DOWN_TIME 时间会发送一个 KEY_HOLD 消息
//
////定义按键消息FIFO大小
//#define KEY_MSG_FIFO_SIZE       20      //最多 255，否则需要修改key_msg_front/key_msg_rear类型
///*********************  如下代码是实现按键定时扫描，发送消息到FIFO  ********************/
///*
// * 定义按键消息FIFO状态
// */
//typedef enum
//{
//    KEY_MSG_EMPTY,      //没有按键消息
//    KEY_MSG_NORMAL,     //正常，有按键消息，但不满
//    KEY_MSG_FULL,       //按键消息满
//} key_msg_e;
//
///*
// * 定义按键消息FIFO相关的变量
// */
//KEY_MSG_t           key_msg[KEY_MSG_FIFO_SIZE];             //按键消息FIFO
//volatile uint8      key_msg_front = 0, key_msg_rear = 0;    //接收FIFO的指针
//volatile uint8      key_msg_flag = KEY_MSG_EMPTY;           //按键消息FIFO状态
//
//
///*!
// *  @brief      发送按键消息到FIFO
// *  @param      KEY_MSG_t       按键消息
// *  @since      v5.0
// *  Sample usage:
//                    KEY_MSG_t *keymsg;
//                    keymsg.key      = KEY_U;
//                    keymsg.status   = KEY_HOLD;
//                    send_key_msg(keymsg);                   //发送
// */
//void send_key_msg(KEY_MSG_t keymsg)
//{
//    uint8 tmp;
//    //保存在FIFO里
//    if(key_msg_flag == KEY_MSG_FULL)
//    {
//        //满了直接不处理
//        return ;
//    }
//    key_msg[key_msg_rear].key = keymsg.key;
//    key_msg[key_msg_rear].status = keymsg.status;
//
//    key_msg_rear++;
//
//    if(key_msg_rear >= KEY_MSG_FIFO_SIZE)
//    {
//        key_msg_rear = 0;                       //重头开始
//    }
//
//    tmp = key_msg_rear;
//    if(tmp == key_msg_front)                   //追到屁股了，满了
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
// *  @brief      从FIFO里获取按键消息
// *  @param      KEY_MSG_t       按键消息
// *  @return     是否获取按键消息（1为获取成功，0为没获取到按键消息）
// *  @since      v5.0
// *  Sample usage:
//                    KEY_MSG_t keymsg;
//                    if(get_key_msg(&keymsg) == 1)
//                    {
//                        printf("\n按下按键KEY%d,类型为%d（0为按下，1为弹起，2为长按）",keymsg.key,keymsg.status);
//                    }
// */
//uint8 get_key_msg(KEY_MSG_t *keymsg)
//{
//    uint8 tmp;
//
//    if(key_msg_flag == KEY_MSG_EMPTY)               //按键消息FIFO为空，直接返回0
//    {
//        return 0;
//    }
//
//    keymsg->key = key_msg[key_msg_front].key;       //从FIFO队首中获取按键值
//    keymsg->status = key_msg[key_msg_front].status; //从FIFO队首中获取按键类型
//
//    key_msg_front++;                                //FIFO队首指针加1，指向下一个消息
//
//    if(key_msg_front >= KEY_MSG_FIFO_SIZE)          //FIFO指针队首溢出则从0开始计数
//    {
//        key_msg_front = 0;                          //重头开始计数（循环利用数组）
//    }
//
//    tmp = key_msg_rear;
//    if(key_msg_front == tmp)                        //比较队首和队尾是否一样，一样则表示FIFO已空了
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
// *  @brief      定时检测key状态
// *  @since      v5.0
// *  @note       此函数需要放入 定时中断服务函数里，定时10ms执行一次
// */
//void key_IRQHandler(void)
//{
//
//    KEY_e   keynum;
//    static uint8 keytime[KEY_MAX];                          //静态数组，保存各数组按下时间
//
//    KEY_MSG_t keymsg;                                       //按键消息
//
//    for(keynum = (KEY_e)0 ; keynum < KEY_MAX; keynum ++)    //每个按键轮询
//    {
//        if(key_get(keynum) == KEY_DOWN)                     //判断按键是否按下(不消抖)
//        {
//            keytime[keynum]++;                              //按下时间累加
//
//            if(keytime[keynum] <= KEY_DOWN_TIME)            //判断时间是否没超过消抖确认按下时间
//            {
//                continue;                                   //没达到，则继续等待
//            }
//            else if(keytime[keynum] == KEY_DOWN_TIME + 1 )  //判断时间是否为消抖确认按下时间
//            {
//                //确认按键按下
//                keymsg.key = keynum;
//                keymsg.status = KEY_DOWN;
//                send_key_msg(keymsg);                       //把按键值和按键类型发送消息到FIFO
//            }
//            else if(keytime[keynum] <= KEY_HOLD_TIME)       //是否没超过长按HOLD按键确认时间
//            {
//                continue;                                   //没超过，则继续等待
//            }
//            else if(keytime[keynum]  == KEY_HOLD_TIME + 1)  //是否为长按hold确认时间
//            {
//                //确认长按HOLD
//                keymsg.key = keynum;
//                keymsg.status = KEY_HOLD;
//                send_key_msg(keymsg);                       //发送
//                keytime[keynum] = KEY_DOWN_TIME + 1;
//            }
//            else
//            {
//                keytime[keynum] = KEY_DOWN_TIME + 1;        //继续重复检测 hold 状态
//            }
//        }
//        else
//        {
//            if(keytime[keynum] > KEY_DOWN_TIME)             //如果确认过按下按键
//            {
//                keymsg.key = keynum;
//                keymsg.status = KEY_UP;
//                send_key_msg(keymsg);                       //发送按键弹起消息
//            }
//
//            keytime[keynum] = 0;                            //时间累计清0
//        }
//    }
//}

