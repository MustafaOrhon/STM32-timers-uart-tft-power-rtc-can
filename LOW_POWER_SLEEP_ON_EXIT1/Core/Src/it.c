/*
 * it.c
 *
 *  Created on: Apr 1, 2023
 *      Author: Mustafa
 */
#include "stm32f4xx_hal.h"
extern TIM_HandleTypeDef htimer2;
extern UART_HandleTypeDef huart2;
void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}
void TIM2_IRQHandler(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_TIM_IRQHandler(&htimer2);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}
void USART2_IRQHandler(void)
{
 HAL_UART_IRQHandler(&huart2);

}
