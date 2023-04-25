/*
 * it.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */
#include "main.h"
extern UART_HandleTypeDef huart2;
void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}
