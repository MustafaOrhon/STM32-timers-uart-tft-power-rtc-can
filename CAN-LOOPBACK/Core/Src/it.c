/*
 * it.c
 *
 *  Created on: Apr 2, 2023
 *      Author: Mustafa
 */
void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}


