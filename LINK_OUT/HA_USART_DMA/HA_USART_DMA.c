#include "HA_USART_DMA.h"
///����DMA�ʼ�
/**
*@brief : ʹ��DMA���д��ڽ���
1.���ڳ�ʼ������
2.DMA��ʼ�����ã�ÿ��DMA��ʼ������ǰ��Ҫ���з���ʼ��Deinit��
3.__HAL_LINKDMA(&USART_Handler,hdmarx,USARTRxDMA_Handler);
4.HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);�򿪴���DMA����
5.HAL_DMA_IRQHandler(&USARTRxDMA_Handler);	//����HAL���жϴ����ú���
6.void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TCIF3_7);//���DMA2_Steam7������ɱ�־
HAL_UART_DMAStop(&UART1_Handler);      //��������Ժ�رմ���DMA


����ô�����ȫ���������ж�  �����ǵ���DMA�Դ��ĺ���ӳ��
**/

UART_HandleTypeDef USART_P1_Handler; //UART���
DMA_HandleTypeDef  USARTRxDMA_Handler;      //DMA���

//[7]:1-˫����׼����
//[6];1-����У׼�ɹ�ָ��

#define  RXBUFFERSIZE 20       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

void UART_Init(void)
{
	USART_P1_Handler.Instance=USART3;					   
	USART_P1_Handler.Init.BaudRate=115200;				    //������
	USART_P1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_P1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	USART_P1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	USART_P1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	USART_P1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&USART_P1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//HAL_UART_Receive_IT(&USART_P1_Handler, (u8*)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
	//Rx DMA����
//	USARTRxDMA_Handler.Instance=DMA1_Stream5;                            //������ѡ��
//	USARTRxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //ͨ��ѡ��
//	USARTRxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;           
//	USARTRxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
//	USARTRxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
//	USARTRxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
//	USARTRxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
//	USARTRxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //������ͨģʽ
//	USARTRxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
//	USARTRxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
//	USARTRxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
//	USARTRxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
//	USARTRxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
////	__HAL_UART_ENABLE(&USART_Handler);
//	HAL_DMA_DeInit(&USARTRxDMA_Handler);   
//	HAL_DMA_Init(&USARTRxDMA_Handler);
	
//	HAL_DMA_RegisterCallback(&UART7RxDMA_Handler, HAL_DMA_XFER_CPLT_CB_ID, UART7_DMA_Accomplish_Callback);
//	HAL_DMA_RegisterCallback(&UART7RxDMA_Handler, HAL_DMA_XFER_HALFCPLT_CB_ID, UART7_DMA_HAlf);
//	

	//	__HAL_LINKDMA(&USART_Handler,hdmarx,USARTRxDMA_Handler);
	//	HAL_UART_Receive_DMA(&USART_Handler,RX_BUF,Uart_size);
}



//void DMA1_Stream5_IRQHandler(void)
//{
//	HAL_DMA_IRQHandler(&USARTRxDMA_Handler);	//����HAL���жϴ����ú���
//}


struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
	USART3->DR = (u8) ch;      
	return ch;
}

//void USART1_IRQHandler(void)                	
//{ 
//	HAL_UART_IRQHandler(&USART1_Handler);	//����HAL���жϴ����ú���
////	if(HAL_UART_GetState(&UART_DBUS_Handler)==HAL_UART_STATE_READY)
////	{
////			HAL_UART_Receive_IT(&UART_DBUS_Handler,(u8 *)RX_BUF,Uart_size);
////	}
//} 


//void EXTI4_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//�����жϴ����ú���
//}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//				if(GPIO_Pin==GPIO_PIN_4) 
//		{
//	 __HAL_UART_ENABLE(&USART_Handler);
//	//		HAL_UART_Receive_IT(&USART_Handler,RX_BUF,Uart_size);
//		//tmp = (uint16_t)(USART_Handler.Instance->DR & (uint16_t)0x00FFU);
//		HAL_UART_Receive_DMA(&USART_Handler,RX_BUF,Uart_size);
//		}	
//}



