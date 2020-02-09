#ifndef __GT911_H
#define __GT911_H
#include "sys.h"

/*---------------------- IIC��ض��� ------------------------*/
//#define SDA_IN()  {GPIOB->MODER&=0XFFFCFFFF;GPIOB->MODER|=0X00000000;}
//#define SDA_OUT() {GPIOB->MODER&=0XFFFCFFFF;GPIOB->MODER|=0X00010000;}

/*---------------------- IIC��ض��� ------------------------*/

#define Response_OK 1  //IIC��Ӧ
#define Response_ERR 0

#define Touch_DelayVaule 10  //ͨѶ��ʱʱ��

//HAL_GPIO_ReadPin
//HAL_GPIO_WritePin
// IO�ڲ���
#define SCL(a)	if (a)	\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_7,GPIO_PIN_SET);\
					else		\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_7,GPIO_PIN_RESET)

#define SDA(a)	if (a)	\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_8,GPIO_PIN_SET);\
					else		\
					HAL_GPIO_WritePin(GPIOH,GPIO_PIN_8,GPIO_PIN_RESET)
	
/*---------------------- GT9XXоƬ��ض��� ------------------------*/
					
#define TOUCH_MAX   5	//���������

typedef struct 
{
	u8  flag;	//������־λ��Ϊ1ʱ��ʾ�д�������
	u8  num;		//��������
	u16 x[TOUCH_MAX];	//x����
	u16 y[TOUCH_MAX];	//y����
}TouchStructure;

extern TouchStructure touchInfo;	//�ṹ������

#define GT9XX_IIC_RADDR 0xBB	//IIC��ʼ����ַ
#define GT9XX_IIC_WADDR 0xBA

#define GT9XX_READ_ADDR 0x814E	//������Ϣ�Ĵ���
#define GT9XX_ID_ADDR 0x8140		//�������ID�Ĵ���

u8 Touch_Init(void);
void Touch_Scan(void);





extern u16 Touch_Num[20][4];

void Touch_Set_Rec(u16 sx,u16 sy, u16 ex,u16 ey,u32 color,u8 num);
void Touch_Set_Cir(u16 x,u16 y, u16 D,u32 color,u8 num);
u8 Touch_Single_Judge(u8 num);
u8 Touch_Get(u8 num);

#endif
