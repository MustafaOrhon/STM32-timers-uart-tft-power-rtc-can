/*
 * msp.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */
#include "main.h"

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
void HAL_UART_MspInit(UART_HandleTypeDef *huart) // Low-Level Inits of the USART2 Peripheral
{
   	//1.Enable the clock for the USART2 peripheral
    __HAL_RCC_USART2_CLK_ENABLE(); // enabled the clock for USART2
     __HAL_RCC_GPIOA_CLK_ENABLE(); // enabled the clock for GPIOA
    //2.Do the pin muxing config
     GPIO_InitTypeDef GPIOInitStruct;
     GPIOInitStruct.Pin = GPIO_PIN_2;
     GPIOInitStruct.Mode = GPIO_MODE_AF_PP;
     GPIOInitStruct.Pull = GPIO_PULLUP; // Line must be up while communication is off
     GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
     GPIOInitStruct.Alternate = GPIO_AF7_USART2; //UART TX
     HAL_GPIO_Init(GPIOA,&GPIOInitStruct);

     GPIOInitStruct.Pin = GPIO_PIN_3; //UART RX
     HAL_GPIO_Init(GPIOA,&GPIOInitStruct);


	//3.Enable the IRQ and set up the priority (NVIC Settings)

     HAL_NVIC_EnableIRQ(USART2_IRQn);
     HAL_NVIC_SetPriority(USART2_IRQn,15,0);

}
