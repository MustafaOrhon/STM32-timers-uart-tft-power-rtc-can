/*
 * msp.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */
#include "stm32f4xx_hal.h"

void HAL_MspInit(void)  // Low Level Processor Specific Inits
{

	//1-Setup the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	//2- Enable the required system exceptions of the arm cortex mx processor refer to cortex m user manuel
    SCB->SHCSR |= 0x7 << 16; // MEMFAULT BUSFAULT USG FAULT ENABLED
	//3-Configure the priority for the system exceptions
    HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
    HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
    HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

	// Enable the clock for tim2 peripheral
	__HAL_RCC_TIM2_CLK_ENABLE();
	//Enable IRQ Handling
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	// Set up Priority number;
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);



}
