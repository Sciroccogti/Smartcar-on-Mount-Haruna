/*
  ******************************************************************************
  * @file    TFT_1_8.h
  * @author  xhJiang
  * @version v1.2
  * @date    2018.5.25
  * @mail:   blog_xhjiang@163.com
  * @brief   1.8寸TFT屏幕
  * @note    通过测试，功能尚待完善
  *          受限于KEA的RAM大小,显存无法使用
  ******************************************************************************
  */
#ifndef _TFT_1_8_H
#define _TFT_1_8_H

#include "common.h"


//定义TFT通信使用硬件SPI或模拟SPI
#define TFT_HARD_MODE   1
#define TFT_SOFT_MODE   0

#define TFT_SPI_MODE    0

//定义TFT的尺寸
#define	TFT_X_MAX	128
#define TFT_Y_MAX	160

#define TFT_SCK_PIN     B2//时钟
#define TFT_SDA_PIN     B3//数据
#define TFT_A0_PIN      F5//数据\命令切换
#define TFT_CS_PIN      F7//片选
#define TFT_RST_PIN     H4//复位

#define TFT_SCK(x)   	GPIO_Set(TFT_SCK_PIN,x);
#define TFT_SDA(x)   	GPIO_Set(TFT_SDA_PIN,x);
#define TFT_A0(x)   	GPIO_Set(TFT_A0_PIN,x);
#define TFT_CS(x)   	GPIO_Set(TFT_CS_PIN,x);
#define TFT_RST(x)   	GPIO_Set(TFT_RST_PIN,x);



extern  uint16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

//-------画笔颜色----------
#define RED     	0XF800    //红色
#define GREEN   	0X07E0    //绿色
#define BLUE    	0X001F    //蓝色
#define BRED    	0XF81F
#define GRED    	0XFFE0    //
#define GBLUE   	0X07FF    //
#define BLACK   	0X0000    //黑色
#define WHITE   	0XFFFF    //白色
#define YELLOW  	0xFFE0    //黄色
#define GRAY0           0xEF7D   
#define GRAY1           0x8410      	
#define GRAY2           0x4208
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 	         0XC618 //浅灰色
#define LGRAYBLUE        0XA651 //浅灰蓝色
#define LBBLUE           0X2B12 //浅棕蓝色

//定义超出范围是否正常循环显示(如坐标x=200时，按照x-128后的值显示)
//1为循环显示，0为不显示
#define LCD_DISPLAY_MODE   1

void LCD_Init(void); 

void LCD_DisplayOff(void);
void LCD_DisplayOn(void);
void LCD_Sleep(void);


void LCD_Clear(uint16 color);
void LCD_SetRegion(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
//void LCD_Refresh_Gram(void);
//void LCD_ClearGram(uint16 color);
void LCD_DrawPoint(uint16 x,uint16 y,uint16 color);
void LCD_Fill(uint16 xsta,uint16 ysta,uint16 xend,uint16 yend,uint16 color);

void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2 ,uint16 color);
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2,uint16 color);		   
void Draw_Circle(uint16 x0,uint16 y0,uint8 r,uint16 color);

void LCD_ShowChar1608(uint16 x,uint16 y,uint8 num,uint8 mode,uint16 pencolor,uint16 backcolor);
void LCD_Show_String(uint8 width,uint8 height,uint8 x,uint8 y,char ch[],uint8 mode,uint16 pencolor,uint16 backcolor);
void LCD_Fill_CharImage(uint8 sx,uint8 sy,uint8 width,uint8 height,uint8 ch[],uint16 pencolor,uint16 backcolor);
void LCD_Fill_Image(uint8 sx,uint8 sy,uint8 width,uint8 height,uint8 ch[]);
void showhanzi(unsigned int x,unsigned int y,unsigned char index);




#endif
