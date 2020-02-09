#include "HAL_9250.h"
#include "OLED.h"

UART_HandleTypeDef UART7_Handler; //UART���

#define Uart_size 4

u8 RX_BUF[Uart_size];
u8 j;


void UART7_Init(void)
{
	UART7_Handler.Instance=UART7;					    //USART1
	UART7_Handler.Init.BaudRate=115200;				    //������
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART7_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
 HAL_UART_Receive_IT(&UART7_Handler,RX_BUF,Uart_size);
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==UART7)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_UART7_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	//����ΪUSART1
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	//��ʼ��PA9
		
		HAL_NVIC_EnableIRQ(UART7_IRQn);		
		HAL_NVIC_SetPriority(UART7_IRQn,1,1);
	}
	
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART7)
	{
		j++;
		OLED_OUT_Num(1,4,j,10);
	}
}
 
void UART7_IRQHandler(void)
{
		HAL_UART_IRQHandler(&UART7_Handler);	//����HAL���жϴ����ú���
}

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
	while((UART7->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}

