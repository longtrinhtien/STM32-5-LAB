/*
 * scheduler_function.c
 *
 *  Created on: 20 thg 11, 2022
 *      Author: ASUS
 */
#include "scheduler_function.h"
unsigned char Error_code_G = 0;// for 2.3.9
enum ErrorCode
{
	ERROR_SCH_TOO_MANY_TASKS,
	ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK,
	ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER,
	ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START,
	ERROR_SCH_LOST_SLAVE,
	ERROR_SCH_CAN_BUS_ERROR,
	ERROR_I2C_WRITE_BYTE_AT24C64,
	ERROR_SCH_CANNOT_DELETE_TASK,
	RETURN_ERROR,
	RETURN_NORMAL,
};

typedef struct
{
	// Pointer to the task (must be a ’ void ( void ) ’ function )
	void ( * pTask) ( void ) ;
	// Delay ( ticks ) until the function will ( next ) be run
	uint32_t Delay ;
	// Interval ( ticks ) between subsequent runs .
	uint32_t Period ;
	// Incremented ( by scheduler ) when task is due to execute
	uint8_t RunMe;
	// This i s a hint to solve the question below .
	uint32_t TaskID ;
} sTask ;


sTask SCH_tasks_G[SCH_MAX_TASKS];
sTask tempTask;
static int numOfTaskNow = 0;

void SCH_Init ( void )
{
unsigned char i ;
for ( i = 0; i < SCH_MAX_TASKS; i ++)
	{
		SCH_Delete_Task( i ) ;
	}
// Reset the global error variable
// − SCH_Delete_Task () w ill generate an error code ,
// ( because the task array i s empty)
//	Error_code_G = 0;
//	Timer_init();
//	Watchdog_init () ;
}

//move everything to right
void moveToRight(int startAt,int numOfTaskNow)
{
    for (int i = numOfTaskNow;i>=startAt;i--)
    {
        SCH_tasks_G[i+1]=SCH_tasks_G[i];
    }
}

//correct delay after adding
void correctDelay(sTask *taskInput, int numOfTaskNow)
{
    // nothing to compare
    if(SCH_tasks_G[0].pTask==0)
    {
        SCH_tasks_G[0]=(*taskInput);
        return;
    }

    //less than first element
    if((*taskInput).Delay<=SCH_tasks_G[0].Delay)
    {
             tempTask=*taskInput;

        //delete task to moveToRight
        	(*taskInput).pTask = 0x0000 ;
        	(*taskInput).Delay = 0;
        	(*taskInput).Period = 0;
        	(*taskInput).RunMe = 0;
        moveToRight(0,numOfTaskNow);
        SCH_tasks_G[0]=tempTask;
        if(SCH_tasks_G[1].pTask!=0)
        {
            SCH_tasks_G[1].Delay =SCH_tasks_G[1].Delay- SCH_tasks_G[0].Delay;
        }
        return;
    }

    //more than everything
    int sumEverything = 0;
    for(int i=0;i<numOfTaskNow;i++)
    {
        if(SCH_tasks_G[i].pTask)
        {
            sumEverything = sumEverything + SCH_tasks_G[i].Delay;
        }
    }
    if((*taskInput).Delay>=sumEverything)
    {
        SCH_tasks_G[numOfTaskNow].Delay -= sumEverything;
        return;
    }

    //new Delay must be between sum1 and sum2
    int sum1=0;
    int sum2=0;
    for (int i=0; i<numOfTaskNow;i++)
    {
        //evaluate sum1 and sum2
        if(i>0)
        {
            sum1 = sum1 + SCH_tasks_G[i-1].Delay;
        } else {
            sum1 = 0;
        }
        sum2 = sum2 + SCH_tasks_G[i].Delay;
        if(SCH_tasks_G[i].pTask)
        {
        if(((*taskInput).Delay >= sum1)&&((*taskInput).Delay <= sum2))
            {
                tempTask=*taskInput;
                //delete task to moveToRight
                	(*taskInput).pTask = 0x0000 ;
                	(*taskInput).Delay = 0;
                	(*taskInput).Period = 0;
                	(*taskInput).RunMe = 0;
                moveToRight(i,numOfTaskNow);
                SCH_tasks_G[i]=tempTask;
                SCH_tasks_G[i].Delay = SCH_tasks_G[i].Delay - sum1;
                if(SCH_tasks_G[i+1].pTask)
                {
                    SCH_tasks_G[i+1].Delay -= SCH_tasks_G[i].Delay;
                }
	return;
            }
        }
    }
}

void SCH_Update( void )
{
	if((SCH_tasks_G[0].Delay>0)&&(SCH_tasks_G[0].RunMe == 0))
	{
		SCH_tasks_G[0].Delay--;
	}

	if(SCH_tasks_G[0].Delay==0)
	{
		SCH_tasks_G[0].RunMe = 1;
		//delete task and set run me = 0 in another function
	}

}

void swap(sTask *a, sTask *b)
{
	sTask temp = *a;
	*a = *b;
	*b = temp;
}




/*−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−*−
SCH_Add_Task () Causes a task ( function ) to be executed at regular inter val s
or aft e r a user−defined delay
−*−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−*/
unsigned char SCH_Add_Task( void (* pFunction)() , unsigned int DELAY, unsigned int PERIOD)
{
	unsigned char Index = 0;
	// First find a gap in the array ( if there is one)
	while (( SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
	{
		Index++;
	}
	// let we know numOfTaskNow
	numOfTaskNow++;
	// If we're here , there is a space in the task array
	SCH_tasks_G[Index].pTask = pFunction ;
	SCH_tasks_G[Index].Delay = DELAY;
	SCH_tasks_G[Index].Period = PERIOD;
	SCH_tasks_G[Index].RunMe = 0;

	//correctDelay here
	correctDelay((&SCH_tasks_G[Index]),numOfTaskNow);
	// return position of task ( to allow later deletion )
	return Index ;
}


void SCH_Dispatch_Tasks(void)
{
	unsigned char Index ;
	// Dispatches ( runs ) the next task ( if one is ready )
//	for ( Index = 0; Index < 1; Index++) {
		if (SCH_tasks_G[0].RunMe > 0) {
			(* SCH_tasks_G[0].pTask) () ; // Run the task
			SCH_tasks_G[0].RunMe = 0; // Reset / reduce RunMe flag
			// Periodic tasks w ill automatically run again
			// − if this is a ’one shot ’ task , remove it from the array
			if ( SCH_tasks_G[0].Period == 0)
			{
				SCH_Delete_Task(0);
			}
			if((SCH_tasks_G[0].Period>0)&&(SCH_tasks_G[0].Delay==0))
    		{
                tempTask=SCH_tasks_G[0];
                SCH_Delete_Task(0);
                SCH_Add_Task(tempTask.pTask,tempTask.Period,tempTask.Period);
    		}

		}
//	}
}


void SCH_Delete_Task( const int TASK_INDEX) //tByte here
{
    for (int i = TASK_INDEX;i<(numOfTaskNow);i++)
    {
        SCH_tasks_G[i]=SCH_tasks_G[i+1];
    }
    	SCH_tasks_G[numOfTaskNow].pTask = 0x0000 ;
    	SCH_tasks_G[numOfTaskNow].Delay = 0;
    	SCH_tasks_G[numOfTaskNow].Period = 0;
    	SCH_tasks_G[numOfTaskNow].RunMe = 0;
    	numOfTaskNow--;
}



void SCH_Go_To_Sleep ()
{
	//todo : Optional
}

void SCH_Report_Status ( void )
{
#ifdef SCH_REPORT_ERRORS //if SCH_REPORT_ERRORS is defined already, then redefine it
	// ONLY APPLIES IF WE ARE REPORTING ERRORS
	// Check for a new error code
	if ( Error_code_G != Last_error_code_G ) {
		// Negative logic on LEDs assumed
		Error_port = 255 − Error_code_G ;
		Last_error_code_G = Error_code_G ;
		if ( Error_code_G != 0) {
			Error_tick_count_G = 60000;
		} else {
			Error_tick_count_G = 0;
		}
	} else {
		if ( Error_tick_count_G != 0) {
			if (−−Error_tick_count_G == 0) {
				Error_code_G = 0; // Reset error code
			}
		}
	}
#endif
}
