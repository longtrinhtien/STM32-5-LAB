/*
 * uart_communiation_fsm.c
 *
 *  Created on: Dec 4, 2022
 *      Author: ASUS
 */

#include "uart_communiation_fsm.h"

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
uint32_t ADC_value = 0;
char str[100];
void uart_communiation_fsm()
{
	switch(uart_communiation_fsm_case)
	{
		case WAITING_FOR_RST:
			//donothing
			break;
		case RST_DONE:
			if(isTimerDone(0))
			{
				setTimer(3000, 0);
				ADC_value = HAL_ADC_GetValue (&hadc1);
				HAL_UART_Transmit(&huart2,(void*)str ,sprintf (str,"!ADC = %d# \n",ADC_value),1000);
			}
			break;
		case OK_DONE:
			uart_communiation_fsm_case = WAITING_FOR_RST;
			break;
		default:
			break;
	}
}

