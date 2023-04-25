/*
 * it.c
 *
 *  Created on: Apr 1, 2023
 *      Author: Mustafa
 */
#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef huart2;
void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}


void USART2_IRQHandler(void)
{
 HAL_UART_IRQHandler(&huart2);

}
