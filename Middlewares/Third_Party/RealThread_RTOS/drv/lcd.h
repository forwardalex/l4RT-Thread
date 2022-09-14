#ifndef __LCD_H
#define __LCD_H
#include "main.h"

extern u16	POINT_COLOR;		//Ĭ�ϻ�����ɫ
extern u16	BACK_COLOR;		//Ĭ�ϱ�����ɫ

//LCD
#define LCD_Width 	240
#define LCD_Height 	240

//color
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40
#define BRRED 			 0XFC07
#define GRAY  			 0X8430
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//
#define LIGHTBLUE      	 0X7D7C	//
#define GRAYBLUE       	 0X5458 //

 
#define LIGHTGREEN     	 0X841F
//#define LIGHTGRAY        0XEF5B
#define LGRAY 			 0XC618

#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12



/*
	LCD_PWR:	PB7
	LCD_RST:	PB6
	LCD_DC:		PB4
	LCD_CS:		PD7
*/
#define	LCD_PWR(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET))
#define	LCD_RST(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET))
#define	LCD_DC(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET))
#define	LCD_CS(n)		(n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET))


void LCD_Init(void);																	//��ʼ��
void LCD_DisplayOn(void);																//����ʾ
void LCD_DisplayOff(void);																//����ʾ
void LCD_Write_HalfWord(const u16 da);													//д����ֽ����ݵ�LCD
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);									//����������ʾ����
void LCD_Clear(u16 color);																//����
void LCD_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color);				//��䵥ɫ
void LCD_Draw_Point(u16 x, u16 y);														//����
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);										//������ɫ��
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);										//����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);									//������
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);												//��Բ
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size);										//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);									//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);							//��ʾ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p);					//��ʾ�ַ���
void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *p);					//��ʾͼƬ
void Display_ALIENTEK_LOGO(u16 x,u16 y);												//��ʾALIENTEK LOGO

#endif


