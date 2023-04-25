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
void LSE_Config(void);
TIM_HandleTypeDef htimer2;
TIM_IC_InitTypeDef hictimer2;
uint32_t input_captures[2] = {0};
uint32_t capture_difference=0;
uint8_t count=1;
double timer2_cnt_freq=0;
double timer2_cnt_res=0;
double user_signal_time_period=0,user_signal_time_freq=0;
uint8_t is_capture_finished = 0;
char user_msg [100];
int main(void)
{
	HAL_Init(); // It must be the first function to initialize
	SystemClockConfig();
	GPIO_Init();
	LSE_Config();
	TIMER2_Init();

	HAL_TIM_IC_Start_IT(&htimer2,TIM_CHANNEL_1);

	while(1)
	{
		if(is_capture_finished)
		{
			if(input_captures[1] > input_captures[0])
			{
			capture_difference = input_captures[1] - input_captures[0];
			}
			else
			{
				capture_difference = (0xFFFFFFFF - input_captures[0]) + input_captures[1];
			}
			timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2) / (htimer2.Init.Prescaler+1);
			timer2_cnt_res = 1/timer2_cnt_freq;
	        user_signal_time_period = capture_difference * timer2_cnt_res;
	        user_signal_time_freq = 1/user_signal_time_period;
		}
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
void TIMER2_Init(void)
{

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler = 1;
	htimer2.Init.Period = 0xFFFFFFFF;

	if( HAL_TIM_IC_Init(&htimer2)!=HAL_OK)
		{
		     Error_Handler();
		}

   hictimer2.ICPolarity =  TIM_ICPOLARITY_RISING;
   hictimer2.ICFilter = 0;
   hictimer2.ICPrescaler = TIM_ICPSC_DIV1 ;
   hictimer2.ICSelection = TIM_ICSELECTION_DIRECTTI;

   if( HAL_TIM_IC_ConfigChannel(&htimer2, &hictimer2,TIM_CHANNEL_1)!=HAL_OK)
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
void LSE_Config(void)
{
#if 0
	RCC_OscInitTypeDef RCC_OscInitStruct =  {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;

	if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK)
	{
		Error_Handler();
	}

#endif
   HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);



}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
 if(!is_capture_finished){
	if(count == 1)
     {
    	 input_captures[0] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1);
    	 count++;
     }
     else if(count == 2)
     {
    	 input_captures[1] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1);
    	 count=0;
    	 is_capture_finished = 1;
     }
 }
}
