/*
 * main.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mustafa
 */

#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdio.h"
#include <stdarg.h>
#include "main.h"
void UART2_Init(void);
void Error_Handler(void);
void RCC_Init(void);
void GPIO_Init(void);
void RTC_Init(void);
void RTC_CalendarConfig(void);
void RTC_AlarmConfig();
UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

void printmsg(char *format,...)
 {

	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);

 }
int main(void)
{
	HAL_Init(); // It must be the first function to initialize

	GPIO_Init();

	UART2_Init();

	printmsg("RTC TESTING\r\n");
    RTC_Init();



    while(1);


  return 0;
}

void GPIO_Init(void)
{
     __HAL_RCC_GPIOA_CLK_ENABLE();
     __HAL_RCC_GPIOC_CLK_ENABLE();

     GPIO_InitTypeDef GPIO_InitStruct;

     GPIO_InitStruct.Pin = GPIO_PIN_5;
     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;

     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 	GPIO_InitStruct.Pin = GPIO_PIN_13;
 	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;   // USING USER BUTTER AS INTERRUPT START
 	GPIO_InitStruct.Pull = GPIO_NOPULL;
 	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

 	HAL_NVIC_SetPriority(EXTI15_10_IRQn,14, 0);
 	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);




}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	RTC_TimeTypeDef rTime;
	RTC_DateTypeDef rDate;

    RTC_CalendarConfig();

	HAL_RTC_GetTime(&hrtc,&rTime,RTC_FORMAT_BIN);

	HAL_RTC_GetDate(&hrtc, &rDate, RTC_FORMAT_BIN);

	printmsg("Current time is: %02d:%02d:%02d\r\n",rTime.Hours,rTime.Minutes,rTime.Seconds);

	printmsg("Current Date is: %02d-%2d-%2d\r\n",rDate.Month,rDate.Date,rDate.Year);

	RTC_AlarmConfig();


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


void RTC_Init(void)
{
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
  hrtc.Init.AsynchPrediv = 0x7F;
  hrtc.Init.SynchPrediv = 0xFF;  // SEND 255 TO MAKE IT 256
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    if( HAL_RTC_Init(&hrtc) != HAL_OK)
    {
    	Error_Handler();

    }

}
void RTC_CalendarConfig(void)  // Configures the calendar as 12:11:10 PM : 23 April 2023 Sunday
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    sTime.Hours =   12;
    sTime.Minutes = 11;
    sTime.Seconds = 10;
    sTime.TimeFormat = RTC_HOURFORMAT12_PM;

    if( HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
       {
       	Error_Handler();

       }


	sDate.Date = 23;
	sDate.Month = RTC_MONTH_APRIL;
	sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	sDate.Year = 23;

	 if( HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	   {
	     Error_Handler();

	   }

}
void RTC_AlarmConfig()  // xx:30:09
{
	   RTC_AlarmTypeDef sAlarm;

	   memset(&sAlarm,0,sizeof(sAlarm));

	   HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_B);

	   sAlarm.Alarm = RTC_ALARM_B;
	   sAlarm.AlarmTime.Hours = 12;
	   sAlarm.AlarmTime.Minutes = 0;
	   sAlarm.AlarmTime.Seconds = 9;
	   sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
	   sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY ;
	   sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	   if(HAL_RTC_SetAlarm_IT(&hrtc,&sAlarm,RTC_FORMAT_BIN)!= HAL_OK)
	   {
		    Error_Handler();

	   }
	   printmsg("ALARM SUCCESFULLY CONFIGURED\r\n");

}
void HAL_RTC_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
	printmsg("ALARM TRIGGERED\r\n");
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_SET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);


}



