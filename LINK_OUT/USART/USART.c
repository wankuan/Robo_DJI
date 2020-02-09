#include "USART.h"


///串口DMA笔记
/**
*@brief : 使用DMA进行串口接收
1.串口初始化配置
2.DMA初始化配置（每次DMA初始化配置前需要进行反初始化Deinit）
3.HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);打开串口DMA接收


**/



UART_HandleTypeDef UART1_Handler; //UART句柄
DMA_HandleTypeDef  UART1RxDMA_Handler;      //DMA句柄
#define Uart_size 8

u8 RX_BUF[Uart_size];
void UART1_Init(void)
{
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=115200;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	//HAL_DMA_Start_IT(&UART1RxDMA_Handler,(uint32_t)&UART1_Handler.Instance->DR,(u32)RX_BUF, Uart_size);//开启DMA传输
	
  //UART1_Handler.Instance->CR3 |= USART_CR3_DMAR;//
	HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)RX_BUF, Uart_size);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	

}

// void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	if(huart->Instance==USART1)
//	{
//		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
//		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
//	  __HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	
//	
//	
//		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
//		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
//		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//高速
//		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
//		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

//		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
//		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
//		
//    //Rx DMA配置
//    UART1RxDMA_Handler.Instance=DMA2_Stream5;                            //数据流选择
//    UART1RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //通道选择
//    UART1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //存储器到外设
//    UART1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
//    UART1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
//    UART1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
//    UART1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
//    UART1RxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //外设普通模式
//    UART1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
//    UART1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
//    UART1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
//    UART1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
//    UART1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
//    
//    HAL_DMA_DeInit(&UART1RxDMA_Handler);   
//    HAL_DMA_Init(&UART1RxDMA_Handler);
//	
//		 __HAL_LINKDMA(&UART1_Handler,hdmarx,UART1RxDMA_Handler);    //将DMA与USART1联系起来(发送DMA)
//		//HAL_NVIC_EnableIRQ(USART1_IRQn);		//使能USART1中断通道
//		//HAL_NVIC_SetPriority(USART1_IRQn,3,2);	//抢占优先级3，子优先级3
//	
//		HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);		
//		HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,3,3);
//	
//	}
//	
//}

//void DMA2_Stream5_IRQHandler(void)
//{
//	HAL_DMA_IRQHandler(&UART1RxDMA_Handler);	//调用HAL库中断处理公用函数
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)//如果是串口1   接收一次数据完成
//	{
//		//	printf("接收完成\n");
//		//	HAL_UART_Transmit(&UART1_Handler,RX_BUF,sizeof(RX_BUF),1000);
//	/*	for(i=0;i<sizeof(RX_BUF);i++)
//		{
//		RX_BUF[i]=0;
//		}*/
//		
//	}
//}
 
//串口1中断服务程序
/*void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
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
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

