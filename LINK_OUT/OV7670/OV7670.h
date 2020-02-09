#ifndef __0V7670_H
#define __0V7670_H

#include "delay.h"
#include "sys.h"

#define OV7670_VSYNC  	PCin(7)			//ͬ���źż��IO


#define OV7670_WRST				PBout(5)		//дָ�븴λ
#define OV7670_WREN				PCout(6)		//д��FIFOʹ��
#define OV7670_RCK					PDout(3)		//������ʱ��
#define OV7670_RRST				PCout(4)  		//��ָ�븴λ
#define OV7670_CS						PCout(5)  		//Ƭѡ�ź�(OE)
															  					 
//GPIOC->IDR&0x00FF 
/////////////////////////////////////////

#define CHANGE_REG_NUM 							171			//��Ҫ���õļĴ�������		  
 				 
u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void EXTI2_Init(void);


#define SCCB_SDA_IN()  {GPIOB->MODER&=0XFF3FFFFF;GPIOB->MODER|=0X00000000;}
#define SCCB_SDA_OUT() {GPIOB->MODER&=0XFF3FFFFF;GPIOB->MODER|=0X00400000;}

//IO��������	 
#define SCCB_SCL    							PBout(10)	 	//SCL
#define SCCB_SDA    							PBout(11) 		//SDA	 

#define SCCB_READ_SDA    		PBin(11)  		//����SDA    
#define SCCB_ID   									0X42 	 			//OV7670��ID

///////////////////////////////////////////





void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif
