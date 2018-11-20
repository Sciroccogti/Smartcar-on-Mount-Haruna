/*
  ******************************************************************************
  * @file    OLED_0_96.h
  * @author  xhJiang
  * @version v1.2
  * @date    2018.6.5
  * @mail:   blog_xhjiang@163.com
  * @brief   0.96����Ļ configure
  * @note    ͨ������
  ******************************************************************************
  */
#ifndef	_OLED_0_96_H
#define	_OLED_0_96_H

#include "common.h"
#include "font.h"

//������ʽ:IOģ��
/*************Pin Define***************/
#define OLED_D0_PIN     E2//ʱ��SCL
#define OLED_D1_PIN     E3//����SDA
#define OLED_RST_PIN    I2//��λ
#define OLED_DC_PIN     I3//����data\����command�л�
//#define OLED_CS_PIN     GND//Ƭѡ

#define OLED_D0(x)      GPIO_Set(OLED_D0_PIN,x);
#define OLED_D1(x)      GPIO_Set(OLED_D1_PIN,x);
#define OLED_RST(x)     GPIO_Set(OLED_RST_PIN,x);
#define OLED_DC(x)      GPIO_Set(OLED_DC_PIN,x);

//��/�ر�OLED��ʾ
#define OLED_DISPLAY_ON    1
#define OLED_DISPLAY_OFF   0

//���峬����Χ�Ƿ�����ѭ����ʾ(������x=200ʱ������x-128���ֵ��ʾ)
//1Ϊѭ����ʾ��0Ϊ����ʾ
#define OLED_DISPLAY_MODE   1

//����OLED�Ƿ���������
#define OLED_REFRESH_YES   1
#define OLED_REFRESH_NO    0
//---------------------------------------------------------------------
//����ʹ�õ�OLED���
//1��ʾʹ��SSD1306������0.96�絥ɫ/˫ɫOLED��
//2��ʾʹ��SSD1331������0.95��RGBȫ��OLED��
//3��ʾʹ��ST7735������0.96��TFT��
//4��ʾʹ��SSD1306������0.96�絥ɫ/˫ɫOLED���������Դ棨������RAM�����õĵͶ˵�Ƭ����
#define OLED_CATAGORY 1
//-----------------------------------------------------------------------
#if (OLED_CATAGORY == 1)


#define OLED_X_WIDTH            128
#define OLED_Y_WIDTH            64

#define OLED_XLevelL		0x00
#define OLED_XLevelH		0x10
#define OLED_XLevel		((XLevelH&0x0F)*16+XLevelL)
#define OLED_Max_Column	        128
#define OLED_Max_Row		64




void OLED_Init(void);
void OLED_Clear(unsigned char fill_data);
void OLED_Clear_Page(uint8 x,uint8 y,unsigned char fill_data);
void OLED_Picture(uint16 high, uint16 width, uint8 *p);
void OLED_Refresh_Gram(void);
void OLED_Refresh_Gram_Page(uint8 x,uint8 y);
void OLED_Reverse(uint8 width,uint8 height,uint8 x,uint8 y,uint8 refresh);
//void OLED_Showimage(const unsigned char *p);
void OLED_Display_Config(uint8 cfg);
void OLED_DrawPoint(uint8 x,uint8 y,uint8 cfg,uint8 refresh);
void OLED_Show_String(uint8 width,uint8 height,uint8 x,uint8 y,uint8 mode,char ch[],uint8 refresh);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot,uint8 refresh);
//void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
//void OLED_Print_Num(uint8 width,uint8 height,uint8 x, uint8 y,uint8 mode, int16 num, uint8 refresh);

#elif (OLED_CATAGORY == 2)


#define OLED_X_WIDTH            96
#define OLED_Y_WIDTH            64

#define OLED_Max_Column	        96
#define OLED_Max_Row		64



void OLED_Init();
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Display_Dim(void);
void OLED_Brightness_Set(uint8 duty);
void OLED_Contrast_Set(uint8 red,uint8 green,uint8 blue);
void OLED_Lock_IC(void);
void OLED_Unlock_IC(void);
void OLED_Clear_Window(uint8 x1,uint8 y1,uint8 x2,uint8 y2);
void OLED_Draw_Line(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 R,uint8 G,uint8 B);
void OLED_Draw_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 R1,uint8 G1,uint8 B1,uint8 R2,uint8 G2,uint8 B2);
void OLED_Fill_Rectangle(uint8 mode);
void OLED_Mirror_Horizontal_Mode(uint8 cfg);
void OLED_Display_Offset(uint8 num);
void OLED_Display_Startline(uint8 num);
void OLED_Mux_Ratio(uint8 num);
void OLED_Copy_Area(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x,uint8 y);
void OLED_Dot_RGB(uint8 x,uint8 y,uint8 R,uint8 G,uint8 B);
void OLED_Dot(uint8 x,uint8 y,uint16 color);
void OLED_Scrolling(uint8 col,uint8 srow,uint8 numrow,uint8 row,uint8 period);
void OLED_Scrolling_Config(uint8 cfg);

#elif (OLED_CATAGORY == 3)
//���ڵ�ǰOLED��Ļ�ӿڵ�CS��ֱ�ӽӵ�,��TFT��Ļ����ͬ���Ŷ�Ӧ���Ǳ�������,��˸���Ļ���޷�ʹ��
#elif (OLED_CATAGORY == 4)
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);
void OLED_Set_Pos(uint8 x, uint8 y);
void OLED_Clear(unsigned char fill_data);
void OLED_Clear_Page(uint8 x,uint8 y,unsigned char fill_data);
void OLED_Init();
void OLED_Display_Config(uint8 cfg);
void OLED_DrawPoint(uint8 x,uint8 y,uint8 cfg);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
void OLED_Show_String(uint8 width,uint8 height,uint8 x,uint8 y,uint8 mode,char ch[])


#endif

#endif
