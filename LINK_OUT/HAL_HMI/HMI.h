#ifndef __HMI_H
#define __HMI_H

#include "sys.h"
#include "USART.h"

void HMI_Send_Data_End(void);
void HMI_Send_REF(char* Data1);
void HMI_Send_Data(char* Data1,char* Data2);
void HMI_Send_Data_Hex(char* Data1,u8 Data2);
void HMI_Send_Single_Char(char* Data1);
void HMI_Send_Single_Num(u8 Data1);
void HMI_Change_Light(u8 Data);
#endif

