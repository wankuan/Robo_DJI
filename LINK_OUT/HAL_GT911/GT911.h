#ifndef __GT911_H
#define __GT911_H
#include "sys.h"

/*---------------------- IIC相关定义 ------------------------*/
//#define SDA_IN()  {GPIOB->MODER&=0XFFFCFFFF;GPIOB->MODER|=0X00000000;}
//#define SDA_OUT() {GPIOB->MODER&=0XFFFCFFFF;GPIOB->MODER|=0X00010000;}

/*---------------------- IIC相关定义 ------------------------*/

#define Response_OK 1  //IIC响应
#define Response_ERR 0

#define Touch_DelayVaule 10  //通讯延时时间

//HAL_GPIO_ReadPin
//HAL_GPIO_WritePin
// IO口操作
#define SCL(a)	if (a)	\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_7,GPIO_PIN_SET);\
					else		\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_7,GPIO_PIN_RESET)

#define SDA(a)	if (a)	\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_8,GPIO_PIN_SET);\
					else		\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_8,GPIO_PIN_RESET)
	
/*---------------------- GT9XX芯片相关定义 ------------------------*/
					
#define TOUCH_MAX   5	//最大触摸点数

typedef struct 
{
	u8  flag;	//触摸标志位，为1时表示有触摸操作
	u8  num;		//触摸点数
	u16 x[TOUCH_MAX];	//x坐标
	u16 y[TOUCH_MAX];	//y坐标
}TouchStructure;

extern TouchStructure touchInfo;	//结构体声明

#define GT9XX_IIC_RADDR 0xBB	//IIC初始化地址
#define GT9XX_IIC_WADDR 0xBA

#define GT9XX_READ_ADDR 0x814E	//触摸信息寄存器
#define GT9XX_ID_ADDR 0x8140		//触摸面板ID寄存器

u8 Touch_Init(void);
void Touch_Scan(void);





extern u16 Touch_Num[20][4];

void Touch_Set_Rec(u16 sx,u16 sy, u16 ex,u16 ey,u32 color,u8 num);
void Touch_Set_Cir(u16 x,u16 y, u16 D,u32 color,u8 num);
u8 Touch_Single_Judge(u8 num);
u8 Touch_Get(u8 num);

#endif
