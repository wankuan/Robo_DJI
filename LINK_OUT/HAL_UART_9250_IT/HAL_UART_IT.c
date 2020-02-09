#include "HAL_9250.h"
#include "OLED.h"

UART_HandleTypeDef UART7_Handler; //UART句柄

#define Uart_size 4

u8 RX_BUF[Uart_size];
u8 j;


void UART7_Init(void)
{
	UART7_Handler.Instance=UART7;					    //USART1
	UART7_Handler.Init.BaudRate=115200;				    //波特率
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART7_Handler);					    //HAL_UART_Init()会使能UART1
	
 HAL_UART_Receive_IT(&UART7_Handler,RX_BUF,Uart_size);
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==UART7)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_UART7_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//高速
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	//复用为USART1
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	//初始化PA9
		
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
		HAL_UART_IRQHandler(&UART7_Handler);	//调用HAL库中断处理公用函数
}

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((UART7->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

