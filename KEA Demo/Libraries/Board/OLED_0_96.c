/*
  ******************************************************************************
  * @file    OLED_0_96.c
  * @author  xhJiang
  * @version v1.2
  * @date    2018.6.5
  * @mail:   blog_xhjiang@163.com
  * @brief   0.96����Ļ configure
  * @note    ͨ������
  ******************************************************************************
  */
#include "OLED_0_96.h"
#include "delay.h"


#if (OLED_CATAGORY == 1)
//��Ҫ��OLED��Ļʼ����ʾ��ֹ����!��֤ÿ�����ص㲻��ʼ�ձ�����,�������ʹ������

//each page is 8 row.
//8 pages,128 col.
//each row and col intersect is a seg,which can be lightened individually.
//refresh at least one page for one time.refresh only one pixel is not permitted.
//������ֿ��Ե����ص�ˢ�����֪лл��
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		 
unsigned char OLED_GRAM[128][8];

/**********************************************
// IIC Write byte
// ��SSD1306д��һ���ֽڡ�
// dat:Ҫд�������/����
// cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
**********************************************/
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
  unsigned char data_temp = dat;
  unsigned char i;
  if(cmd!=0) cmd = 1;//��0��1,��ͬBool
  OLED_DC(cmd);
  OLED_D0(0);
  for(i=0;i<8;i++)
  {
    if(data_temp & 0x80){ OLED_D1(1);}
    else{	OLED_D1(0);}
    OLED_D0(1);
    OLED_D0(0);
    data_temp <<= 1;
  }
  OLED_DC(1);//�����������־��λ����,��ֹ����״̬������������
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʾ��������
//  @param      x			x����������(0<=x<=127)��x>127,��x=x%128;
//  @param      y			y����������(0<=y<=63)��x>63,��x=x%64;
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Set_Pos(uint8 x, uint8 y)
{
  uint8 page = y/8;
  page %= 8;
  OLED_WR_Byte(0xb0+page,0);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
  OLED_WR_Byte((x&0x0f)|0x01,0);
}

//�����Դ浽OLED��ʾ����
//�൱��ȫ��ˢ���Դ�ֵ
void OLED_Refresh_Gram(void)
{
  unsigned char i,n;
  for(i=0;i<8;i++)
  {
    OLED_WR_Byte ((0xb0+i),0);    //����ҳ��ַ(0~7)
    OLED_WR_Byte (0x00,0);      //������ʾλ�á��е͵�ַ
    OLED_WR_Byte (0x10,0);      //������ʾλ�á��иߵ�ַ
    for(n=0;n<OLED_X_WIDTH;n++) OLED_WR_Byte(OLED_GRAM[n][i],1);
  }
}
//���²����Դ浽OLED��ʾ����
//�൱�ھֲ�ˢ���Դ�ֵ
void OLED_Refresh_Gram_Page(uint8 x,uint8 y)
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  uint8 page;   //y�������ڵ�page
  page = y/8;
  OLED_Set_Pos(x,y);
  OLED_WR_Byte(0xb0+(y>>3),0);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
  OLED_WR_Byte((x&0x0f)|0x00,0);
  OLED_WR_Byte(OLED_GRAM[x][page],1);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��������
//  @param      fill_data	�����ɫ(0x00 or 0xff)
//                              00Ϊȫ��,FFΪȫ��
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(unsigned char fill_data)
{
  unsigned char i,n;
  for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=fill_data;
  OLED_Refresh_Gram();//������ʾ
}
//ҳ�������,ֻ����Դ�����
//��Ҫ������ͬ�ľֲ�ˢ�º�����ˢ��!
void OLED_Clear_Page(uint8 x,uint8 y,unsigned char fill_data)
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  uint8 page;   //y�������ڵ�page
  page = y/8;
  OLED_GRAM[x][page]=fill_data;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʼ������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Init()
{
//  Initial GPIO PIN
  GPIO_Init(OLED_D0_PIN,GPO,0);
  GPIO_Init(OLED_D1_PIN,GPO,0);
  GPIO_Init(OLED_RST_PIN,GPO,1);
  GPIO_Init(OLED_DC_PIN,GPO,0);
//  GPIO_Init(OLED_CS_PIN,GPO,0);
  
//Initialized State(Parameters as Default)
  OLED_RST(0);
  Soft_Delay_ms(100);
  OLED_RST(1);
  Soft_Delay_us(100);
//Initial_M096128x64_ssd1306
  OLED_WR_Byte(0xae,0);//display off
                         //Initial Settings Configuration
  OLED_WR_Byte(0xd5,0);//--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80,0);//--set divide ratio

  OLED_WR_Byte(0xa8,0);//--set multiplex ratio(1 to 64)������
  OLED_WR_Byte(0x3F,0);//--1/64 duty

  OLED_WR_Byte(0x00,0);//---set low column address
  OLED_WR_Byte(0x10,0);//---set high column address

  OLED_WR_Byte(0xd3,0);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00,0);//-not offset

  OLED_WR_Byte(0x40,0);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)

  OLED_WR_Byte(0x20,0);//Set Memory Addressing Mode
  OLED_WR_Byte(0x02,0);//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET,0);11,Invalid
  OLED_WR_Byte(0xb0,0);//Set Page Start Address for Page Addressing Mode,0-7

  OLED_WR_Byte(0xa1,0);//--set segment re-map 0 to 127--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����

  OLED_WR_Byte(0xc8,0);//Set COM Output Scan Direction--Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  //ע��a0c0��a1c8��������Ļ��ʾ������
  

  OLED_WR_Byte(0xda,0);//--set com pins hardware configuration
  OLED_WR_Byte(0x12,0);

  OLED_WR_Byte(0x81,0);//--set contrast control register
  OLED_WR_Byte(0x0f,0);// Set SEG Output Current Brightness0x66

  OLED_WR_Byte(0xd9,0);//--set pre-charge period
  OLED_WR_Byte(0xf1,0);//

  OLED_WR_Byte(0xdb,0);//--set vcomh
  OLED_WR_Byte(0x30,0);//Set VCOM Deselect Level

  OLED_WR_Byte(0xa4,0);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content  

  OLED_WR_Byte(0xa6,0);//--set normal display  0Ϊ��1Ϊ��//A7��ʾ0����1��

//Clear Screen
  OLED_WR_Byte(0x8d,0);//--set DC-DC enable
  OLED_WR_Byte(0x14,0);//

  OLED_WR_Byte(0xaf,0);//--turn on oled panel

  Soft_Delay_ms(100);
  OLED_Set_Pos(0,0);
  OLED_Clear(0x00);
}

//��/��OLED��ʾ
void OLED_Display_Config(uint8 cfg)
{
  if(cfg)
  {
    OLED_WR_Byte(0x8d,0);//--set DC-DC enable
    OLED_WR_Byte(0x14,0);//DCDC on

    OLED_WR_Byte(0xaf,0);//--turn on oled panel
    Soft_Delay_ms(100);
    OLED_Set_Pos(0,0);
  }
  else
  {
    OLED_WR_Byte(0x8d,0);//--set DC-DC enable
    OLED_WR_Byte(0x10,0);//DCDC off
    OLED_WR_Byte(0xae,0);//display off
  }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED ����/Ϩ�� һ����
//  @param      x	    x����������
//  @param      y           y����������
//  @param      cfg         1 ��� ,0 ���
//  @param      refresh     1 ����ˢ�¸�ҳ ,0 ��ˢ�¸�ҳ(�ȴ�����OLED_Refresh_Gram����ȫ��ˢ��)
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_DrawPoint(uint8 x,uint8 y,uint8 cfg,uint8 refresh)
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  uint8 data1;  //data1��ǰ�������
  uint8 page;   //y�������ڵ�page
  uint8 pos;    //y��������page�ڵ�λ��
  page = y/8;
  pos = y%8;
  data1 = 0x01<<pos;
  if(cfg)OLED_GRAM[x][page]|=data1;
  else OLED_GRAM[x][page]&=~data1;
  if(refresh)
  {
    OLED_Set_Pos(x,y);
    OLED_WR_Byte(0xb0+(y>>3),0);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
    OLED_WR_Byte((x&0x0f)|0x00,0);
    OLED_WR_Byte(OLED_GRAM[x][page],1);
  }
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
//refresh     1 ����ˢ�¸õ� ,0 ��ˢ�¸õ�(�ȴ�����OLED_Refresh_Gram����ȫ��ˢ��)
//ע���ú���ˢ��Ч�ʺܵͣ����еĲ���������������õģ����Ż������˷���������-��-
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot,uint8 refresh)
{  
  unsigned char x,y;
  for(x=x1;x<=x2;x++)
  {
    for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot,refresh);
  }
}
//OLED����
//x+width��Ҫ����128
//y+height��Ҫ����64
void OLED_Reverse(uint8 width,uint8 height,uint8 x,uint8 y,uint8 refresh)
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  uint8 page;   //y�������ڵ�page
  uint8 pos;    //y��������page�ڵ�λ��
  uint8 tempy = height;
  while(tempy)
  {
    page = y/8;
    pos = y%8;
    uint8 tempx = width;
    while(tempx)
    {
      if(BIT_GET(OLED_GRAM[x][page],pos))  BIT_CLEAN(OLED_GRAM[x][page],pos);
      else   BIT_SET(OLED_GRAM[x][page],pos);
      if(refresh)
      {
        OLED_Set_Pos(x,y);
        OLED_WR_Byte(0xb0+(y>>3),0);
        OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
        OLED_WR_Byte((x&0x0f)|0x00,0);
        OLED_WR_Byte(OLED_GRAM[x][page],1);
      }
      tempx--;
      x++;
    }
    x-=width;
    tempy--;
    y++;
  }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʾ�ַ���
//  @param      width	    ������
//  @param      hight	    ����߶�
//  @param      x	    x����������
//  @param      y           y����������
//  @param      mode        0,������ʾ;1,������ʾ
//  @param      refresh     1 ����ˢ�¸�ҳ ,0 ��ˢ�¸�ҳ(�ȴ�����OLED_Refresh_Gram����ȫ��ˢ��)
//  @param      ch[]        �ַ���
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Show_String(uint8 width,uint8 height,uint8 x,uint8 y,uint8 mode,char ch[],uint8 refresh)
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  //�����ֿ�淶�������Ⱥ͸߶�ֵ
  if(width >= 8) width = 8;
  else if(width <= 6) width = 6;
  else width = 8;
  
  if(height >= 16) height = 16;
  else if(height <= 8) height = 8;
  else if((height <= 12) && (height > 8)) height = 12;
  else height = 16;
  
  uint8 c=0,i=0,j=0,temp=0,y0=y,l=0,pos=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;//�ӿ���ʾ�ַ�" "��ʼ�������Ҫ32��ƫ����
    if(x>(128-width)){x=0;y+=height;y0+=height;}
    uint8 ascii_length;//��ģ����
    ascii_length = (height-1)/8+1;
    ascii_length *= width;
    for(i=0;i<ascii_length;i++)
    {
      if((width==6) && (height==8)) temp = F6x8[c][i];//ע��6x8�ֿ�Ϊ����ȡģ!
      else if((width==8) && (height==16)) temp = asc2_1608[c][i];
      else if((width==6) && (height==12)) temp = asc2_1206[c][i];
      for(l=0;l<8;l++)
      {
        if((width==6)&&(height==8))
        {
          if(temp&0x01)OLED_DrawPoint(x,y,mode,0);//��λ��ǰ����ȡģ,���λ��Ӧ��˳��ȡģ�����λ
          else OLED_DrawPoint(x,y,!mode,0);
          temp>>=1;
        }
        else
        {
          if(temp&0x80)OLED_DrawPoint(x,y,mode,0);//��λ��ǰ˳��ȡģ
          else OLED_DrawPoint(x,y,!mode,0);
          temp<<=1;
        }
        pos = y%8;
        if((pos==7)&&refresh) OLED_Refresh_Gram_Page(x,y);
        y++;
	if((y-y0)==height)
        {
          if(refresh) OLED_Refresh_Gram_Page(x,y);
          y=y0;
          x++;
          break;//����ȡģ�Ĳ���λ
        }
      }
    }
    j++;  
//      ����ķ�����Ȼ��ݣ���ǿ�ƽ�y�̶�����height�ĸ���ڣ���Ч��yֻ��Ϊ����ֵ
//      OLED_Set_Pos(x,y);
//      for(i=0;i<width;i++){    OLED_WR_Byte(ASCII_8x16[c][i],1);}
//      OLED_Set_Pos(x,y+8);
//      for(i=0;i<width;i++){    OLED_WR_Byte(ASCII_8x16[c][i+8],1);}
  }
}

////-------------------------------------------------------------------------------------------------------------------
////  @brief      OLED��ʾ�ַ���(6*8����)
////  @param      x			x����������
////  @param      y           y����������
////  @param      ch[]        �ַ���
////  @return     void
////  @since      v1.0
////  Sample usage:			
////-------------------------------------------------------------------------------------------------------------------
//void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[])
//{
//  uint8 c=0,i=0,j=0;
//  y %= 64;
//  x %= 128;
//  while (ch[j]!='\0')
//  {
//    c =ch[j]-32;
//    if(x>122){x=0;y++;}
//    OLED_Set_Pos(x,y);
//    for(i=0;i<6;i++)	
//    OLED_WR_Byte(F6x8[c][i],1);
//    x+=6;
//    j++;
//  }
//}


//�������ʹ��sprintf������
////����ASCII���е����ֶ�Ӧ
////�ڲ�ʹ���û��������
//void OLED_HEXACSII(uint16 hex,uint8* Print)
//{
//  uint8 hexcheck ;
//  uint8 TEMP ;
//  uint16 temp_num;
//  uint8 length;
//  TEMP = 6 ;
//  Print[TEMP ]='\0';
//  temp_num=hex;
////  while(temp_num)
////  {
////    temp_num/=10;
////    length++;
////  }
//  while(TEMP)
//  {
//    TEMP -- ;
//    hexcheck  =  hex%10 ;
//    hex   /=10 ;
//    Print[TEMP]  = hexcheck + 0x30 ;
////    if(hex==0) {break;}
//  }
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      OLED��ʾ�з�����
////  @param      x	    x����������
////  @param      y           y����������
////  @param      num         �޷�����
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void OLED_Print_Num(uint8 width,uint8 height,uint8 x, uint8 y,uint8 mode, int16 num, uint8 refresh)
//{
//  uint8 *ch1,ch[7];
//  if(num<0)
//  {
//    num = -num;
//    OLED_Show_String(width,height,x, y, mode, "-",refresh);
//    x+=width;
//    if(x>(128-width)){x=0;y+=height;}
//  }
//  OLED_HEXACSII((uint16)num,ch);
//  ch1 =&ch[1];
//  OLED_Show_String(width,height,x, y, mode, ch,refresh);//��ʾ����
//  
////  while(*ch1 != '\0')
////  {
////    OLED_Show_Char(width,height,x, y, mode, ch1,refresh);
////    x += width;
////    ch1++;
////  }
//}

//��ʾͼƬ���ܴ�����
////-------------------------------------------------------------------------------------------------------------------
////  @brief      OLED��ʾͼ��
////  @param      high  		ͼ��߶�
////  @param      width 		ͼ����
////  @param      *p    		ͼ���ַ�����飩
////  @param      value 		��ֵ����ֵ
////  @return     void
////  @since      v1.0
////  Sample usage:			
////-------------------------------------------------------------------------------------------------------------------
//void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value)
//{
//	int16 i,j;
//	int16 temp,temp1;
//	uint8 dat;
//
//
//	temp1 = high%8;
//	if(temp1 == 0) temp = high/8;
//	else           temp = high/8+1;
//
//
//	for(i=0; i<temp; i++)
//	{
//		OLED_Set_Pos(0,i);
//		for(j=0; j<width; j++)
//		{
//			dat = 0;
//			if( i<(temp-1) || !temp1 || temp1>=1)dat |= (*(p+i*8*width+j+width*0) > value? 1: 0)<<0;
//			if( i<(temp-1) || !temp1 || temp1>=2)dat |= (*(p+i*8*width+j+width*1) > value? 1: 0)<<1;
//			if( i<(temp-1) || !temp1 || temp1>=3)dat |= (*(p+i*8*width+j+width*2) > value? 1: 0)<<2;
//			if( i<(temp-1) || !temp1 || temp1>=4)dat |= (*(p+i*8*width+j+width*3) > value? 1: 0)<<3;
//			if( i<(temp-1) || !temp1 || temp1>=5)dat |= (*(p+i*8*width+j+width*4) > value? 1: 0)<<4;
//			if( i<(temp-1) || !temp1 || temp1>=6)dat |= (*(p+i*8*width+j+width*5) > value? 1: 0)<<5;
//			if( i<(temp-1) || !temp1 || temp1>=7)dat |= (*(p+i*8*width+j+width*6) > value? 1: 0)<<6;
//			if( i<(temp-1) || !temp1 || temp1>=8)dat |= (*(p+i*8*width+j+width*7) > value? 1: 0)<<7;
//
//			OLED_WrDat(dat);
//		}
//	}
//}

/******************************************
// picture������ʾһ��ͼƬ
******************************************/
void OLED_Picture(uint16 high, uint16 width,uint8 *p)
{
  unsigned char x,y;
  unsigned int i=0;
  for(y=0;y<8;y++)
    {
      OLED_WR_Byte(0xb0+y,0);
      OLED_WR_Byte(0x0,0);
      OLED_WR_Byte(0x10,0);
      for(x=0;x<128;x++)
        {
          OLED_WR_Byte(p[i++],1);
        }
    }
}

#elif (OLED_CATAGORY==2)

/**********************************************
// SPI Write byte
// ��SSD1331д��һ���ֽڡ�
// dat:Ҫд�������/����
// cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
**********************************************/

void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
  if(cmd!=0) cmd = 1;//��0��1,��ͬBool
  unsigned char i;			  
  OLED_DC(cmd);
//  OLED_CS(0);

  for(i=0;i<8;i++)
  {			  
    OLED_D0(0);
    if(dat&0x80)   {OLED_D1(1);}
    else {OLED_D1(0);}
    OLED_D0(1);
    dat<<=1;   
  }
//  OLED_CS(1);
  OLED_DC(1);//�����������־��λ����,��ֹ����״̬������������ 	  
} 




//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʼ������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Init()
{
//  Initial GPIO PIN
  GPIO_Init(OLED_D0_PIN,GPO,0);
  GPIO_Init(OLED_D1_PIN,GPO,0);
  GPIO_Init(OLED_RST_PIN,GPO,1);
  GPIO_Init(OLED_DC_PIN,GPO,0);
//  GPIO_Init(OLED_CS_PIN,GPO,0);
  
//Initialized State(Parameters as Default)
//  OLED_CS(0);
  OLED_RST(0);
  Soft_Delay_ms(100);
  OLED_RST(1);
  Soft_Delay_us(100);
  //Initial_SSD1331
  OLED_WR_Byte(0xAE,0);//Display OFF
  
  OLED_WR_Byte(0xa4,0); //set Normal Display
  
  OLED_WR_Byte(0xb3,0);//Set Display Clock Divider /Oscillator Frequency
  OLED_WR_Byte(0xd0,0);

  OLED_WR_Byte(0xa2,0);//set display offset
  OLED_WR_Byte(0x00,0);
  
  OLED_WR_Byte(0xa8,0); //set Multiplex Ratio
  OLED_WR_Byte(0x3f,0);

  OLED_WR_Byte(0xad,0); //set master configuration
  OLED_WR_Byte(0x8e,0);//external vcc selected
  
  OLED_WR_Byte(0xa1,0);//set display start line
  OLED_WR_Byte(0x00,0);
  
  OLED_WR_Byte(0xa0,0);//Set Re-map & color depth
//8λ����˵��A[7:0]
//A[0]:0-��ɨ��ģʽ,ÿ��д�Դ�ʱ,�е�ַ+1,�е�ַ=95ʱ,�е�ַ+1;  1-��ɨ��ģʽ,ÿ��д�Դ�ʱ,�е�ַ+1,�е�ַ=63ʱ,�е�ַ+1
//A[1]:0-�Դ�map��colomn0��ӦSeg0,Colomn95��ӦSeg95;  1-�Դ�map��colomn0��ӦSeg95,Colomn95��ӦSeg0;(��Ч��ˮƽ����)
//A[2]:0-"A"="R","B"="G","C"="B" order; 1-"A"="B","B"="G","C"="R" order;
//A[3]:
//A[4]:
//A[5]:
//A[7-6]:��ɫ��ʾ;0-256ɫģʽ,1-64Kȫ��ģʽ1,2-64Kȫ��ģʽ2
  OLED_WR_Byte(0x72,0);//65K color1 , A=R,B=G,C=B order//74
  
  OLED_WR_Byte(0x87,0); //set High Brightness(Current Control)
  OLED_WR_Byte(0x06,0);
  
  //�趨3��ɫ�Աȶ�,��ɫΪABC,��A0����ָ��"ABC"��˳��Ϊ"RGB"��"BGR"
  OLED_WR_Byte(0x81,0); //set Contrast color A
  OLED_WR_Byte(0x80,0);//0x91
  OLED_WR_Byte(0x82,0); //set Contrast color B
  OLED_WR_Byte(0x80,0);//0x50
  OLED_WR_Byte(0x83,0); //set Contrast color C
  OLED_WR_Byte(0x80,0);//0x7D

  OLED_WR_Byte(0xbb,0);//Set Pre-charge level
  OLED_WR_Byte(0x3a,0);
  
  //Set Second Pre-charge Speed for Color "A,B,C"
  //����һ�����ص��Ϊ��������,��һ��Ԥ���ܺ͵ڶ��γ��,�ڵڶ��γ��Ĺ�����,�����Ŀ��ֵ�趨Ϊ�����ֵ,����ʹ����ٶȸ���,�������ٶȸ���(���������Ȳ���仯)
  //����ʹ��PID���ڵ�˼�����ù���,Ĭ�ϵڶ��γ���Ŀ��ֵ��RGB�ĶԱȶ��趨��ͬ
//  OLED_WR_Byte(0x8A,0);//set color A Segment Second charge speed
//  OLED_WR_Byte(0x64,0);
//  OLED_WR_Byte(0x8B,0);//set color B Segment Second charge speed
//  OLED_WR_Byte(0x78,0);
//  OLED_WR_Byte(0x8C,0);//set color C Segment Second charge speed
//  OLED_WR_Byte(0x64,0);
  

  OLED_WR_Byte(0xBE,0);//set VCOMH
  OLED_WR_Byte(0x3E,0);

  OLED_WR_Byte(0xB1,0);//set phase 1&2 Period Adjustment
  OLED_WR_Byte(0x31,0);
  
  OLED_WR_Byte(0xB0,0);//power save mode
  OLED_WR_Byte(0x0b,0);//disable
 
  
//addition
  
//  OLED_WR_Byte(0xb9,0);//Enable Gray Scale Table
//  OLED_WR_Byte(0xb8,0);//Set Gray Scale Table
//  OLED_WR_Byte(0x01,0);
//  OLED_WR_Byte(0x05,0);
//  OLED_WR_Byte(0x09,0);
//  OLED_WR_Byte(0x0d,0);
//  OLED_WR_Byte(0x11,0);
//  OLED_WR_Byte(0x15,0);
//  OLED_WR_Byte(0x19,0);
//  OLED_WR_Byte(0x1d,0);
//  OLED_WR_Byte(0x21,0);
//  OLED_WR_Byte(0x25,0);
//  OLED_WR_Byte(0x29,0);
//  OLED_WR_Byte(0x2d,0);
//  OLED_WR_Byte(0x31,0);
//  OLED_WR_Byte(0x35,0);
//  OLED_WR_Byte(0x39,0);
//  OLED_WR_Byte(0x3d,0);
//  OLED_WR_Byte(0x41,0);
//  OLED_WR_Byte(0x45,0);
//  OLED_WR_Byte(0x49,0);
//  OLED_WR_Byte(0x4d,0);
//  OLED_WR_Byte(0x51,0);
//  OLED_WR_Byte(0x55,0);
//  OLED_WR_Byte(0x59,0);
//  OLED_WR_Byte(0x5d,0);
//  OLED_WR_Byte(0x61,0);
//  OLED_WR_Byte(0x65,0);
//  OLED_WR_Byte(0x69,0);
//  OLED_WR_Byte(0x6d,0);
//  OLED_WR_Byte(0x71,0);
//  OLED_WR_Byte(0x75,0);
//  OLED_WR_Byte(0x79,0);
//  OLED_WR_Byte(0x7d,0);
  
  
  OLED_WR_Byte(0xaf,0); //set Display on

  OLED_WR_Byte(0x15,0);//Setup Column--
  OLED_WR_Byte(0x00,0);//--start address
  OLED_WR_Byte(95,0);//--end address
  
  OLED_WR_Byte(0x75,0);//Setup Row--
  OLED_WR_Byte(0x00,0);//--start address
  OLED_WR_Byte(63,0);//--end address
  
}
/*------------------------------------------------------------------------------------
//�����趨
//����:duty 0-15,����15��ֵ����Ϊ15
//duty���Ƶ���˥��,0Ϊ1/16�ĵ�������,1Ϊ2/16,15Ϊ16/16�ĵ�������,dutyԽ����Ļ����Խ��
//���Ҫʹ�������,������Ҫ��������Ϊ15,����Ҫ���ԱȶȾ�����Ϊ255
------------------------------------------------------------------------------------*/
void OLED_Brightness_Set(uint8 duty)
{
  //��������
  if(duty>15) duty = 15;
  OLED_WR_Byte(0x87,0); //set High Brightness(Current Control)
  OLED_WR_Byte(duty,0);
}

/*-------------------------------------------------------------------------------------
//�Աȶ��趨
//����:red/green/blue ��Χ:0-255
//�Աȶ�Ϊ�ڸ�����ɫ�����Ȼ�����,�Աȶ�Խ��ᵼ��:���ȵĽ��ݲ���ֵ�ĸ���;�����Ȳ�����ͬ������Ļ����

--------------------------------------------------------------------------------------*/
void OLED_Contrast_Set(uint8 red,uint8 green,uint8 blue)
{
  OLED_WR_Byte(0x81,0); //set Contrast A
  OLED_WR_Byte(red,0);
  OLED_WR_Byte(0x82,0); //set Contrast B
  OLED_WR_Byte(green,0);
  OLED_WR_Byte(0x83,0); //set Contrast C
  OLED_WR_Byte(blue,0);
}

/*---------------------------
//OLED����IC����,�����ܳ����������������������
---------------------------------*/
void OLED_Lock_IC(void)
{
  OLED_WR_Byte(0xFD,0);
  OLED_WR_Byte(0x22,0);
}

/*---------------------------
//OLED����IC����
---------------------------------*/
void OLED_Unlock_IC(void)
{
  OLED_WR_Byte(0xFD,0);
  OLED_WR_Byte(0x12,0);
}

/*-------------------------------
//������ʾ
-----------------------------------*/
void OLED_Display_On(void)
{
  OLED_WR_Byte(0xaf,0); //set Display on
}

/*-------------------------------
//�ر���ʾ
-----------------------------------*/
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0xae,0); //set Display off
}

/*-------------------------------
//�����Ҷ���ʾģʽ
-----------------------------------*/
void OLED_Display_Dim(void)
{
  OLED_WR_Byte(0xac,0); //set dim Display
}
/*----------------------------------
//����ģʽ
//����cfg=1,����ģʽʹ��;cfg=0,����ģʽ����
-----------------------------------*/
void OLED_PowerSave(uint8 cfg)
{
  OLED_WR_Byte(0xB0,0);//power save mode
  if(cfg)
    OLED_WR_Byte(0x1a,0);//enable
  else
    OLED_WR_Byte(0x0b,0);//disable
}
/*-------------------------------
//����
//����:   x1 ����ʼ��ַ
//        y1 ����ʼ��ַ
//        x2 �н�����ַ
//        y2 �н�����ַ
//ע������ִ�к���,�����Ҫ���һ������Ļ,��Ҫȷ������һ��OLEDд����ǰ�г�ֵ�ִ��ʱ��,������Ҫ�ֶ��ṩһ����ʱ
//��Ĭ�ϵ�ʱ��������������Ҫ����400us����ʱ
//  Soft_Delay_us(400);
-----------------------------------*/
void OLED_Clear_Window(uint8 x1,uint8 y1,uint8 x2,uint8 y2)
{
  OLED_WR_Byte(0x25,0);
  OLED_WR_Byte(x1,0);
  OLED_WR_Byte(y1,0);
  OLED_WR_Byte(x2,0);
  OLED_WR_Byte(y2,0);
}

/*-------------------------------
//�ڲ����ɻ�������
//����:   x1 ����ʼ��ַ
//        y1 ����ʼ��ַ
//        x2 �н�����ַ
//        y2 �н�����ַ
//        R,G,B ����ɫ
//        ��ɫ������Χ0-63
------------------------------*/
void OLED_Draw_Line(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 R,uint8 G,uint8 B)
{
  OLED_WR_Byte(0x21,0);     //DRAW LINE COMMAND
  OLED_WR_Byte(x1,0);       //column start address
  OLED_WR_Byte(y1,0);       //row start address
  OLED_WR_Byte(x2,0);       //column end address
  OLED_WR_Byte(y2,0);       //row end address
  OLED_WR_Byte(R,0);        //color C
  OLED_WR_Byte(G,0);        //color B
  OLED_WR_Byte(B,0);        //color A
}


/*-------------------------------
//�ڲ����ɻ���������
//����:   x1 ����ʼ��ַ
//        y1 ����ʼ��ַ
//        x2 �н�����ַ
//        y2 �н�����ַ
//        R1,G1,B1 ����ɫ
//        E2,G2,B2 ���ɫ
------------------------------*/
void OLED_Draw_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 R1,uint8 G1,uint8 B1,uint8 R2,uint8 G2,uint8 B2)
{
  OLED_WR_Byte(0x22,0);     //DRAW RECTANGLE COMMAND
  OLED_WR_Byte(x1,0);       //column start address 
  OLED_WR_Byte(y1,0);       //row start address
  OLED_WR_Byte(x2,0);       //column end address
  OLED_WR_Byte(y2,0);       //row end address
  OLED_WR_Byte(R1,0);       //line color
  OLED_WR_Byte(G1,0);
  OLED_WR_Byte(B1,0);
  OLED_WR_Byte(R2,0);       //fill color
  OLED_WR_Byte(G2,0);
  OLED_WR_Byte(B2,0);
}

/*-------------------------
//�����������
//����:1��ʾʹ�ܾ����ڲ����,0��ʾ���þ����ڲ����,��Ҫ�ڻ�����֮ǰ����,����֮��Ļ��ƾ���������Ч
------------------------*/
void OLED_Fill_Rectangle(uint8 mode)
{
  if(mode) mode = 1;
  OLED_WR_Byte(0x26,0);
  OLED_WR_Byte(mode,0);
}


/*--------------------------------------------------------------------------------------
//��������,���Ѿ�д���������Ч,�����������ʾ���᾵��
//����:horizontal=1��ʾˮƽ������ʾ,vertical=1��ʾ��ֱ������ʾ,Ĭ������Ϊ�������Ҿ�������ʾ
-------------------------------------------------------------------------------------*/
void OLED_Mirror_Mode(uint8 horizontal,uint8 vertical)
{
  uint8 temp;
  if((horizontal == 0) && (vertical == 0)) temp = 96;
  else if((horizontal!=0) && (vertical == 0)) temp = 98;
  else if((horizontal==0) && (vertical != 0)) temp = 112;
  else temp = 114;
  OLED_WR_Byte(0xa0,0);
  OLED_WR_Byte(temp,0);//
}

/*---------------------------------------------------------------------
//��ƫ��
//����:num
//������Χ:0-63
-------------------------------------------------------------------*/
void OLED_Display_Offset(uint8 num)
{
  if(num>63) num=63;
  OLED_WR_Byte(0xa2,0);
  OLED_WR_Byte(num,0);
}
/*---------------------------------------------------------------------
//��ʼ��ƫ��
//����:num
//������Χ:0-63
//����ƫ�Ƶ�����������г���,Ҳ���Բ鿴�����ֲ�˵��
-------------------------------------------------------------------*/
void OLED_Display_Startline(uint8 num)
{
  if(num>63) num=63;
  OLED_WR_Byte(0xa1,0);
  OLED_WR_Byte(num,0);
}
/*---------------------------------------------------------------------
//����ʹ��
//����:num:ʹ�ܵ�����
//������Χ:16-64
//��û����ƫ�Ƶ������,���·���δʹ�ܵ�����64-num��ȫ��
//ע��:�˴������·��Ǿ��Է���,����OLED������Ϊ���Է�������Ϸ�
//���ʹ����Ĭ�ϵľ���
-------------------------------------------------------------------*/
void OLED_Mux_Ratio(uint8 num)
{
  if(num>64) num=64;
  else if(num<16) num = 16; 
  OLED_WR_Byte(0xa8,0); //set Multiplex Ratio
  OLED_WR_Byte(num,0);
}

/*---------------------------------------------------------------------
//��ʼ��ƫ��
//����:num
//������Χ:0-63
//����ƫ�Ƶ�����������г���,Ҳ���Բ鿴�����ֲ�˵��
-------------------------------------------------------------------*/
void OLED_Copy_Area(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x,uint8 y)
{
  OLED_WR_Byte(0x23,0);     //Copy
  OLED_WR_Byte(x1,0);       //column start address
  OLED_WR_Byte(y1,0);       //row start address
  OLED_WR_Byte(x2,0);       //column end address
  OLED_WR_Byte(y2,0);       //row end address
  OLED_WR_Byte(x,0);        //paste column
  OLED_WR_Byte(y,0);        //paste row
}

/*---------------------------------------------------------------------
//����һ����
//RGB������ʽ
//5λR,6λG,5λB
-------------------------------------------------------------------*/
void OLED_Dot_RGB(uint8 x,uint8 y,uint8 R,uint8 G,uint8 B)
{
  OLED_WR_Byte(0x15,0);
  OLED_WR_Byte(x,0);
  OLED_WR_Byte(x+1,0);
  OLED_WR_Byte(0x75,0);
  OLED_WR_Byte(y,0);
  OLED_WR_Byte(y+1,0);
  OLED_WR_Byte(((B<<3)|(G>>3)),1);
  OLED_WR_Byte(((G<<5)|R),1);
}
/*---------------------------------------------------------------------
//����һ����
//16λ����ʽ
//5λR,6λG,5λB
-------------------------------------------------------------------*/
void OLED_Dot(uint8 x,uint8 y,uint16 color)
{
  uint8 VH,VL;
  VH=color>>8;
  VL=color;
  OLED_WR_Byte(0x15,0);
  OLED_WR_Byte(x,0);
  OLED_WR_Byte(x+1,0);
  OLED_WR_Byte(0x75,0);
  OLED_WR_Byte(y,0);
  OLED_WR_Byte(y+1,0);
  OLED_WR_Byte(VH,1);
  OLED_WR_Byte(VL,1);
}

/*---------------------------------------------------------------------
//ͼ�����
//
//col��ˮƽ��������ֵ0-95
//srow��ʼ������
//numrow��������
//srow + numrow <=64
//row�д�ֱ��������ֵ0-63
//row��������Mux RatioΪ64ʱ��Ч
//period��������,��λΪframe,0-3
//period˵��:0->6frames;1->10frames;2->100frames;3->200frames
-------------------------------------------------------------------*/
void OLED_Scrolling(uint8 col,uint8 srow,uint8 numrow,uint8 row,uint8 period)
{
  OLED_WR_Byte(0x27,0); // Continuous horizontal scroll
  OLED_WR_Byte(col,0); // Horizontal scroll by 1 column
  OLED_WR_Byte(srow,0); // Define row 40 as start row address
  OLED_WR_Byte(numrow,0); // Scrolling 24 rows
  OLED_WR_Byte(row,0); // No vertical scroll
  OLED_WR_Byte(period,0); // Set time interval between each scroll step as 6 frames
}

/*---------------------------------------------------------------------
//ͼ���������
//cfg=1����ʹ��,cfg=0��������
//�������ú���Ҫ����д����Ļ����
-------------------------------------------------------------------*/
void OLED_Scrolling_Config(uint8 cfg)
{
  if(cfg)  OLED_WR_Byte(0x2F,0);
  else OLED_WR_Byte(0x2E,0);
}


#elif (OLED_CATAGORY==3)

#elif (OLED_CATAGORY == 4)
/**********************************************
// IIC Write byte
// ��SSD1306д��һ���ֽڡ�
// dat:Ҫд�������/����
// cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
**********************************************/
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
  unsigned char data_temp = dat;
  unsigned char i;
  if(cmd!=0) cmd = 1;//��0��1,��ͬBool
  OLED_DC(cmd);
  OLED_D0(0);
  for(i=0;i<8;i++)
  {
    if(data_temp & 0x80){ OLED_D1(1);}
    else{	OLED_D1(0);}
    OLED_D0(1);
    OLED_D0(0);
    data_temp <<= 1;
  }
  OLED_DC(1);//�����������־��λ����,��ֹ����״̬������������
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʾ��������
//  @param      x			x����������(0<=x<=127)��x>127,��x=x%128;
//  @param      y			y����������(0<=y<=63)��x>63,��x=x%64;
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Set_Pos(uint8 x, uint8 y)
{
  uint8 page = y/8;
  page %= 8;
  OLED_WR_Byte(0xb0+page,0);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
  OLED_WR_Byte((x&0x0f)|0x01,0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��������
//  @param      fill_data	�����ɫ(0x00 or 0xff)
//                              00Ϊȫ��,FFΪȫ��
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(unsigned char fill_data)
{
  unsigned char i,n;
  for(i=0;i<8;i++)
  {
    OLED_WR_Byte ((0xb0+i),0);    //����ҳ��ַ(0~7)
    OLED_WR_Byte (0x00,0);      //������ʾλ�á��е͵�ַ
    OLED_WR_Byte (0x10,0);      //������ʾλ�á��иߵ�ַ
    for(n=0;n<OLED_X_WIDTH;n++) OLED_WR_Byte(fill_data,1);
  }

}
//ҳ�������,ֻ����Դ�����
//��Ҫ������ͬ�ľֲ�ˢ�º�����ˢ��!
void OLED_Clear_Page(uint8 x,uint8 y,unsigned char fill_data)
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  uint8 page;   //y�������ڵ�page
  page = y/8;
  OLED_Set_Pos(x,y);
  OLED_WR_Byte(0xb0+(y>>3),0);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
  OLED_WR_Byte((x&0x0f)|0x00,0);
  OLED_WR_Byte(fill_data,1);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʼ������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Init()
{
//  Initial GPIO PIN
  GPIO_Init(OLED_D0_PIN,GPO,0);
  GPIO_Init(OLED_D1_PIN,GPO,0);
  GPIO_Init(OLED_RST_PIN,GPO,1);
  GPIO_Init(OLED_DC_PIN,GPO,0);
//  GPIO_Init(OLED_CS_PIN,GPO,0);
  
//Initialized State(Parameters as Default)
  OLED_RST(0);
  Soft_Delay_ms(100);
  OLED_RST(1);
  Soft_Delay_us(100);
//Initial_M096128x64_ssd1306
  OLED_WR_Byte(0xae,0);//display off
                         //Initial Settings Configuration
  OLED_WR_Byte(0xd5,0);//--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80,0);//--set divide ratio

  OLED_WR_Byte(0xa8,0);//--set multiplex ratio(1 to 64)������
  OLED_WR_Byte(0x3F,0);//--1/64 duty

  OLED_WR_Byte(0x00,0);//---set low column address
  OLED_WR_Byte(0x10,0);//---set high column address

  OLED_WR_Byte(0xd3,0);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00,0);//-not offset

  OLED_WR_Byte(0x40,0);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)

  OLED_WR_Byte(0x20,0);//Set Memory Addressing Mode
  OLED_WR_Byte(0x02,0);//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET,0);11,Invalid
  OLED_WR_Byte(0xb0,0);//Set Page Start Address for Page Addressing Mode,0-7

  OLED_WR_Byte(0xa1,0);//--set segment re-map 0 to 127--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����

  OLED_WR_Byte(0xc8,0);//Set COM Output Scan Direction--Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  //ע��a0c0��a1c8��������Ļ��ʾ������
  

  OLED_WR_Byte(0xda,0);//--set com pins hardware configuration
  OLED_WR_Byte(0x12,0);

  OLED_WR_Byte(0x81,0);//--set contrast control register
  OLED_WR_Byte(0x0f,0);// Set SEG Output Current Brightness0x66

  OLED_WR_Byte(0xd9,0);//--set pre-charge period
  OLED_WR_Byte(0xf1,0);//

  OLED_WR_Byte(0xdb,0);//--set vcomh
  OLED_WR_Byte(0x30,0);//Set VCOM Deselect Level

  OLED_WR_Byte(0xa4,0);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content  

  OLED_WR_Byte(0xa6,0);//--set normal display  0Ϊ��1Ϊ��//A7��ʾ0����1��

//Clear Screen
  OLED_WR_Byte(0x8d,0);//--set DC-DC enable
  OLED_WR_Byte(0x14,0);//

  OLED_WR_Byte(0xaf,0);//--turn on oled panel

  Soft_Delay_ms(100);
  OLED_Set_Pos(0,0);
  OLED_Clear(0x00);
}

//��/��OLED��ʾ
void OLED_Display_Config(uint8 cfg)
{
  if(cfg)
  {
    OLED_WR_Byte(0x8d,0);//--set DC-DC enable
    OLED_WR_Byte(0x14,0);//DCDC on

    OLED_WR_Byte(0xaf,0);//--turn on oled panel
    Soft_Delay_ms(100);
    OLED_Set_Pos(0,0);
  }
  else
  {
    OLED_WR_Byte(0x8d,0);//--set DC-DC enable
    OLED_WR_Byte(0x10,0);//DCDC off
    OLED_WR_Byte(0xae,0);//display off
  }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED ����/Ϩ�� һ����
//  @param      x	    x����������
//  @param      y           y����������
//  @param      cfg         1 ��� ,0 ���
//  @param      refresh     1 ����ˢ�¸�ҳ ,0 ��ˢ�¸�ҳ(�ȴ�����OLED_Refresh_Gram����ȫ��ˢ��)
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_DrawPoint(uint8 x,uint8 y,uint8 cfg)
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  uint8 data1;  //data1��ǰ�������
  uint8 page;   //y�������ڵ�page
  uint8 pos;    //y��������page�ڵ�λ��
  page = y/8;
  pos = y%8;
  data1 = 0x01<<pos;
  OLED_Set_Pos(x,y);
  OLED_WR_Byte(0xb0+(y>>3),0);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
  OLED_WR_Byte((x&0x0f)|0x00,0);
  if(cfg)OLED_WR_Byte(data1,1);
  else OLED_WR_Byte(~data1,1);
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
//refresh     1 ����ˢ�¸õ� ,0 ��ˢ�¸õ�(�ȴ�����OLED_Refresh_Gram����ȫ��ˢ��)
//ע���ú���ˢ��Ч�ʺܵͣ����еĲ���������������õģ����Ż������˷���������-��-
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot)
{  
  unsigned char x,y;
  for(x=x1;x<=x2;x++)
  {
    for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
  }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʾ�ַ���
//  @param      width	    ������
//  @param      hight	    ����߶�
//  @param      x	    x����������
//  @param      y           y����������
//  @param      mode        0,������ʾ;1,������ʾ
//  @param      refresh     1 ����ˢ�¸�ҳ ,0 ��ˢ�¸�ҳ(�ȴ�����OLED_Refresh_Gram����ȫ��ˢ��)
//  @param      ch[]        �ַ���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_Show_String(uint8 width,uint8 height,uint8 x,uint8 y,uint8 mode,char ch[])
{
  #if (OLED_DISPLAY_MODE)
    y %= 64;
    x %= 128;
  #else
    if(x>127||y>63)return;//������Χ��.
  #endif
  //�����ֿ�淶�������Ⱥ͸߶�ֵ
  if(width >= 8) width = 8;
  else if(width <= 6) width = 6;
  else width = 8;
  
  if(height >= 16) height = 16;
  else if(height <= 8) height = 8;
  else if((height <= 12) && (height > 8)) height = 12;
  else height = 16;
  
  uint8 c=0,i=0,j=0,temp=0,y0=y,l=0,pos=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;//�ӿ���ʾ�ַ�" "��ʼ�������Ҫ32��ƫ����
    if(x>(128-width)){x=0;y+=height;y0+=height;}
    uint8 ascii_length;//��ģ����
    ascii_length = (height-1)/8+1;
    ascii_length *= width;
    for(i=0;i<ascii_length;i++)
    {
      if((width==6) && (height==8)) temp = F6x8[c][i];//ע��6x8�ֿ�Ϊ����ȡģ!
      else if((width==8) && (height==16)) temp = asc2_1608[c][i];
      else if((width==6) && (height==12)) temp = asc2_1206[c][i];
      for(l=0;l<8;l++)
      {
//        ����ķ�����Ȼ��ݣ���ǿ�ƽ�y�̶�����height�ĸ���ڣ���Ч��yֻ��Ϊ����ֵ
        OLED_Set_Pos(x,y);
        for(i=0;i<width;i++){    OLED_WR_Byte(ASCII_8x16[c][i],1);}
        OLED_Set_Pos(x,y+8);
        for(i=0;i<width;i++){    OLED_WR_Byte(ASCII_8x16[c][i+8],1);}
      }
    }
    j++;

  }
}







#endif
