#include "HA_USART_DMA.h"
///串口DMA笔记
/**
*@brief : 使用DMA进行串口接收
1.串口初始化配置
2.DMA初始化配置（每次DMA初始化配置前需要进行反初始化Deinit）
3.__HAL_LINKDMA(&USART_Handler,hdmarx,USARTRxDMA_Handler);
4.HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);打开串口DMA接收
5.HAL_DMA_IRQHandler(&USARTRxDMA_Handler);	//调用HAL库中断处理公用函数
6.void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TCIF3_7);//清除DMA2_Steam7传输完成标志
HAL_UART_DMAStop(&UART1_Handler);      //传输完成以后关闭串口DMA


会调用串口完全接收数据中断  而不是调用DMA自带的函数映射
**/

UART_HandleTypeDef USART_P1_Handler; //UART句柄
DMA_HandleTypeDef  USARTRxDMA_Handler;      //DMA句柄

//[7]:1-双方已准备好
//[6];1-接收校准成功指令

#define  RXBUFFERSIZE 20       //接收状态标记	

u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

void UART_Init(void)
{
	USART_P1_Handler.Instance=USART3;					   
	USART_P1_Handler.Init.BaudRate=115200;				    //波特率
	USART_P1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	USART_P1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	USART_P1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	USART_P1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	USART_P1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&USART_P1_Handler);					    //HAL_UART_Init()会使能UART1
	
	//HAL_UART_Receive_IT(&USART_P1_Handler, (u8*)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
	//Rx DMA配置
//	USARTRxDMA_Handler.Instance=DMA1_Stream5;                            //数据流选择
//	USARTRxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //通道选择
//	USARTRxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;           
//	USARTRxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
//	USARTRxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
//	USARTRxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
//	USARTRxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
//	USARTRxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //外设普通模式
//	USARTRxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
//	USARTRxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
//	USARTRxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
//	USARTRxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
//	USARTRxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
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
//	HAL_DMA_IRQHandler(&USARTRxDMA_Handler);	//调用HAL库中断处理公用函数
//}


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
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
	USART3->DR = (u8) ch;      
	return ch;
}

//void USART1_IRQHandler(void)                	
//{ 
//	HAL_UART_IRQHandler(&USART1_Handler);	//调用HAL库中断处理公用函数
////	if(HAL_UART_GetState(&UART_DBUS_Handler)==HAL_UART_STATE_READY)
////	{
////			HAL_UART_Receive_IT(&UART_DBUS_Handler,(u8 *)RX_BUF,Uart_size);
////	}
//} 


//void EXTI4_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//调用中断处理公用函数
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



