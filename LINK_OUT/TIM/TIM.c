#include "TIM.h"

/**
* @Notice: The Timer of APB2 up to 180MHZ and the Timer of APB1 up to 180MHZ OR 90MHZ!!!!!
**/
#define TIM1_IT
TIM_HandleTypeDef TIM1_Handler;  
TIM_OC_InitTypeDef TIM1_CH1Handler;	 

void TIM1_Init(u16 psc,u16 arr)
{
	   TIM1_Handler.Instance=TIM1;         
    TIM1_Handler.Init.Prescaler=psc-1;   
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM1_Handler.Init.Period=arr-1;         
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	   HAL_TIM_Base_Init(&TIM1_Handler);
	   
	  #ifdef TIM1_IT
	  HAL_TIM_Base_Start_IT(&TIM1_Handler); 
   #else
			HAL_TIM_Base_Start(&TIM1_Handler);	
	  #endif

	   
}
void TIM1_PWM_Init(u16 psc,u16 arr,u16 counter)
{ 
	   TIM1_Init(psc,arr);
    TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; 
    TIM1_CH1Handler.Pulse=counter;            
    TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; 
    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_4);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_TIM1_CLK_ENABLE();			
    __HAL_RCC_GPIOA_CLK_ENABLE();		
	
    GPIO_Initure.Pin=GPIO_PIN_11;          
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	
    GPIO_Initure.Pull=GPIO_PULLUP;          
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    
	   GPIO_Initure.Alternate= GPIO_AF1_TIM1;	
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		#ifdef TIM1_IT
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn,0,3);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);          
  #endif		
	}
	
}

void TIM_SetCompare1(TIM_TypeDef* TIMx,u32 compare1)
{
  TIMx->CCR1 = compare1;
}
void TIM_SetCompare2(TIM_TypeDef* TIMx,u32 compare2)
{
  TIMx->CCR2 = compare2;
}
void TIM_SetCompare3(TIM_TypeDef* TIMx,u32 compare3)
{
  TIMx->CCR3 = compare3;
}
void TIM_SetCompare4(TIM_TypeDef* TIMx,u32 compare4)
{
  TIMx->CCR4 = compare4;
}

void TIM1_UP_TIM10_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TIM1_Handler);
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim==(&TIM1_Handler))
//    {
//			     
//    }
//}


