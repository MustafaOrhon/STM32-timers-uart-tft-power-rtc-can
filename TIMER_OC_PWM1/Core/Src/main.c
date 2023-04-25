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
TIM_OC_InitTypeDef hoctimer2 = {0};

int main(void)
{
	HAL_Init(); // It must be the first function to initialize
	SystemClockConfig();
	GPIO_Init();
	UART2_Init();
    TIMER2_Init();

    if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_1)!= HAL_OK)
    {
    	Error_Handler();
    }
    if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_2)!= HAL_OK)
    {
    	Error_Handler();
    }
    if(HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_3)!= HAL_OK)
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
	   htimer2.Init.Period = 20000-1;
	   htimer2.Init.Prescaler = 41999;

	   if(HAL_TIM_PWM_Init(&htimer2)!= HAL_OK)
	   {
		   Error_Handler();

	   }
	   hoctimer2.OCMode = TIM_OCMODE_PWM1;
	   hoctimer2.OCPolarity = TIM_OCPOLARITY_HIGH;
	   hoctimer2.Pulse = (htimer2.Init.Period * 25) / 100; // 25 Duty Cycle
	   if(HAL_TIM_PWM_ConfigChannel(&htimer2,&hoctimer2,TIM_CHANNEL_1)!= HAL_OK)
	   {
		   Error_Handler();

	   }
	   hoctimer2.Pulse = (htimer2.Init.Period * 45) / 100; // 45 Duty Cycle
	   if(HAL_TIM_PWM_ConfigChannel(&htimer2,&hoctimer2,TIM_CHANNEL_2)!= HAL_OK)
	   {
		   Error_Handler();

	   }

	   hoctimer2.Pulse = (htimer2.Init.Period * 90) / 100; // 75 Duty Cycle
	   if(HAL_TIM_PWM_ConfigChannel(&htimer2,&hoctimer2,TIM_CHANNEL_3)!= HAL_OK)
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

