/**
  ******************************************************************************
  * @file    DBUS.c
  * @author  huangwankuan 
  * @version V1.1
  * @date    2018-12-7
  * @brief   DBUS����
      ʵ�ʵ������ݰ��ֽڳ���25
      ��24���ֽڴ��и���־λ�����ң�������Ӷ�ʧ�����ֵ����0�������ӳɹ���Ϊ0��
  **/

#include "DBUS.h"

//#define DBUS_DEBUG   //DBUS����

UART_HandleTypeDef UART_DBUS_Handler; //UART���

DMA_HandleTypeDef  UART1RxDMA_Handler;      //DMA���

DJI_REMOTE DJI_RE;

#define Uart_size 25

u8 RX_BUF[Uart_size];

u16 Channel[6];
//ͨ��0Ϊ�ұߵ�X��
//ͨ��1Ϊ�ұߵ�Y��
//ͨ��2Ϊ��ߵ�Y��
//ͨ��3Ϊ��ߵ�X��
/**
  @function  ��Ӧ�ֲ�ƫ���ֲ�
  **/
void GetChannel(void)
	{
	DJI_RE.Right_X=((RX_BUF[2]&0x07)<<8)+RX_BUF[1];
	DJI_RE.Right_Y=((RX_BUF[3]&0x3F)<<5)+((RX_BUF[2]&0xF8)>>3);
	DJI_RE.Left_Y=((RX_BUF[5]&0x01)<<10)+(RX_BUF[4]<<2)+(RX_BUF[3]>>6);
	DJI_RE.Left_X=((RX_BUF[6]&0x0F)<<7)+(RX_BUF[5]>>1);
  switch(RX_BUF[8])
  {
    case 2: DJI_RE.SW1=UP; break;
    case 0: DJI_RE.SW1=MID; break;
    case 255: DJI_RE.SW1=DOWN; break;
    default: DJI_RE.SW1=ERR; break;            
  }
  switch(RX_BUF[10])
  {
    case 24: DJI_RE.SW2=UP; break;
    case 16: DJI_RE.SW2=MID; break;
    case 7: DJI_RE.SW2=DOWN; break;
    default: DJI_RE.SW2=ERR; break;            
  }
//  DJI_RE.SW1=(RX_BUF[6]&0x30)>>4;
//  DJI_RE.SW2=(RX_BUF[6]&0xC0)>>6;
		if(RX_BUF[23]>0)
				DJI_RE.Flag_S=LOST;//����ʧ��
		else
			 DJI_RE.Flag_S=Connected;//���ӳɹ�
}


void DBUS_Init(void)
{
	
	UART_DBUS_Handler.Instance=USART1;					    //USART1
	UART_DBUS_Handler.Init.BaudRate=100000;				    //������
	UART_DBUS_Handler.Init.WordLength=UART_WORDLENGTH_9B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART_DBUS_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART_DBUS_Handler.Init.Parity=UART_PARITY_EVEN;		    //����żУ��λ
	UART_DBUS_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART_DBUS_Handler.Init.Mode=UART_MODE_RX;		    //�շ�ģʽ
	UART_DBUS_Handler.Init.OverSampling=UART_OVERSAMPLING_8;  
	HAL_UART_Init(&UART_DBUS_Handler);					  
	
 	HAL_UART_Receive_DMA(&UART_DBUS_Handler,RX_BUF,Uart_size);
  
	DJI_RE.Left_X=1024;
	DJI_RE.Left_Y=1024;
	DJI_RE.Right_X=1024;
	DJI_RE.Right_Y=1024;
  DJI_RE.Flag_S=LOST;
  
  //HAL_UART_Receive_IT(&UART_DBUS_Handler, (u8*)RX_BUF, Uart_size);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		GetChannel();
    #ifdef DBUS_DEBUG
    
//    printf("%d %d %d %d %d\r\n",
//    DJI_RE.Left_X,DJI_RE.Left_Y,DJI_RE.Right_X,DJI_RE.Right_Y,
//    DJI_RE.Flag_S);
    for(int i=0;i<Uart_size;i++)
    {
    printf("%d ",RX_BUF[i]);
    }
    printf("\r\n");
    #endif
	  HAL_UART_Receive_IT(&UART_DBUS_Handler, (u8*)RX_BUF, Uart_size);
	}
		if(huart->Instance==USART3)
	{
      
			//printf("USART1 22222222222222222222 OK \r\n");
	}
}
//����1�жϷ������

void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART_DBUS_Handler);	//����HAL���жϴ����ú���
} 



