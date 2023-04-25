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
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;
TIM_OC_InitTypeDef hoctimer2;
uint32_t pulse1_value = 42000;
uint32_t ccr_content;
int main(void)
{
	HAL_Init(); // It must be the first function to initialize
	SystemClockConfig();
	GPIO_Init();
	UART2_Init();
    TIMER2_Init();

    if(HAL_TIM_OC_Start(&htimer2, TIM_CHANNEL_1)!= HAL_OK)
    {
    	Error_Handler();
    }
    while(1);


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
void TIMER2_Init(void)
{
   htimer2.Instance = TIM2;
   htimer2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
   htimer2.Init.Period = 0xFFFFFFFF;
   htimer2.Init.Prescaler = 1;

   if(HAL_TIM_OC_Init(&htimer2)!= HAL_OK)
   {
	   Error_Handler();

   }

   hoctimer2.OCMode = TIM_OCMODE_TOGGLE;
   hoctimer2.Pulse = pulse1_value;
   hoctimer2.OCPolarity = TIM_OCPOLARITY_HIGH;

   if(HAL_TIM_OC_ConfigChannel(&htimer2, &hoctimer2, TIM_CHANNEL_1)!= HAL_OK)
      {
   	   Error_Handler();

      }

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

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	  //TIM2_CH1 Toggle with 500 hz
      if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
      {

    	   ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
      }


}
