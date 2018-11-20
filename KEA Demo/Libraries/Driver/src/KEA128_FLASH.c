/*
  ******************************************************************************
  * @file    flash.c
  * @author  xhJiang
  * @version v1.1
  * @date    2018.6.5
  * @mail:   blog_xhjiang@163.com
  * @brief   FLASH configure
  ******************************************************************************
  */
#include "KEA128_FLASH.h"

// flash commands 
#define ERASE_VERITF_ALL_BLOCKS             0x01  // 擦除检验所有区块
#define ERASE_VERITF_BLOCKS                 0x02  // 擦除检验数据块
#define ERASE_VERITF_FLASH_SECTION          0x03  // 擦除检验Flash 段
#define READ_ONCE                           0x04  // 读取一次
#define PROGRAM_FLASH                       0x06  // 编程Flash
#define PROGRAM_ONCE                        0x07  // 编程一次
#define ERASE_ALL_BLOCKS                    0x08  // 擦除所有区块
#define ERASE_FLASH_BLOCKS                  0x09  // 擦除Flash 区块
#define ERASE_FLASH_SECTOR                  0x0A  // 擦除Flash 扇区
#define UNSECURE_FLASH                      0x0B  // 解密的Flash
#define VERITF_BACKDOOR_ACCESS_KEY          0x0C  // 检验后门访问密钥
#define SET_USER_MARGIN_LEVEL               0x0D  // 设置用户裕量水平
#define SET_FACTORY_MARGIN_LEVEL            0x0E  // 设置出厂裕量水平
#define CONFIGURE_NVM                       0x0F  // 配置NVM

volatile uint8 s_flash_command_run[] = {0x00, 0xB5, 0x80, 0x21, 0x01, 0x70, 0x01, 0x78, 0x09, 0x06, 0xFC, 0xD5,0x00, 0xBD};
typedef void (*flash_run_entry_t)(volatile uint8 *reg);
flash_run_entry_t s_flash_run_entry;

//内部使用
uint8 FlashCmdStart(void)
{
  DisableInterrupts;

  FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;
  MCM->PLACR = MCM->PLACR | MCM_PLACR_ESFC_MASK;
  s_flash_run_entry = (flash_run_entry_t)((uint32)s_flash_command_run + 1);
  s_flash_run_entry(&FTMRE->FSTAT);           //向CCIF写1启动命令执行(清零CCIF)
  MCM->PLACR = MCM->PLACR & (~MCM_PLACR_ESFC_MASK);
  EnableInterrupts;

  if(FTMRE->FSTAT & (FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_MGSTAT_MASK)) return 1;	//出现错误
  return 0;												//成功
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH初始化
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void FLASH_Init(void)
{
  uint16 clk;

  SIM->SCGC |= SIM_SCGC_FLASH_MASK;                                   //开启flash时钟         

  while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //等待之前命令完成(等待CCIF置位)

  clk = bus_clk_khz/1000-1;

  FTMRE->FCLKDIV = FTMRE_FCLKDIV_FDIV(clk) | FTMRE_FCLKDIV_FDIVLCK_MASK;//设置flash分频系数20MHz-1Mhz(0x13)

  while(!(FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIVLD_MASK));                //检查自复位后是否已经加载了分频
  while((FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIV_MASK)!=clk);              //严格检查分频系数!不正确的分频系数将会损坏flash

  FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;     //清除状态标识
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH擦除扇区
//  @param      SectorNum 		需要擦除的扇区编号
//  @note                               擦除位读取为1，编程位读取为0
//  @return     				返回1擦除失败，返回0擦除成功
//  @since      v1.0
//  Sample usage:               uint32 dat = FLASH_GetSectorSize(10);
//-------------------------------------------------------------------------------------------------------------------
uint8 FLASH_EraseSector(uint32 SectorNum)
{
  uint32 addr;
  uint16 clk;
  addr = (uint32)SectorNum*SECTOR_SIZE;
  clk = bus_clk_khz/1000-1;

  if((FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIV_MASK)==clk)              //严格检查分频系数!不正确的分频系数将会损坏flash
  {
    while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //等待上一次命令序列完成后的CCIF置位
  }
  else
  {
    while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //等待初始化序列完成后的CCIF置位
    FTMRE->FCLKDIV = FTMRE_FCLKDIV_FDIV(clk) | FTMRE_FCLKDIV_FDIVLCK_MASK;//设置flash分频系数20MHz-1Mhz(0x13)
    while(!(FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIVLD_MASK));                //检查自复位后是否已经加载了分频
    while((FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIV_MASK)!=clk);              //严格检查分频系数!不正确的分频系数将会损坏flash
  }
  if(FTMRE->FSTAT & (FTMRE_FSTAT_ACCERR_MASK|FTMRE_FSTAT_FPVIOL_MASK))  //检查ACCERR或FPVIOL是否置位(访问错误和误写入保护区域检查)
    FTMRE->FSTAT &= ~(FTMRE_FSTAT_ACCERR_MASK|FTMRE_FSTAT_FPVIOL_MASK);                         
      
  FTMRE->FCCOBIX = 0;
  FTMRE->FCCOBHI = ERASE_FLASH_SECTOR;
  FTMRE->FCCOBLO = addr>>16;                  //FLASH数据块全局地址

  FTMRE->FCCOBIX = 1;
  FTMRE->FCCOBHI = (addr&0xffff)>>8;
  FTMRE->FCCOBLO = addr&0xff;                 //待擦除扇区任意位置上的全局地址

  FlashCmdStart();

  while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //等待命令序列完成后的CCIF置位

  return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH数据写入到扇区
//  @param      SectorNum 		需要写入的扇区编号
//  @param      *buf	 		数据首地址
//  @param      len		 		写入的字节数 特别提醒一个uint16变量是两个字节 一个uint32变量是四个字节 每次写入得长度必须为4得倍数
//  @param      offset		 	必须为4的整数倍
//  @return     				返回1写入失败，返回0写入成功
//  @since      v1.0
//  Sample usage:               FLASH_WriteSector(10,(const uint8 *)buf,4,0);//将buf数组里面的前四位数据写入到10号扇区(每一位是uint8类型)
//  @note                       使用flash时，最好从最后一个扇区开始使用，最后一个扇区号为255
//  @note                       使用flash前，确保已经对该区域进行了擦除操作！
//  @note                       去掉注释内容可以将以4字节单位传输转换为以8字节单位传输,速度更快
//                              如果不追求编程速度,建议以4字节最小单位传输,具有更高的兼容性
//-------------------------------------------------------------------------------------------------------------------

uint8 FLASH_WriteSector(uint32 SectorNum, const uint8 *buf, uint32 len, uint32 offset)
{
  uint16 step, i;
  uint32 addr;

  addr = (uint32)SectorNum*SECTOR_SIZE + offset;

  step = 4;
//  step = 8;

  for(i=0; i<len; i+=step)
  {
    FTMRE->FCCOBIX = 0;
    FTMRE->FCCOBHI = PROGRAM_FLASH;
    FTMRE->FCCOBLO = addr>>16;      //block全局地址
    
    FTMRE->FCCOBIX = 1;
    FTMRE->FCCOBHI = (addr&0xFFFF)>>8;
    FTMRE->FCCOBLO = addr&0xFC;     //字节全局地址
    
    FTMRE->FCCOBIX = 2;
    FTMRE->FCCOBLO = buf[0];
    FTMRE->FCCOBHI = buf[1];
    
    FTMRE->FCCOBIX = 3;
    FTMRE->FCCOBLO = buf[2];
    FTMRE->FCCOBHI = buf[3];

//    FTMRE->FCCOBIX = 4;
//    FTMRE->FCCOBLO = buf[4];
//    FTMRE->FCCOBHI = buf[5];
//  
//    FTMRE->FCCOBIX = 5;
//    FTMRE->FCCOBLO = buf[6];
//    FTMRE->FCCOBHI = buf[7];

    buf += step;
    addr += step;

    FlashCmdStart();
  }
  return 0;
}


void FLASH_WriteFloat(uint32 SectorNum,const float data, uint32 offset)
{
  uint32 float_int;
  float_int = *(uint32 *)&data;
  uint8 buf[4];
  buf[0]=(0x00ff)&float_int;
  float_int = float_int>>8;
  buf[1]=(0x00ff)&float_int;
  float_int = float_int>>8;
  buf[2]=(0x00ff)&float_int;
  float_int = float_int>>8;
  buf[3]=(0x00ff)&float_int;
  
  uint32 addr;
  addr = (uint32)SectorNum*SECTOR_SIZE + offset;
  
  FTMRE->FCCOBIX = 0;
  FTMRE->FCCOBHI = PROGRAM_FLASH;
  FTMRE->FCCOBLO = addr>>16;      //block全局地址
  
  FTMRE->FCCOBIX = 1;
  FTMRE->FCCOBHI = (addr&0xFFFF)>>8;
  FTMRE->FCCOBLO = addr&0xFC;     //字节全局地址
  
  FTMRE->FCCOBIX = 2;
  FTMRE->FCCOBLO = buf[0];
  FTMRE->FCCOBHI = buf[1];
  
  FTMRE->FCCOBIX = 3;
  FTMRE->FCCOBLO = buf[2];
  FTMRE->FCCOBHI = buf[3];
  
  FlashCmdStart();
  
}
    
void FLASH_WriteDouble(uint32 SectorNum,const double data, uint32 offset)
{
  uint8 buf[8];
 
  uint8 temp;
  for(temp=0; temp<8; temp++)
  {
    buf[temp]=((uint8*)&data)[temp];//拆分
  }
  uint32 addr;
  addr = (uint32)SectorNum*SECTOR_SIZE + offset;
  
  FTMRE->FCCOBIX = 0;
  FTMRE->FCCOBHI = PROGRAM_FLASH;
  FTMRE->FCCOBLO = addr>>16;      //block全局地址
  
  FTMRE->FCCOBIX = 1;
  FTMRE->FCCOBHI = (addr&0xFFFF)>>8;
  FTMRE->FCCOBLO = addr&0xFC;     //字节全局地址
  
  FTMRE->FCCOBIX = 2;
  FTMRE->FCCOBLO = buf[0];
  FTMRE->FCCOBHI = buf[1];
  
  FTMRE->FCCOBIX = 3;
  FTMRE->FCCOBLO = buf[2];
  FTMRE->FCCOBHI = buf[3];
  
  FTMRE->FCCOBIX = 4;
  FTMRE->FCCOBLO = buf[4];
  FTMRE->FCCOBHI = buf[5];

  FTMRE->FCCOBIX = 5;
  FTMRE->FCCOBLO = buf[6];
  FTMRE->FCCOBHI = buf[7];

  FlashCmdStart();
}

double flash_read_double(uint32 SectorNum,uint32 offset)
{
  uint32 datah,datal;
  
  datah=flash_read(SectorNum,offset,uint32);
  offset+=4;
  datal=flash_read(SectorNum,offset,uint32);
  
  double data;
  ((uint32*)&data)[0]=datah;//合并
  ((uint32*)&data)[1]=datal;

  return data;
}
