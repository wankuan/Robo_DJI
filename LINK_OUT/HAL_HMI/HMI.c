#include "HMI.h"

extern UART_HandleTypeDef UART1_Handler;

void HMI_Send_Data_End()//0xff
{
		u8 a[3] = {0xff,0xff,0xff};
		HAL_UART_Transmit(&UART1_Handler,a,sizeof(a),1000);		
}
void HMI_Send_Data(char* Data1,char* Data2)
{
		printf("%s",Data1);
		printf("%s","\"");
		printf("%s",Data2);
		printf("\"");
}

void HMI_Send_Data_Hex(char* Data1,u8 Data2)
{
		printf("%s",Data1);
		printf("%s","\"");
		printf("0x%x",Data2);
		printf("\"");
}

void HMI_Send_Single_Char(char* Data1)
{
	printf("%s",Data1);
}

void HMI_Send_Single_Num(u8 Data1)
{
	HAL_UART_Transmit(&UART1_Handler,&Data1,sizeof(Data1),1000);	
}

void HMI_Send_REF(char* Data1)
{
	printf("ref %s",Data1);
	HMI_Send_Data_End();
}

void HMI_Change_Light(u8 Data)
{
	printf("dim=%d",Data);
	HMI_Send_Data_End();
}

	

