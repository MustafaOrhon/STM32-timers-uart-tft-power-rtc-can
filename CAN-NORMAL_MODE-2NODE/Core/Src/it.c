/*
 * it.c
 *
 *  Created on: Apr 2, 2023
 *      Author: Mustafa
 */
#include "main.h"
extern TIM_HandleTypeDef htimer6;
extern CAN_HandleTypeDef hcan1;
void SysTick_Handler(void)
{
   HAL_IncTick();
   HAL_SYSTICK_IRQHandler();

}

void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htimer6);
}
void EXTI0_IRQHandler(void)
{
	HAL_TIM_Base_Start_IT(&htimer6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
void CAN1_TX_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_RX1_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_SCE_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan1);
}
