/*
 * it.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */
#include "stm32f4xx_hal.h"
extern TIM_HandleTypeDef htimer2;
void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htimer2);
}
