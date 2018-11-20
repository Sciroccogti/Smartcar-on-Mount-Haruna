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
#define ERASE_VERITF_ALL_BLOCKS             0x01  // ����������������
#define ERASE_VERITF_BLOCKS                 0x02  // �����������ݿ�
#define ERASE_VERITF_FLASH_SECTION          0x03  // ��������Flash ��
#define READ_ONCE                           0x04  // ��ȡһ��
#define PROGRAM_FLASH                       0x06  // ���Flash
#define PROGRAM_ONCE                        0x07  // ���һ��
#define ERASE_ALL_BLOCKS                    0x08  // ������������
#define ERASE_FLASH_BLOCKS                  0x09  // ����Flash ����
#define ERASE_FLASH_SECTOR                  0x0A  // ����Flash ����
#define UNSECURE_FLASH                      0x0B  // ���ܵ�Flash
#define VERITF_BACKDOOR_ACCESS_KEY          0x0C  // ������ŷ�����Կ
#define SET_USER_MARGIN_LEVEL               0x0D  // �����û�ԣ��ˮƽ
#define SET_FACTORY_MARGIN_LEVEL            0x0E  // ���ó���ԣ��ˮƽ
#define CONFIGURE_NVM                       0x0F  // ����NVM

volatile uint8 s_flash_command_run[] = {0x00, 0xB5, 0x80, 0x21, 0x01, 0x70, 0x01, 0x78, 0x09, 0x06, 0xFC, 0xD5,0x00, 0xBD};
typedef void (*flash_run_entry_t)(volatile uint8 *reg);
flash_run_entry_t s_flash_run_entry;

//�ڲ�ʹ��
uint8 FlashCmdStart(void)
{
  DisableInterrupts;

  FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;
  MCM->PLACR = MCM->PLACR | MCM_PLACR_ESFC_MASK;
  s_flash_run_entry = (flash_run_entry_t)((uint32)s_flash_command_run + 1);
  s_flash_run_entry(&FTMRE->FSTAT);           //��CCIFд1��������ִ��(����CCIF)
  MCM->PLACR = MCM->PLACR & (~MCM_PLACR_ESFC_MASK);
  EnableInterrupts;

  if(FTMRE->FSTAT & (FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_MGSTAT_MASK)) return 1;	//���ִ���
  return 0;												//�ɹ�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��ʼ��
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void FLASH_Init(void)
{
  uint16 clk;

  SIM->SCGC |= SIM_SCGC_FLASH_MASK;                                   //����flashʱ��         

  while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //�ȴ�֮ǰ�������(�ȴ�CCIF��λ)

  clk = bus_clk_khz/1000-1;

  FTMRE->FCLKDIV = FTMRE_FCLKDIV_FDIV(clk) | FTMRE_FCLKDIV_FDIVLCK_MASK;//����flash��Ƶϵ��20MHz-1Mhz(0x13)

  while(!(FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIVLD_MASK));                //����Ը�λ���Ƿ��Ѿ������˷�Ƶ
  while((FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIV_MASK)!=clk);              //�ϸ����Ƶϵ��!����ȷ�ķ�Ƶϵ��������flash

  FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;     //���״̬��ʶ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��������
//  @param      SectorNum 		��Ҫ�������������
//  @note                               ����λ��ȡΪ1�����λ��ȡΪ0
//  @return     				����1����ʧ�ܣ�����0�����ɹ�
//  @since      v1.0
//  Sample usage:               uint32 dat = FLASH_GetSectorSize(10);
//-------------------------------------------------------------------------------------------------------------------
uint8 FLASH_EraseSector(uint32 SectorNum)
{
  uint32 addr;
  uint16 clk;
  addr = (uint32)SectorNum*SECTOR_SIZE;
  clk = bus_clk_khz/1000-1;

  if((FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIV_MASK)==clk)              //�ϸ����Ƶϵ��!����ȷ�ķ�Ƶϵ��������flash
  {
    while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //�ȴ���һ������������ɺ��CCIF��λ
  }
  else
  {
    while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //�ȴ���ʼ��������ɺ��CCIF��λ
    FTMRE->FCLKDIV = FTMRE_FCLKDIV_FDIV(clk) | FTMRE_FCLKDIV_FDIVLCK_MASK;//����flash��Ƶϵ��20MHz-1Mhz(0x13)
    while(!(FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIVLD_MASK));                //����Ը�λ���Ƿ��Ѿ������˷�Ƶ
    while((FTMRE->FCLKDIV & FTMRE_FCLKDIV_FDIV_MASK)!=clk);              //�ϸ����Ƶϵ��!����ȷ�ķ�Ƶϵ��������flash
  }
  if(FTMRE->FSTAT & (FTMRE_FSTAT_ACCERR_MASK|FTMRE_FSTAT_FPVIOL_MASK))  //���ACCERR��FPVIOL�Ƿ���λ(���ʴ������д�뱣��������)
    FTMRE->FSTAT &= ~(FTMRE_FSTAT_ACCERR_MASK|FTMRE_FSTAT_FPVIOL_MASK);                         
      
  FTMRE->FCCOBIX = 0;
  FTMRE->FCCOBHI = ERASE_FLASH_SECTOR;
  FTMRE->FCCOBLO = addr>>16;                  //FLASH���ݿ�ȫ�ֵ�ַ

  FTMRE->FCCOBIX = 1;
  FTMRE->FCCOBHI = (addr&0xffff)>>8;
  FTMRE->FCCOBLO = addr&0xff;                 //��������������λ���ϵ�ȫ�ֵ�ַ

  FlashCmdStart();

  while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));                       //�ȴ�����������ɺ��CCIF��λ

  return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH����д�뵽����
//  @param      SectorNum 		��Ҫд����������
//  @param      *buf	 		�����׵�ַ
//  @param      len		 		д����ֽ��� �ر�����һ��uint16�����������ֽ� һ��uint32�������ĸ��ֽ� ÿ��д��ó��ȱ���Ϊ4�ñ���
//  @param      offset		 	����Ϊ4��������
//  @return     				����1д��ʧ�ܣ�����0д��ɹ�
//  @since      v1.0
//  Sample usage:               FLASH_WriteSector(10,(const uint8 *)buf,4,0);//��buf���������ǰ��λ����д�뵽10������(ÿһλ��uint8����)
//  @note                       ʹ��flashʱ����ô����һ��������ʼʹ�ã����һ��������Ϊ255
//  @note                       ʹ��flashǰ��ȷ���Ѿ��Ը���������˲���������
//  @note                       ȥ��ע�����ݿ��Խ���4�ֽڵ�λ����ת��Ϊ��8�ֽڵ�λ����,�ٶȸ���
//                              �����׷�����ٶ�,������4�ֽ���С��λ����,���и��ߵļ�����
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
    FTMRE->FCCOBLO = addr>>16;      //blockȫ�ֵ�ַ
    
    FTMRE->FCCOBIX = 1;
    FTMRE->FCCOBHI = (addr&0xFFFF)>>8;
    FTMRE->FCCOBLO = addr&0xFC;     //�ֽ�ȫ�ֵ�ַ
    
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
  FTMRE->FCCOBLO = addr>>16;      //blockȫ�ֵ�ַ
  
  FTMRE->FCCOBIX = 1;
  FTMRE->FCCOBHI = (addr&0xFFFF)>>8;
  FTMRE->FCCOBLO = addr&0xFC;     //�ֽ�ȫ�ֵ�ַ
  
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
    buf[temp]=((uint8*)&data)[temp];//���
  }
  uint32 addr;
  addr = (uint32)SectorNum*SECTOR_SIZE + offset;
  
  FTMRE->FCCOBIX = 0;
  FTMRE->FCCOBHI = PROGRAM_FLASH;
  FTMRE->FCCOBLO = addr>>16;      //blockȫ�ֵ�ַ
  
  FTMRE->FCCOBIX = 1;
  FTMRE->FCCOBHI = (addr&0xFFFF)>>8;
  FTMRE->FCCOBLO = addr&0xFC;     //�ֽ�ȫ�ֵ�ַ
  
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
  ((uint32*)&data)[0]=datah;//�ϲ�
  ((uint32*)&data)[1]=datal;

  return data;
}
