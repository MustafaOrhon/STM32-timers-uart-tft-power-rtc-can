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
void TIMER6_Init(void);
void SystemClockConfig(void);
void CAN1_Init(void);
void CAN1_TX(void);
void CAN1_RX(void);
void CAN1_Filter_Config(void);
void LED_MANAGE_Output(uint8_t data);
void Send_Response(uint32_t StdId);
UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;
TIM_HandleTypeDef htimer6;
char buff[100];
char rx_buffer[100];
uint8_t req_counter = 0;
int main(void)
{

	HAL_Init();

	SystemClockConfig();

	GPIO_Init();

	UART2_Init();

	TIMER6_Init();

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



	while(1);

	return 0;
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	                                              // USING LED PC6-PC8-PC-9-PB8
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;   // USING USER BUTTER AS INTERRUPT START
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI0_IRQn,15, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

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
void TIMER6_Init(void)
{

	htimer6.Instance = TIM6;
	htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer6.Init.Prescaler = 4199;
	htimer6.Init.Period = 9999;

	if(HAL_TIM_Base_Init(&htimer6)!= HAL_OK)
	{
		Error_Handler();

	}



}
void CAN1_Init(void)
{
     hcan1.Instance = CAN1;
     hcan1.Init.Mode = CAN_MODE_NORMAL;
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
uint8_t led_no =0;
void CAN1_TX(void)
{
   CAN_TxHeaderTypeDef htxcan1;
   uint32_t TxMailBox;

   uint8_t message;
   htxcan1.DLC = 1;
   htxcan1.StdId = 0x65D;
   htxcan1.IDE = CAN_ID_STD;
   htxcan1.RTR = CAN_RTR_DATA;

   message = ++led_no;
   if(led_no == 4)
   {
	   led_no = 0;
   }

   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

   if(HAL_CAN_AddTxMessage(&hcan1, &htxcan1, &message, &TxMailBox)!= HAL_OK)
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
    CAN_FilterInitStruct.FilterMaskIdHigh = 0x01C0;
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
	uint8_t rx_buff[8];
    char msg[50];


	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0,&hrxcan1,rx_buff)!= HAL_OK)
	{
		Error_Handler();
	}

	if(hrxcan1.StdId == 0x65D && hrxcan1.RTR == 0)
	{
		//This is data frame to send N1 to N2
		LED_MANAGE_Output(rx_buff[0]);

	}
	else if(hrxcan1.StdId == 0x651 && hrxcan1.RTR == 1)
	{
		// This is a remote frame sent by n1 to n2
		Send_Response(hrxcan1.StdId);
		return;

	}
	else if(hrxcan1.StdId == 0x651 && hrxcan1.RTR == 0)
	{
		//  Its a reply (data frame) n2 to n1
		sprintf(msg,"Reply Recieved : %#X\r\n",rx_buff[0] << 8 | rx_buff[1]);

	}
	 if(HAL_UART_Transmit(&huart2, (uint8_t *)msg , strlen(msg), HAL_MAX_DELAY))
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	     CAN_TxHeaderTypeDef TxHeader;

		 uint32_t TxMailbox;

		 uint8_t message; //no meaning for data frame

		if ( req_counter  == 4)
		{
			//N1 sending Remote frame to N2
			TxHeader.DLC = 2; //N1 demanding 2 bytes of reply
			TxHeader.StdId = 0x651;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;

			if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader,&message,&TxMailbox) != HAL_OK)
			{
				Error_Handler();
			}
			req_counter = 0;

		}else
		{
			CAN1_TX();
			req_counter++;
		}

}
void LED_MANAGE_Output(uint8_t data)
{
	switch(data)
	{
	case 1:
		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6,GPIO_PIN_SET);
		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET);
	case 2:
	        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8,GPIO_PIN_SET);
		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_9,GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET);
	case 3:
	        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9,GPIO_PIN_SET);
		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_8,GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET);
	case 4:

		    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6| GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET);

	}


}

void Send_Response(uint32_t StdId)
{
	   CAN_TxHeaderTypeDef htxcan1;
	   uint32_t TxMailBox;

	   uint8_t response[2] = {0xAB,0xCD};
	   htxcan1.DLC = 2;
	   htxcan1.StdId = StdId;
	   htxcan1.IDE = CAN_ID_STD;
	   htxcan1.RTR = CAN_RTR_DATA;


	   if(HAL_CAN_AddTxMessage(&hcan1, &htxcan1, response, &TxMailBox)!= HAL_OK)
	   {
		   Error_Handler();
	   }


}
