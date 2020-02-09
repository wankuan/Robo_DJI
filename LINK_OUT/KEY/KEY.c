#include "KEY.h"

void KEY_Init(void)
{

		GPIO_InitTypeDef GPIO_Initure;
		
		__HAL_RCC_GPIOB_CLK_ENABLE();           
		
		GPIO_Initure.Pin=GPIO_PIN_2; 
		GPIO_Initure.Mode=GPIO_MODE_IT_RISING;
		GPIO_Initure.Pull=GPIO_PULLDOWN;        
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
}

