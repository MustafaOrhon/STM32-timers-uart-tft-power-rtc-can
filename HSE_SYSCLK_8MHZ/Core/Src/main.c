/*
 * main.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */

#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdio.h"
#define TRUE  1
#define FALSE 0
char msg[100];
void UART2_Init(void);
void Error_Handler(void);
void RCC_Init(void);
UART_HandleTypeDef huart2;
RCC_OscInitTypeDef RCC_OscInitStruct;
RCC_ClkInitTypeDef RCC_ClockInitStruct;
int main(void)
{
	HAL_Init(); // It must be the first function to initialize
   RCC_Init();
   UART2_Init();

     memset(msg,0,sizeof(msg));
	 sprintf(msg,"SYSCLK: %ld Hz\r\n",HAL_RCC_GetSysClockFreq());
	 HAL_UART_Transmit(&huart2,(uint8_t *)msg,strlen(msg), 100);
	 memset(msg,0,sizeof(msg));
	 sprintf(msg,"HCLK: %ld Hz\r\n",HAL_RCC_GetHCLKFreq());
	 HAL_UART_Transmit(&huart2,(uint8_t *)msg,strlen(msg), 100);
	 memset(msg,0,sizeof(msg));
	 sprintf(msg,"PCLK1: %ld Hz \r\n",HAL_RCC_GetPCLK1Freq());
	 HAL_UART_Transmit(&huart2,(uint8_t *)msg,strlen(msg), 100);
	 memset(msg,0,sizeof(msg));
	 sprintf(msg,"PCLK2: %ld Hz\r\n",HAL_RCC_GetPCLK2Freq());
	 HAL_UART_Transmit(&huart2,(uint8_t *)msg,strlen(msg), 100);

  return 0;
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
void Error_Handler(void)
{
  while(1);
}

void RCC_Init(void)
{


	memset(&RCC_OscInitStruct,0,sizeof(RCC_OscInitStruct));
	memset(&RCC_ClockInitStruct,0,sizeof(RCC_ClockInitStruct));

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS; // We are using STLINK Microcontroller Osc.
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK)
	{
		Error_Handler();

	}
	RCC_ClockInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
			                             RCC_CLOCKTYPE_PCLK1 |RCC_CLOCKTYPE_PCLK2;
	RCC_ClockInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSE;
	RCC_ClockInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV2 ;
	RCC_ClockInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClockInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&RCC_ClockInitStruct,FLASH_ACR_LATENCY_0WS)!= HAL_OK)
	{
		Error_Handler();

	}
   __HAL_RCC_HSI_DISABLE();

   HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}






