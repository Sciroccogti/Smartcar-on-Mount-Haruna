/*
  ******************************************************************************
  * @file    TFT_1_8.c
  * @author  xhJiang
  * @version v1.2
  * @date    2018.5.25
  * @mail:   blog_xhjiang@163.com
  * @brief   1.8��TFT��Ļ configure
  * @note    ͨ������
  ******************************************************************************
  */

#include "TFT_1_8.h"
#include "delay.h"

//����SPI�ӻ�����
uint8 spi_slave_num = 1;

uint16 BACK_COLOR=WHITE, POINT_COLOR=BLACK;   //����ɫ������ɫ��Ĭ�ϰ׵׺���

//KEA��RAM����,�޷�������˴������,����޷�ʹ��
uint16 TFT_GRAM[TFT_X_MAX][TFT_Y_MAX];//�����Դ�



void LCD_Write_Bus(uint8 data)//��������д��
{
  uint8 lcd_write_data = data;
  uint8 i = 8;
  while(i--)
  {
    if(lcd_write_data & (0x80)){ TFT_SDA(1);}//���λΪ1ʱ��SDA���1
    else{ TFT_SDA(0);}//���λΪ0ʱ��SDA���0
    TFT_SCK(1);
    TFT_SCK(0);
    //��TFT_SCK(0)��TFT_SCK(1)���
    //���������ݲ������½������ݷ���
    lcd_write_data <<= 1;
  }
}
void LCD_Write_Data8(uint8 da)//����8λ����
{
  if(spi_slave_num > 1)
  {
    TFT_CS(0);
    TFT_A0(1);
    TFT_SCK(0);//��֤SCLK�������ڷ�������ǰ���ڿ���״̬
    LCD_Write_Bus(da);
    TFT_CS(1);
  }
  else
  {
    TFT_A0(1);
    LCD_Write_Bus(da);
  }
}
 void LCD_Write_Data16(uint16 da)
{
  LCD_Write_Data8(da>>8);
  LCD_Write_Data8(da);
}
void LCD_Write_Index(uint8 da)
{
  if(spi_slave_num > 1)
  {
    TFT_CS(0);
    TFT_A0(0);
    TFT_SCK(0);//��֤SCLK�������ڷ�������ǰ���ڿ���״̬
    LCD_Write_Bus(da);
    TFT_CS(1);
  }
  else
  {
    TFT_A0(0);
    LCD_Write_Bus(da);
  }
}
void LCD_Write_Index_Data(uint8 reg,uint16 da)
{
  LCD_Write_Index(reg);
  LCD_Write_Data16(da);
}

void LCD_Memory_Write()
{
  LCD_Write_Index(0x2C);//Start Memory Write
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����������
//  @param      x1     	        ����x��������
//  @param      y1     	        ����y��������
//  @param      x2       	����x������յ�
//  @param      y2       	����y������յ�
//  @return     void
//  @since      v1.0
//  Sample usage:               LCD_SetRegion(0,0,10,10);// x��y����㶼��0���յ㶼��10
//-------------------------------------------------------------------------------------------------------------------
void LCD_SetRegion(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
  LCD_Write_Index(0x2a);//Column Address Set
  LCD_Write_Data8(x1>>8);
  LCD_Write_Data8(x1);
  LCD_Write_Data8(x2>>8);
  LCD_Write_Data8(x2);

  LCD_Write_Index(0x2b);//Row Address Set
  LCD_Write_Data8(y1>>8);
  LCD_Write_Data8(y1);
  LCD_Write_Data8(y2>>8);
  LCD_Write_Data8(y2);
}

void LCD_SetXY(unsigned int x,unsigned int y)
{
  LCD_SetRegion(x,y,x,y);
}
//���ù����ʼ�㣬�յ�Ϊx=128,y=160(������Ҫ����һ����ʼ��)
void LCD_SetCursor(unsigned int x,unsigned int y)
{
  LCD_Write_Index(0x2a);//Column Address Set
  LCD_Write_Data8(x>>8);
  LCD_Write_Data8(x);

  LCD_Write_Index(0x2b);//Row Address Set
  LCD_Write_Data8(y>>8);
  LCD_Write_Data8(y);
}
//Һ������ʼ��
void LCD_Init()
{
  GPIO_Init(TFT_SCK_PIN,GPO,0);
  GPIO_Init(TFT_SDA_PIN,GPO,0);
  GPIO_Init(TFT_A0_PIN,GPO,0);
  GPIO_Init(TFT_CS_PIN,GPO,0);
  GPIO_Init(TFT_RST_PIN,GPO,0);
//����һ����Щ��������ñ����ʱ����ʾ����
  TFT_CS(1);
//  if(GPIO_Get(TFT_CS_PIN)==0)
//  {
//     LCD_Write_Index_Data(0,0);
//     LCD_ShowString(0,0," ");
//     LCD_ShowNum(0,0,0,0);
//     LCD_Show2Num(0,0,0,0);
//     LCD_DrawRectangle(0,0,0,0);
//     Draw_Circle(0,0,0);
//   }
  TFT_RST(0);
  Soft_Delay_ms(20);
  TFT_RST(1);
  Soft_Delay_ms(20);
  TFT_CS(0);  //�䨰?a????��1?��
  
//  LCD_Write_Index(0x01); //Software reset
//  Soft_Delay_ms(120); //Delay 120ms
  LCD_Write_Index(0x11); //Sleep out
  Soft_Delay_ms(120); //Delay 120ms
  //------------------------------------ST7735S Frame Rate-----------------------------------------//
  LCD_Write_Index(0xB1);
  LCD_Write_Data8(0x05);
  LCD_Write_Data8(0x3C);
  LCD_Write_Data8(0x3C);
  LCD_Write_Index(0xB2);
  LCD_Write_Data8(0x05);
  LCD_Write_Data8(0x3C);
  LCD_Write_Data8(0x3C);
  LCD_Write_Index(0xB3);
  LCD_Write_Data8(0x05);
  LCD_Write_Data8(0x3C);
  LCD_Write_Data8(0x3C);
  LCD_Write_Data8(0x05);
  LCD_Write_Data8(0x3C);
  LCD_Write_Data8(0x3C);
  //------------------------------------End ST7735S Frame Rate-----------------------------------------//
  LCD_Write_Index(0xB4); //Dot inversion
  LCD_Write_Data8(0x03);
  //-----------------------------------Start ST7735S Power Control-----------------------------------//
  LCD_Write_Index(0xC0);
  LCD_Write_Data8(0x28);
  LCD_Write_Data8(0x08);
  LCD_Write_Data8(0x04);
  LCD_Write_Index(0xC1);
  LCD_Write_Data8(0XC0);
  LCD_Write_Index(0xC2);
  LCD_Write_Data8(0x0D);
  LCD_Write_Data8(0x00);
  LCD_Write_Index(0xC3);
  LCD_Write_Data8(0x8D);
  LCD_Write_Data8(0x2A);
  LCD_Write_Index(0xC4);
  LCD_Write_Data8(0x8D);
  LCD_Write_Data8(0xEE);
  //---------------------------------End ST7735S Power Sequence-------------------------------------//
  LCD_Write_Index(0xC5); //VCOM
  LCD_Write_Data8(0x1A);
  LCD_Write_Index(0x36); //MX, MY, RGB mode
  LCD_Write_Data8(0xC0);
  //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
  LCD_Write_Index(0xE0);
  LCD_Write_Data8(0x04);
  LCD_Write_Data8(0x22);
  LCD_Write_Data8(0x07);
  LCD_Write_Data8(0x0A);
  LCD_Write_Data8(0x2E);
  LCD_Write_Data8(0x30);
  LCD_Write_Data8(0x25);
  LCD_Write_Data8(0x2A);
  LCD_Write_Data8(0x28);
  LCD_Write_Data8(0x26);
  LCD_Write_Data8(0x2E);
  LCD_Write_Data8(0x3A);
  LCD_Write_Data8(0x00);
  LCD_Write_Data8(0x01);
  LCD_Write_Data8(0x03);
  LCD_Write_Data8(0x13);
  LCD_Write_Index(0xE1);
  LCD_Write_Data8(0x04);
  LCD_Write_Data8(0x16);
  LCD_Write_Data8(0x06);
  LCD_Write_Data8(0x0D);
  LCD_Write_Data8(0x2D);
  LCD_Write_Data8(0x26);
  LCD_Write_Data8(0x23);
  LCD_Write_Data8(0x27);
  LCD_Write_Data8(0x27);
  LCD_Write_Data8(0x25);
  LCD_Write_Data8(0x2D);
  LCD_Write_Data8(0x3B);
  LCD_Write_Data8(0x00);
  LCD_Write_Data8(0x01);
  LCD_Write_Data8(0x04);
  LCD_Write_Data8(0x13);
  //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
  LCD_Write_Index(0x3A); //65k mode
  LCD_Write_Data8(0x05);
  LCD_Write_Index(0x29); //Display on
}
//��LCD����˯��ģʽ(�͹���ģʽ)
void LCD_Sleep(void)
{
  LCD_Write_Data8(0x10);
  Soft_Delay_ms(120);
}

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
  LCD_Write_Index(0x29);
}	 
//LCD�ر���ʾ
//LCD�ر���ʾ��ͬ���ĵ磬��ʱ��ر���ʾ�����ý���˯��ģʽ
void LCD_DisplayOff(void)
{
  LCD_Write_Index(0x28);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����������
//  @param      color           ��ɫ����
//  @return     void
//  @since      v1.0
//  Sample usage:               LCD_Clear(YELLOW);// ȫ������Ϊ��ɫ
//-------------------------------------------------------------------------------------------------------------------
void LCD_Clear(uint16 color)
{
  uint8 VH,VL;
  uint16 i,j;
  BACK_COLOR = color;
  VH=color>>8;
  VL=color;
  LCD_SetRegion(0,0,TFT_X_MAX-1,TFT_Y_MAX-1);
  LCD_Memory_Write();
  for(i=0;i<TFT_X_MAX;i++)
  {
    for (j=0;j<TFT_Y_MAX;j++)
    {
      LCD_Write_Data8(VH);
      LCD_Write_Data8(VL);
    }
  }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ���Դ��������
//  @param      color     		��ɫ����
//  @return     void
//  @since      v1.0
//  @note       �Դ治����,�ú����޷�ʹ��
//  Sample usage:               LCD_ClearGram(YELLOW);// ȫ������Ϊ��ɫ,��Ҫ����LCD_Refresh_Gram������ʾ
//-------------------------------------------------------------------------------------------------------------------
void LCD_ClearGram(uint16 color)
{
  unsigned char i,n;
  for(i=0;i<TFT_Y_MAX;i++)
  {
    for(n=0;n<TFT_X_MAX;n++)
    {
      TFT_GRAM[n][i]=color;
    }
  }
}
//  @note       �Դ治����,�ú����޷�ʹ��
void LCD_Refresh_Gram(void)
{
  unsigned char i,j;
  LCD_SetRegion(0,0,TFT_X_MAX-1,TFT_Y_MAX-1);
  LCD_Memory_Write();
  for(i=0;i<TFT_X_MAX;i++)
  {
    for (j=0;j<TFT_Y_MAX;j++)
    {
      LCD_Write_Data16(TFT_GRAM[i][j]);
    }

  }
}
//----------------------------------------------------------------------------------
//  @brief      ��һ����
//  @param
//  @return     void
//  @since      v1.0
//-----------------------------------------------------------
void LCD_DrawPoint(uint16 x,uint16 y,uint16 color)
{
  POINT_COLOR = color;
  LCD_SetXY(x,y);
  LCD_Memory_Write();
  LCD_Write_Data16(color);
}
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Fill(uint16 sx,uint16 sy,uint16 ex,uint16 ey,uint16 color)
{
  uint16 i,j;
  LCD_SetRegion(sx,sy,ex,ey);
  LCD_Memory_Write();
  for(i=sy;i<=ey;i++)
  {
    for(j=sx;j<=ex;j++)LCD_Write_Data16(color);
  }
////  �ȼ�������д��
//  uint16 xlen=0;
//  xlen=ex-sx+1;	   
//  for(i=sy;i<=ey;i++)
//  {									   
//    LCD_SetCursor(sx,i);      				//���ù��λ�� 
//    LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
//    for(j=0;j<xlen;j++)LCD_Write_Data16(color);	//���ù��λ�� 	    
//  }
}
//����
//x1,y1:�������
//x2,y2:�յ����� 
void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2,uint16 color)
{
  uint16 t;
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 

  delta_x=x2-x1;//������������
  delta_y=y2-y1; 
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1; //���õ������� 
  else if(delta_x==0)incx=0;//��ֱ�� 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else if(delta_y==0)incy=0;//ˮƽ�� 
  else{incy=-1;delta_y=-delta_y;} 
  if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )//������� 
  {  
    LCD_DrawPoint(uRow,uCol,color);//���� 
    xerr+=delta_x ; 
    yerr+=delta_y ; 
    if(xerr>distance) 
    { 
      xerr-=distance; 
      uRow+=incx; 
    } 
    if(yerr>distance) 
    { 
      yerr-=distance; 
      uCol+=incy; 
    } 
  }  
}    
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2,uint16 color)
{
  LCD_DrawLine(x1,y1,x2,y1,color);
  LCD_DrawLine(x1,y1,x1,y2,color);
  LCD_DrawLine(x1,y2,x2,y2,color);
  LCD_DrawLine(x2,y1,x2,y2,color);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(uint16 x0,uint16 y0,uint8 r,uint16 color)
{
  int a,b;
  int di;
  a=0;b=r;
  di=3-(r<<1);             //�ж��¸���λ�õı�־
  while(a<=b)
  {
    LCD_DrawPoint(x0+b,y0-a,color);             //0
    LCD_DrawPoint(x0-a,y0+b,color);             //1
    LCD_DrawPoint(x0-b,y0-a,color);             //7//3
    LCD_DrawPoint(x0-a,y0-b,color);             //2
    LCD_DrawPoint(x0+b,y0+a,color);             //4
    LCD_DrawPoint(x0+a,y0-b,color);             //5
    LCD_DrawPoint(x0+a,y0+b,color);             //6
    LCD_DrawPoint(x0-b,y0+a,color);
    a++;
    //ʹ��Bresenham�㷨��Բ
    if(di<0)di +=4*a+6;
    else
    {
      di+=10+4*(a-b);
      b--;
    }
    LCD_DrawPoint(x0+a,y0+b,color);
  }
}
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//���ӷ�ʽ��ʾ������ʾ��ԭ�еı�����
void LCD_ShowChar1608(uint16 x,uint16 y,uint8 num,uint8 mode,uint16 pencolor,uint16 backcolor)
{
  uint8 temp;
  uint8 pos,t;
  uint16 y0=y;
  POINT_COLOR = pencolor;
  BACK_COLOR = backcolor;
  if(x>TFT_X_MAX-8||y>TFT_Y_MAX-16)return;
  //���ô���
  num=num-' ';//�õ�ƫ�ƺ��ֵ
  for(pos=0;pos<16;pos++)
  {
    temp=asc2_1608[num][pos];//����1608����
    for(t=0;t<8;t++)
    {
      if(temp&0x80)LCD_DrawPoint(x,y,pencolor);
      else if(!mode) LCD_DrawPoint(x,y,backcolor);//�ǵ��ӷ�ʽ�滻������ɫ
      temp<<=1;
      y++;
      if((y-y0)==16)
      {
        y=y0;
        x++;
        break;
      }
    }
  }
}


uint32 mypow(uint8 m,uint8 n)
{
	uint32 result=1;
	while(n--)result*=m;
	return result;
}

//void LCD_ShowNum(uint16 x,uint16 y,uint32 num,uint8 len)
//{
//	uint8 t,temp;
//	uint8 enshow=0;
//	num=(uint16)num;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/mypow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				LCD_ShowChar(x+8*t,y,' ',0);
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
//	}
//}

void LCD_Show_String(uint8 width,uint8 height,uint8 x,uint8 y,char ch[],uint8 mode,uint16 pencolor,uint16 backcolor)
{
  #if (LCD_DISPLAY_MODE)
    y %= 160;
    x %= 128;
  #else
    if(x>127||y>159)return;//������Χ��.
  #endif
  //�����ֿ�淶�������Ⱥ͸߶�ֵ
  if(width >= 8) width = 8;
  else if(width <= 6) width = 6;
  else width = 8;
  
  if(height >= 16) height = 16;
  else if(height <= 8) height = 8;
  else if((height <= 12) && (height > 8)) height = 12;
  else height = 16;
  
  uint8 c=0,i=0,j=0,temp=0,y0=y,l=0;
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
          if(temp&0x01)LCD_DrawPoint(x,y,pencolor);//��λ��ǰ����ȡģ,���λ��Ӧ��˳��ȡģ�����λ
          else if(!mode) LCD_DrawPoint(x,y,backcolor);//�ǵ��ӷ�ʽ�滻������ɫ
          temp>>=1;
        }
        else
        {
          if(temp&0x80)LCD_DrawPoint(x,y,pencolor);//��λ��ǰ˳��ȡģ
          else if(!mode) LCD_DrawPoint(x,y,backcolor);//�ǵ��ӷ�ʽ�滻������ɫ
          temp<<=1;
        }
        y++;
	if((y-y0)==height)
        {
          y=y0;
          x++;
          break;//����ȡģ�Ĳ���λ
        }
      }
    }
    j++;
  }
}

////��ָ��λ����ʾһ������(16*16��С)
//void showhanzi16(unsigned int x,unsigned int y,unsigned char index)	
//{  
//	unsigned char i,j,k;
//	const unsigned char *temp=hanzi16;    
//	temp+=index*32;	
//	for(j=0;j<16;j++)
//	{
//		LCD_SetCursor(x,y+j);
//		LCD_WriteRAM_Prepare();	//��ʼд��GRAM
//		for(k=0;k<2;k++)
//		{
//			for(i=0;i<8;i++)
//			{ 		     
//			 	if((*temp&(1<<i))!=0)
//				{
//					LCD_WR_DATA(POINT_COLOR);
//				} 
//				else
//				{
//					LCD_WR_DATA(BACK_COLOR);
//				}   
//			}
//			temp++;
//		}
//	 }
//}

//��ʾPctolcd2002ȡ��ģ����ȡ�õ�ͼ����ģ
//OLED��������ֲ
void LCD_Fill_CharImage(uint8 sx,uint8 sy,uint8 width,uint8 height,uint8 ch[],uint16 pencolor,uint16 backcolor)
{
  uint8 temp,l,i;
  uint8 y0=sy;
  uint16 loop_count = 0;

  loop_count = (height-1)/8+1;
  loop_count *= width;
  for(i=0;i<loop_count;i++)
  {
    temp = ch[i];
    for(l=0;l<8;l++)
    {
      if(temp&0x80)LCD_DrawPoint(sx,sy,pencolor);//��λ��ǰ˳��ȡģ
      else LCD_DrawPoint(sx,sy,backcolor);//�滻������ɫ
      temp<<=1;
      sy++;
      if((sy-y0)==height)
      {
        sy=y0;
        sx++;
        break;//����ȡģ�Ĳ���λ
      }
    }
  }
}
//��ʾ16λ���ɫͼ��,����ImagetoLcdȡģ,��ֱɨ��,��λ��ǰ,���ͼ��16λ��ɫR5,G6,B5
void LCD_Fill_Image(uint8 sx,uint8 sy,uint8 width,uint8 height,uint8 ch[])
{
  uint16 temp = 0;
  uint32 i = 0,j = 0;
  uint32 length = 0;
  for(i=0;i<width;i++)
  {
    for(j=0;j<height;j++)
    {
      length = 2*(i*height+j);
      temp = ch[length]<<8;
      temp += ch[length+1];
      LCD_DrawPoint(sx+i,sy+j,temp);
    }
  }
}
