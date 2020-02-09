#ifndef _DBUS_H_
#define _DBUS_H_
#include "sys.h"

void DBUS_Init(void);
void GetChannel(void);
extern DMA_HandleTypeDef  UART1RxDMA_Handler;      //DMA句柄
extern UART_HandleTypeDef UART_DBUS_Handler; //UART句柄
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
 //我们的遥控器不是DT7，是DJ6，编码不一样SW1和SW2的编码暂时没搞明白，所以添加ERR
};

enum
{
Connected = 0,
LOST = 1 ,
};


#endif 
