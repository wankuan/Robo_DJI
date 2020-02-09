/**
  ******************************************************************************
  * @file    Templates/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.2.5
  * @date    04-November-2016
  * @brief   HAL MSP module.
  *         
  @verbatim
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..]
    This file is generated automatically by STM32CubeMX and eventually modified 
    by the user

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually  
            modified by the user
   */ 
    //板载按键外部中断
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,2);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线2
   
   	HAL_NVIC_EnableIRQ(USART3_IRQn);		//使能USART1中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,2,3);	//抢占优先级3，子优先级3
    
//    		HAL_NVIC_SetPriority(USART1_IRQn,2,0);	//抢占优先级3，子优先级3
//			HAL_NVIC_EnableIRQ(USART1_IRQn);		//使能USART1中断通道
      

   HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,2,0);
   HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
      
   HAL_NVIC_SetPriority(CAN1_RX0_IRQn,1,1); 
   HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);   
    
	 HAL_NVIC_SetPriority(CAN1_TX_IRQn,1,0); 
   HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);       
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually  
            modified by the user
   */
}


//CAN底层驱动，引脚配置，时钟配置，中断配置
//此函数会被HAL_CAN_Init()调用
//hcan:CAN句柄
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //使能CAN1时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();			   
		
		
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;   //
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //快速
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //复用为CAN1
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);         //初始化
	
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	
	GPIO_InitTypeDef GPIO_Initure;
		if(huart->Instance==USART3)
	{
		
		__HAL_RCC_GPIOD_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_8;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;	
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART1
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_9;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;	
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART1
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PA9
	
	}
	if(huart->Instance==USART1)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	  __HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	
   
		GPIO_Initure.Pin=GPIO_PIN_7;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_NOPULL;	
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//复用为USART1
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA9

    //Rx DMA配置
    UART1RxDMA_Handler.Instance=DMA2_Stream5;                            //数据流选择
    UART1RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //通道选择
    UART1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //存储器到外设
    UART1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
    UART1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
    UART1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
    UART1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
    UART1RxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //外设普通模式
    UART1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
    UART1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
    UART1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
    UART1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
    UART1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
    
    
    HAL_DMA_DeInit(&UART1RxDMA_Handler);   
    HAL_DMA_Init(&UART1RxDMA_Handler);
	
		__HAL_LINKDMA(&UART_DBUS_Handler,hdmarx,UART1RxDMA_Handler);    //将DMA与USART1联系起来(发送DMA)
	}
	
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
