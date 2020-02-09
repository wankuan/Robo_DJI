#ifndef __CAN_H_
#define __CAN_H_

#include "sys.h"

extern CAN_HandleTypeDef   CAN_ESC_Handler;   //CAN1句柄
extern CanTxMsgTypeDef     TxMessage;      //发送消息
extern CanRxMsgTypeDef     RxMessage;      //接收消息
extern u8 MOTOR_IN;


typedef enum
{

	CAN_3508_Motor_ID_ALL = 0x200,
	CAN_3508_Motor_ID_1 = 0x201,
	CAN_3508_Motor_ID_2 = 0x202,
	CAN_3508_Motor_ID_3 = 0x203,
	CAN_3508_Motor_ID_4 = 0x204,
}CAN_Message_ID;

u8 CAN1_Mode_Init(u32 mode);
void MOTOR_SET_Current(int16_t M1, int16_t M2, int16_t M3, int16_t M4);
void CAN1_Send_Msg(u16 STDID,u8* data,u8 size);
u8 CAN1_Receive_Msg(u8 *buf);

#endif

