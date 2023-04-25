/*
 * it.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */
#include "stm32f4xx_hal.h"
void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}
void TIM2_IRQHandler(void)
{

}
