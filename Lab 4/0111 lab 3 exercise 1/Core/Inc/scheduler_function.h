/*
 * scheduler_function.h
 *
 *  Created on: 20 thg 11, 2022
 *      Author: ASUS
 */

#ifndef INC_SCHEDULER_FUNCTION_H_
#define INC_SCHEDULER_FUNCTION_H_

#include "global.h"
// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCH_MAX_TASKS 30
#define NO_TASK_ID 0
void testUnit();
void testRunInMain();

void SCH_Init ( void );
void SCH_Update( void ) ;
unsigned char SCH_Add_Task( void (* pFunction)() , unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks( void );
void SCH_Delete_Task( const int TASK_INDEX) ;
void SCH_Go_To_Sleep () ;
void SCH_Report_Status ( void ) ;

#endif /* INC_SCHEDULER_FUNCTION_H_ */
