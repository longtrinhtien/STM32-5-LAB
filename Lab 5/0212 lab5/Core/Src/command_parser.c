/*
 * command_parser.c
 *
 *  Created on: Dec 2, 2022
 *      Author: ASUS
 */

#include "command_parser.h"


void command_parser_fsm()
{
	switch(command_praser_fsm_case)
	{
		case WAITING_FOR_CHAMTHANG:
			if(temp == '!')
			{
				command_praser_fsm_case = WAITING_FOR_R;
			}
			break;
		case WAITING_FOR_R:
			if(temp == 'R')
			{
				command_praser_fsm_case = WAITING_FOR_S;
			}
			else
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG;
			}
			break;
		case WAITING_FOR_S:
			if(temp == 'S')
			{
				command_praser_fsm_case = WAITING_FOR_T;
			}
			else
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG;
			}
			break;
		case WAITING_FOR_T:
			if(temp == 'T')
			{
				command_praser_fsm_case = WAITING_FOR_THANG;
			}
			else
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG;
			}
			break;
		case WAITING_FOR_THANG:
			if(temp == '#')
			{
				uart_communiation_fsm_case = RST_DONE;
				command_praser_fsm_case = WAITING_FOR_O;
				setTimer(3000, 0);
			}
			else
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG;
			}
			break;
		//ok check
		case WAITING_FOR_CHAMTHANG2:
			if(temp == '!')
			{
				command_praser_fsm_case = WAITING_FOR_O;
			}
			break;
		case WAITING_FOR_O:
			if(temp == 'O')
			{
				command_praser_fsm_case = WAITING_FOR_K;
			}
			else
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG2;
			}
			break;
		case WAITING_FOR_K:
			if(temp == 'K')
			{
				command_praser_fsm_case = WAITING_FOR_THANG2;
			}
			else
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG2;
			}
			break;
		case WAITING_FOR_THANG2:
			if(temp == '#')
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG;
				uart_communiation_fsm_case = OK_DONE;
			}
			else
			{
				command_praser_fsm_case = WAITING_FOR_CHAMTHANG2;
			}
			break;
		default:
			break;
	}
}


