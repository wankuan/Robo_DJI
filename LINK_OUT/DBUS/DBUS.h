#ifndef _DBUS_H_
#define _DBUS_H_
#include "sys.h"

void DBUS_Init(void);
void GetChannel(void);
extern DMA_HandleTypeDef  UART1RxDMA_Handler;      //DMA���
extern UART_HandleTypeDef UART_DBUS_Handler; //UART���
typedef struct{
	u16 Left_X;
	u16 Left_Y;
	u16 Right_X;
	u16 Right_Y;
  u8  SW1;
  u8  SW2;
	u8  Flag_S;
}DJI_REMOTE;

enum
{
 UP = 0 ,
 MID = 1 ,
 DOWN = 2 ,
 ERR = 3 ,
 //���ǵ�ң��������DT7����DJ6�����벻һ��SW1��SW2�ı�����ʱû�����ף��������ERR
};

enum
{
Connected = 0,
LOST = 1 ,
};


#endif 
