#ifndef __TIM_H
#define __TIM_H

#include "sys.h"
extern TIM_HandleTypeDef TIM1_Handler;  

void TIM1_Init(u16 psc,u16 arr);
void TIM1_PWM_Init(u16 psc,u16 arr,u16 counter);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void TIM_SetCompare1(TIM_TypeDef* TIMx,u32 compare1);
void TIM_SetCompare2(TIM_TypeDef* TIMx,u32 compare2);
void TIM_SetCompare3(TIM_TypeDef* TIMx,u32 compare3);
void TIM_SetCompare4(TIM_TypeDef* TIMx,u32 compare4);

#endif 


