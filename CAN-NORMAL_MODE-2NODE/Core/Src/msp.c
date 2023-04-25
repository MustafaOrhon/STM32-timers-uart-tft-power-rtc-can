/*
 * msp.c
 *
 *  Created on: Apr 3, 2023
 *      Author: Mustafa
 */
#include "main.h"

void HAL_MspInit(void)
{
 //Here will do low level processor specific inits.
	//1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exceptions

	//3. configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	 GPIO_InitTypeDef gpio_uart;
	 //here we are going to do the low level inits. of the USART2 peripheral

	 //1. enable the clock for the USART2 peripheral as well as for GPIOA peripheral
	 __HAL_RCC_USART2_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();

	 //2 . Do the pin muxing configurations
	 gpio_uart.Pin = GPIO_PIN_2;
	 gpio_uart.Mode =GPIO_MODE_AF_PP;
	 gpio_uart.Pull = GPIO_PULLUP;
	 gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	 gpio_uart.Alternate =  GPIO_AF7_USART2; //UART2_TX
	 HAL_GPIO_Init(GPIOA,&gpio_uart);

	 gpio_uart.Pin = GPIO_PIN_3; //UART2_RX
	 HAL_GPIO_Init(GPIOA,&gpio_uart);
	 //3 . Enable the IRQ and set up the priority (NVIC settings )
	 HAL_NVIC_EnableIRQ(USART2_IRQn);
	 HAL_NVIC_SetPriority(USART2_IRQn,15,0);

}
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	// Enable the clock
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	// Config Pins
	GPIO_InitTypeDef GPIO_Can;
	GPIO_Can.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_Can.Mode = GPIO_MODE_AF_PP;
	GPIO_Can.Pull = GPIO_NOPULL;
	GPIO_Can.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Can.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOD,&GPIO_Can);

	// ENABLE NVIC
	   HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	   HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	   HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	   HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

	   HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15,0);
	   HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15,0);
	   HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15,0);
	   HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15,0);


}
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

	// Enable the clock for tim2 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();
	//Enable IRQ Handling
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	// Set up Priority number;
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);



}
