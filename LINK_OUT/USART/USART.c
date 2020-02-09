#include "USART.h"


///����DMA�ʼ�
/**
*@brief : ʹ��DMA���д��ڽ���
1.���ڳ�ʼ������
2.DMA��ʼ�����ã�ÿ��DMA��ʼ������ǰ��Ҫ���з���ʼ��Deinit��
3.HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);�򿪴���DMA����


**/



UART_HandleTypeDef UART1_Handler; //UART���
DMA_HandleTypeDef  UART1RxDMA_Handler;      //DMA���
#define Uart_size 8

u8 RX_BUF[Uart_size];
void UART1_Init(void)
{
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=115200;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//HAL_DMA_Start_IT(&UART1RxDMA_Handler,(uint32_t)&UART1_Handler.Instance->DR,(u32)RX_BUF, Uart_size);//����DMA����
	
  //UART1_Handler.Instance->CR3 |= USART_CR3_DMAR;//
	HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)RX_BUF, Uart_size);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	

}

// void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	if(huart->Instance==USART1)
//	{
//		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
//		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
//	  __HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	
//	
//	
//		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
//		GPIO_Initure.Pull=GPIO_PULLUP;			//����
//		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//����
//		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
//		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

//		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
//		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
//		
//    //Rx DMA����
//    UART1RxDMA_Handler.Instance=DMA2_Stream5;                            //������ѡ��
//    UART1RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //ͨ��ѡ��
//    UART1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //�洢��������
//    UART1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
//    UART1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
//    UART1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
//    UART1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
//    UART1RxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //������ͨģʽ
//    UART1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
//    UART1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
//    UART1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
//    UART1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
//    UART1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
//    
//    HAL_DMA_DeInit(&UART1RxDMA_Handler);   
//    HAL_DMA_Init(&UART1RxDMA_Handler);
//	
//		 __HAL_LINKDMA(&UART1_Handler,hdmarx,UART1RxDMA_Handler);    //��DMA��USART1��ϵ����(����DMA)
//		//HAL_NVIC_EnableIRQ(USART1_IRQn);		//ʹ��USART1�ж�ͨ��
//		//HAL_NVIC_SetPriority(USART1_IRQn,3,2);	//��ռ���ȼ�3�������ȼ�3
//	
//		HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);		
//		HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,3,3);
//	
//	}
//	
//}

//void DMA2_Stream5_IRQHandler(void)
//{
//	HAL_DMA_IRQHandler(&UART1RxDMA_Handler);	//����HAL���жϴ����ú���
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)//����Ǵ���1   ����һ���������
//	{
//		//	printf("�������\n");
//		//	HAL_UART_Transmit(&UART1_Handler,RX_BUF,sizeof(RX_BUF),1000);
//	/*	for(i=0;i<sizeof(RX_BUF);i++)
//		{
//		RX_BUF[i]=0;
//		}*/
//		
//	}
//}
 
//����1�жϷ������
/*void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	if(HAL_UART_GetState(&UART1_Handler)==HAL_UART_STATE_READY)
	{
			HAL_UART_Receive_IT(&UART1_Handler,(u8 *)RX_BUF,Uart_size);
	}
} */



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
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}

