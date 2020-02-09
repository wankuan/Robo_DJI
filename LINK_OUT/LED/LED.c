#include "LED.h"

void LED_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
	 
    __HAL_RCC_GPIOE_CLK_ENABLE(); 	
    __HAL_RCC_GPIOF_CLK_ENABLE(); 	
	
		GPIO_Initure.Pin = GPIO_PIN_11; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
    GPIO_Initure.Pin = GPIO_PIN_14; 
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
		LED_R=LED_OFF;
		LED_G=LED_OFF;
}

