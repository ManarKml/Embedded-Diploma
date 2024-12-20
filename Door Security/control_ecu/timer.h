/*
 * timer.h
 *
 *  Created on: Oct 31, 2024
 *      Author: Manar
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum{
	TIMER_0, TIMER_1, TIMER_2
}Timer_ID_Type;

typedef enum
{
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}Timer_ClockType;

typedef enum{
	NORMAL, COMPARE
}Timer_ModeType;

typedef struct
{
	uint16 timer_InitialValue;
	uint16 timer_compare_MatchValue; /*it will be used in compare mode only*/
	Timer_ID_Type timer_ID;
	Timer_ClockType timer_clock;
	Timer_ModeType timer_mode;
}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description :
 * Function to disable the Timer via Timer_ID.
 */
void Timer_deInit(Timer_ID_Type timer_type);

/*
 * Description :
 * Function to set the Call Back function address to the required Timer.
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );

#endif /* TIMER_H_ */
