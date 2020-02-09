//****************************************************** 
//函数：OLED_GPIO_Initial();			GPIO设置   
//			OLED_Reset_Initial();		OLED配置 
//			OLED_Refresh_Gram();		GRAM写入OLED
//目的：OLED配置
//说明：
//Maker：huangwankuan
//2017.2.6
//Version：V1.1
//All rights reserved
//******************************************************
#ifndef _OLED_h
#define _OLED_h

#include "sys.h"
#include "delay.h"   
#define OLED_CMD 1         //CMD 选择命令
#define OLED_DATA 0        //数据命令

#define ASCII_Mode 1
#define String_Mode 0

#define CMD_OLED PBout(14)  //SPI2数据/命令信号

#define RES_OLED PHout(7)  	//OLED复位信号

#define NSS_SPI  PBout(12)  //SPI2片选信号
  
void SPI1_Init(void);

u8 SPI1_ReadWriteByte(u8 TxData);

void OLED_Write_Data(u8 Write_Data_OLED,u8 decide);	
	 
void OLED_Display_On(void);   //开启OLED显示		
void OLED_Display_Off(void);  //关闭OLED显示
void OLED_Reset_Initial(void);//OLED复位函数（包括OLED的各项寄存器设置）
void OLED_Refresh_Gram(void); //将GRAM中的缓存写入至OLED中
void OLED_Clean(void);//清屏   数据不写如GRAM缓存  直接写入OLED
void OLED_Clean_GRAM(void);
void OLED_ALL_ON(void);//全亮   数据不写如GRAM缓存  直接写入OLED
void OLED_BMP(void);
void OLED_BMP_3(void);
void OLED_Test(void);
void OLED_XY(u8 X_OLED,u8 Y_OLED);//移位函数|__  
//GRAM的值不改变 对硬件OLED进行操作   如果（1，1）则表示向右移动一位  向上移动一位
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_OUT_String(u8 x,u8 y,u8* p,u8 size);
u32 Math_th(u8 m,u8 n);
void OLED_OUT_Num(u8 x,u8 y,u32 num,u8 size);
#endif
