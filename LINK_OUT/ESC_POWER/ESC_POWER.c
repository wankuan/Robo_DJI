#include "ESC_POWER.h"

void ESC_POWER(void)
{
		GPIO_InitTypeDef GPIO_Initure;
	 
    __HAL_RCC_GPIOH_CLK_ENABLE(); 	 	
	
		GPIO_Initure.Pin = GPIO_PIN_2| GPIO_PIN_3| GPIO_PIN_4| GPIO_PIN_5; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
   HAL_GPIO_WritePin(GPIOH,GPIO_PIN_2, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOH,GPIO_PIN_3, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5, GPIO_PIN_SET);   
   

}
