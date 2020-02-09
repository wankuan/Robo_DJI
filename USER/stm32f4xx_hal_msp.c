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
    //���ذ����ⲿ�ж�
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //ʹ���ж���2
   
   	HAL_NVIC_EnableIRQ(USART3_IRQn);		//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART3_IRQn,2,3);	//��ռ���ȼ�3�������ȼ�3
    
//    		HAL_NVIC_SetPriority(USART1_IRQn,2,0);	//��ռ���ȼ�3�������ȼ�3
//			HAL_NVIC_EnableIRQ(USART1_IRQn);		//ʹ��USART1�ж�ͨ��
      

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


//CAN�ײ��������������ã�ʱ�����ã��ж�����
//�˺����ᱻHAL_CAN_Init()����
//hcan:CAN���
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //ʹ��CAN1ʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();			   
		
		
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;   //
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //����ΪCAN1
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);         //��ʼ��
	
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	
	GPIO_InitTypeDef GPIO_Initure;
		if(huart->Instance==USART3)
	{
		
		__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_8;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;	
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART1
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_9;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;	
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART1
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//��ʼ��PA9
	
	}
	if(huart->Instance==USART1)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	  __HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	
   
		GPIO_Initure.Pin=GPIO_PIN_7;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_NOPULL;	
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART1
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PA9

    //Rx DMA����
    UART1RxDMA_Handler.Instance=DMA2_Stream5;                            //������ѡ��
    UART1RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //ͨ��ѡ��
    UART1RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //�洢��������
    UART1RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
    UART1RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
    UART1RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
    UART1RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
    UART1RxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //������ͨģʽ
    UART1RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
    UART1RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
    UART1RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
    UART1RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
    UART1RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
    
    
    HAL_DMA_DeInit(&UART1RxDMA_Handler);   
    HAL_DMA_Init(&UART1RxDMA_Handler);
	
		__HAL_LINKDMA(&UART_DBUS_Handler,hdmarx,UART1RxDMA_Handler);    //��DMA��USART1��ϵ����(����DMA)
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
