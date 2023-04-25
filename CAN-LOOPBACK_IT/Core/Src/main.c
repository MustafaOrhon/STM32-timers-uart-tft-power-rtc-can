/*
 * main.c
 *
 *  Created on: Apr 2, 2023
 *      Author: Mustafa
 */

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdio.h>
void GPIO_Init(void);
void Error_Handler(void);
void UART2_Init(void);
void SystemClockConfig(void);
void CAN1_Init(void);
void CAN1_TX(void);
void CAN1_RX(void);
void CAN1_Filter_Config(void);
UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;
char buff[100];
char rx_buffer[100];

int main(void)
{

	HAL_Init();

	SystemClockConfig();

	GPIO_Init();

	UART2_Init();

    CAN1_Init();

    CAN1_Filter_Config();

    if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY
    		                               |CAN_IT_RX_FIFO0_MSG_PENDING |CAN_IT_BUSOFF) !=HAL_OK)
    {
    	Error_Handler();

    }


    if(HAL_CAN_Start(&hcan1)!= HAL_OK) // Move Can from Init -> Normal Mode
    {
    	Error_Handler();
    }




	while(1)
	{
		CAN1_TX();
	}

	return 0;
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

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClockConfig(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClockInitStruct;

	memset(&RCC_OscInitStruct,0,sizeof(RCC_OscInitStruct));
	memset(&RCC_ClockInitStruct,0,sizeof(RCC_ClockInitStruct));

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
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


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_Handler();
	}


}
void CAN1_Init(void)
{
     hcan1.Instance = CAN1;
     hcan1.Init.Mode = CAN_MODE_LOOPBACK;
     hcan1.Init.AutoBusOff = DISABLE;
     hcan1.Init.AutoRetransmission = ENABLE;
     hcan1.Init.AutoWakeUp = DISABLE;
     hcan1.Init.ReceiveFifoLocked = DISABLE;
     hcan1.Init.TimeTriggeredMode = DISABLE;
     hcan1.Init.TransmitFifoPriority = DISABLE;

     // Time-Settings  125 kbps bitrate
     hcan1.Init.Prescaler = 21;
     hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
     hcan1.Init.TimeSeg1 = CAN_BS1_13TQ	;
     hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;

     if(HAL_CAN_Init(&hcan1)!= HAL_OK)
    	 {
    	   Error_Handler();

    	 }

}
void CAN1_TX(void)
{
   CAN_TxHeaderTypeDef htxcan1;
   uint32_t TxMailBox;

   char msg[7] = "MUSTAFA";
   htxcan1.DLC = 7;
   htxcan1.StdId = 0x65D;
   htxcan1.IDE = CAN_ID_STD;
   htxcan1.RTR = CAN_RTR_DATA;

   if(HAL_CAN_AddTxMessage(&hcan1, &htxcan1, (uint8_t *)msg, &TxMailBox)!= HAL_OK)
   {
	   Error_Handler();
   }


}


void CAN1_Filter_Config()
{

    CAN_FilterTypeDef CAN_FilterInitStruct;

    CAN_FilterInitStruct.FilterActivation = ENABLE;
    CAN_FilterInitStruct.FilterBank = 0;
    CAN_FilterInitStruct.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN_FilterInitStruct.FilterIdHigh = 0x0000;
    CAN_FilterInitStruct.FilterIdLow = 0x0000;
    CAN_FilterInitStruct.FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStruct.FilterMaskIdLow = 0x0000;
    CAN_FilterInitStruct.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterInitStruct.FilterScale = CAN_FILTERSCALE_32BIT;

    if(HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStruct)!= HAL_OK)
  	     {
  	       	   Error_Handler();
  	     }

}
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	   sprintf(buff,"Message transmitted : M0\r\n");
	   if(HAL_UART_Transmit(&huart2, (uint8_t *)buff , strlen(buff), HAL_MAX_DELAY))
	       	 {
	       	   Error_Handler();
	     	 }

}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	   sprintf(buff,"Message transmitted M1\r\n");
	   if(HAL_UART_Transmit(&huart2, (uint8_t *)buff , strlen(buff), HAL_MAX_DELAY))
	       	 {
	       	   Error_Handler();
	     	 }
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	   sprintf(buff,"Message transmitted M1\r\n");
	   if(HAL_UART_Transmit(&huart2, (uint8_t *)buff , strlen(buff), HAL_MAX_DELAY))
	       	 {
	       	   Error_Handler();
	     	 }
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef hrxcan1;
	uint8_t rx_buff[7];



	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0,&hrxcan1,rx_buff)!= HAL_OK)
	{
		Error_Handler();
	}

	  sprintf(rx_buffer,"Message Recieved : %s\r\n",rx_buff);
	  if(HAL_UART_Transmit(&huart2, (uint8_t *)rx_buffer , strlen(rx_buffer), HAL_MAX_DELAY))
	       	 {
	       	   Error_Handler();
	     	 }

}
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	   char error[50];
	   sprintf(error,"Error Detected\r\n");
	   if(HAL_UART_Transmit(&huart2, (uint8_t *)error , strlen(error), HAL_MAX_DELAY))
	       	 {
	       	   Error_Handler();
	     	 }
}
