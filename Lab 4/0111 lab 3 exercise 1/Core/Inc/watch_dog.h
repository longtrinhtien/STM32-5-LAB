/*
 * watch_dog.h
 *
 *  Created on: 20 thg 11, 2022
 *      Author: ASUS
 */

#ifndef INC_WATCH_DOG_H_
#define INC_WATCH_DOG_H_

#include "global.h"
void MX_IWDG_Init( void );
void Watchdog_Refresh( void );
unsigned char Is_Watchdog_Reset ( void );
void Watchdog_Counting( void );
void Reset_Watchdog_Counting( void );



#endif /* INC_WATCH_DOG_H_ */
