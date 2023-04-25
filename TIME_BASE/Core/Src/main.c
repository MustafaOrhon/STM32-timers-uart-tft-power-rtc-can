/*
 * main.c
 *
 *  Created on: Mar 31, 2023
 *      Author: Mustafa
 */

#include "stm32f4xx_hal.h"
#include "string.h"
#include "main.h"
void SystemClockConfig(void);
void Error_Handler(void);
void TIMER2_Init(void);
void GPIO_Init(void);
TIM_HandleTypeDef htimer2;
int main(void)
{
	HAL_Init(); // It must be the first function to initialize
	SystemClockConfig();
	GPIO_Init();
	TIMER2_Init();
    HAL_TIM_Base_Start(&htimer2);

	while(1)
	{
		/* Loop until the update event flag is set */
		while(__HAL_TIM_GET_FLAG(&htimer2,TIM_FLAG_UPDATE) == RESET);
		/* The required time delay has been elapsed */
		/* User code can be executed */
		__HAL_TIM_CLEAR_FLAG(&htimer2,TIM_FLAG_UPDATE);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	}
  return 0;
}


void SystemClockConfig(void)
{

}

void Error_Handler(void)
{
  while(1);
}
void TIMER2_Init(void)
{

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler = 1599;
	htimer2.Init.Period = 9999;

	HAL_TIM_Base_Init(&htimer2);



}
void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



}
