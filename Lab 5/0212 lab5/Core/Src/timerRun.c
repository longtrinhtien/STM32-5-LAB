/*
 * timerforled.c
 *
 *  Created on: 8 thg 10, 2022
 *      Author: ASUS
 */


#include "timerRun.h"
#define NUM_OF_TIMER	4
#define TICK	10//10ms interrupt
int32_t count[NUM_OF_TIMER];

int flag[NUM_OF_TIMER];


void startAllTimer()
{
	for (int i=0;i<NUM_OF_TIMER;i++)
	{
		count[i]=0;
		flag[i]=0;
	}
}

void setTimer(int duration,int num_of_timer)
{
	count[num_of_timer]=duration/TICK;
}


int isTimerDone(int num_of_timer)
{
	if(flag[num_of_timer]==1)
	{
		flag[num_of_timer]=0;
		return 1;
	}
		return 0;
}



void timerRun()
{
	for(int timer=0;timer<NUM_OF_TIMER;timer++)
	{
		if(count[timer]>0)
		{
			count[timer]--;
		}
		if(count[timer]<=0)
		{
			flag[timer]=1;
		}
	}

}




