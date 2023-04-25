/*
 * main.c
 *
 *  Created on: Apr 1, 2023
 *      Author: Mustafa
 */

#include "stm32f4xx_hal.h"
#include "string.h"
#include "main.h"
void SystemClockConfig(void);
void Error_Handler(void);
void TIMER2_Init(void);
void UART2_Init(void);
void GPIO_Init(void);
UART_HandleTypeDef huart2 = {0};
TIM_HandleTypeDef htimer2 = {0};
char some_data[] = "Merhabalar\r\n";

int main(void)
{
	HAL_Init(); // It must be the first function to initialize
	SystemClockConfig();

	GPIO_Init();

	UART2_Init();






    while(1)
    {
    	__WFI(); // enable WAIT FOR INSTRUCTION

    }


  return 0;
}


void SystemClockConfig(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClockInitStruct;

	memset(&RCC_OscInitStruct,0,sizeof(RCC_OscInitStruct));
	memset(&RCC_ClockInitStruct,0,sizeof(RCC_ClockInitStruct));

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16; //Default
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16; // 16/16 1Mhz
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;

	if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClockInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
			                        RCC_CLOCKTYPE_PCLK1  |RCC_CLOCKTYPE_PCLK2;
	RCC_ClockInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClockInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClockInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClockInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClockInitStruct, FLASH_ACR_LATENCY_2WS)!=HAL_OK)
	{
		Error_Handler();
	}

	// Systick config
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void Error_Handler(void)
{
  while(1);
}



void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//Enable IRQ Handling
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	// Set up Priority number;
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,14, 0);



}
void HAL_GPIO_EXTI_Callback(uint16_t  GPIO_Pin)
{
    if(HAL_UART_Transmit(&huart2,(uint8_t*)some_data,(uint16_t)strlen((char*)some_data),HAL_MAX_DELAY)!= HAL_OK)
    {

    	Error_Handler();
    }
}
void UART2_Init(void)  // High Level Init
{
   huart2.Instance = USART2; //Linking handle with usart2
   huart2.Init.BaudRate =  115200;
   huart2.Init.WordLength = UART_WORDLENGTH_8B;
   huart2.Init.StopBits = UART_STOPBITS_1;
   huart2.Init.Parity = UART_PARITY_NONE;
   huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart2.Init.Mode = UART_MODE_TX_RX;

   if(HAL_UART_Init(&huart2) != HAL_OK)
   {
	     // There is a problem
	      Error_Handler();
   }


}

