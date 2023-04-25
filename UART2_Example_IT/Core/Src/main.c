/*
 * main.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */

#include "stm32f4xx_hal.h"
#include "string.h"
#define TRUE  1
#define FALSE 0
void SystemClockConfig(void);
void UART2_Init(void);
void Error_Handler(void);

UART_HandleTypeDef huart2;
char *user_data = "Hello\r\n";
uint8_t rx_data;
uint8_t data_buffer[100];
uint32_t count=0;
uint8_t reception_complete = FALSE;
int main(void)
{
	HAL_Init(); // It must be the first function to initialize
	SystemClockConfig();
	UART2_Init();
    uint16_t length = strlen(user_data);
	if(HAL_UART_Transmit(&huart2,(uint8_t *)user_data,length,HAL_MAX_DELAY) != HAL_OK)
		{
		 Error_Handler();
		}
	while(reception_complete != TRUE)
	HAL_UART_Receive_IT(&huart2, &rx_data, 1);



  while(1);

  return 0;
}


void SystemClockConfig(void)
{

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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rx_data == '\r')
	{
		reception_complete= TRUE;
		data_buffer[count++] = '\r';
		HAL_UART_Transmit(huart,data_buffer,count,HAL_MAX_DELAY);

	}
	else{
	     data_buffer[count++] = rx_data;
	    }

}
