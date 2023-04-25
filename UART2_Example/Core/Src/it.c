/*
 * it.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */

void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}
