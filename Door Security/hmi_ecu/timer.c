/*
 * timer.c
 *
 *  Created on: Oct 31, 2024
 *      Author: Manar
 */

#include "timer.h"
#include <avr/interrupt.h>
#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include "std_types.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static void (*g_timer0Callback)(void) = NULL_PTR;
static void (*g_timer1Callback)(void) = NULL_PTR;
static void (*g_timer2Callback)(void) = NULL_PTR;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	switch(Config_Ptr->timer_ID)
	{
	case TIMER_0:
		TCNT0 = Config_Ptr->timer_InitialValue;
		if(Config_Ptr->timer_mode == COMPARE)
		{
			TCCR0 =  (1<<FOC0) | (1<<WGM01);
			OCR0 = Config_Ptr->timer_compare_MatchValue;
			TIMSK |= (1<<OCIE0); /* Enable Timer0 Compare Interrupt */
		}
		else
		{
			TCCR0 =  (1<<FOC0);
			TIMSK |= (1<<TOIE0); /* Enable Timer0 Overflow Interrupt */
		}
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock);
		break;

	case TIMER_1:
		TCNT1 = Config_Ptr->timer_InitialValue;
		if(Config_Ptr->timer_mode == COMPARE)
		{
			TCCR1A =  (1<<FOC1A) | (1<<FOC1B) | (1<<WGM12);
			OCR1A = Config_Ptr->timer_compare_MatchValue;
			TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare Interrupt */
		}
		else
		{
			TCCR1A =  (1<<FOC1A) | (1<<FOC1B);
			TIMSK |= (1<<TOIE1); /* Enable Timer1 Overflow Interrupt */
		}
		TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_clock);
		break;

	case TIMER_2:
		TCNT2 = Config_Ptr->timer_InitialValue; /* Set initial value */
		if(Config_Ptr->timer_mode == COMPARE)
		{
			TCCR2 =  (1<<FOC2) | (1<<WGM21);
			OCR2 = Config_Ptr->timer_compare_MatchValue;
			TIMSK |= (1<<OCIE2); /* Enable Timer2 Compare Interrupt */
		}
		else
		{
			TCCR2 =  (1<<FOC1A);
			TIMSK |= (1<<TOIE2); /* Enable Timer2 Overflow Interrupt */
		}
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock);
		break;
	}
}

/*
 * Description :
 * Function to disable the Timer via Timer_ID.
 */
void Timer_deInit(Timer_ID_Type timer_ID) {
    switch (timer_ID) {
        case TIMER_0:
            TCCR0 = 0;
            TIMSK &= ~((1 << OCIE0) | (1 << TOIE0));
            g_timer0Callback = NULL_PTR;
            break;
        case TIMER_1:
            TCCR1B = 0;
            TIMSK &= ~((1 << OCIE1A) | (1 << TOIE1));
            g_timer1Callback = NULL_PTR;
            break;
        case TIMER_2:
            TCCR2 = 0;
            TIMSK &= ~((1 << OCIE2) | (1 << TOIE2));
            g_timer2Callback = NULL_PTR;
            break;
    }
}


/*
 * Description :
 * Function to set the Call Back function address to the required Timer.
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
{
	switch(a_timer_ID)
	{
	case TIMER_0:
		g_timer0Callback = a_ptr;
		break;
	case TIMER_1:
		g_timer1Callback = a_ptr;
		break;
	case TIMER_2:
		g_timer2Callback = a_ptr;
		break;
	}
}

/* Interrupt Service Routines */
ISR(TIMER0_COMP_vect)
{
	if(g_timer0Callback != NULL_PTR)
	{
		g_timer0Callback();
	}
}

ISR(TIMER0_OVF_vect)
{
	if(g_timer0Callback != NULL_PTR)
	{
		g_timer0Callback();
	}
}

ISR(TIMER1_COMPA_vect) {
    if (g_timer1Callback != NULL_PTR) {
        g_timer1Callback();
    }
}

ISR(TIMER1_OVF_vect) {
    if (g_timer1Callback != NULL_PTR) {
        g_timer1Callback();
    }
}

ISR(TIMER2_COMP_vect) {
    if (g_timer2Callback != NULL_PTR) {
        g_timer2Callback();
    }
}

ISR(TIMER2_OVF_vect) {
    if (g_timer2Callback != NULL_PTR) {
        g_timer2Callback();
    }
}


/*

static void (*g_timer0Callback)(void) = NULL;
static void (*g_timer1Callback)(void) = NULL;
static void (*g_timer2Callback)(void) = NULL;

void Timer_init(const Timer_ConfigType *Config_Ptr) {
    switch (Config_Ptr->timer_ID) {
        case TIMER0:
            // Set initial value
            TCNT0 = Config_Ptr->timer_InitialValue;

            // Set compare match value if in CTC mode
            if (Config_Ptr->timer_mode == TIMER_MODE_CTC) {
                OCR0 = Config_Ptr->timer_compare_MatchValue;
                TCCR0 = (1 << WGM01);  // Set CTC mode
            } else {
                TCCR0 &= ~(1 << WGM01);  // Set Normal mode
            }

            // Set clock prescaler
            TCCR0 |= Config_Ptr->timer_clock;

            // Enable Interrupts
            TIMSK |= (Config_Ptr->timer_mode == TIMER_MODE_CTC) ? (1 << OCIE0) : (1 << TOIE0);
            break;

        case TIMER1:
            // Set initial value
            TCNT1 = Config_Ptr->timer_InitialValue;

            // Set compare match value if in CTC mode
            if (Config_Ptr->timer_mode == TIMER_MODE_CTC) {
                OCR1A = Config_Ptr->timer_compare_MatchValue;
                TCCR1B |= (1 << WGM12);  // Set CTC mode
            } else {
                TCCR1B &= ~(1 << WGM12);  // Set Normal mode
            }

            // Set clock prescaler
            TCCR1B |= Config_Ptr->timer_clock;

            // Enable Interrupts
            TIMSK |= (Config_Ptr->timer_mode == TIMER_MODE_CTC) ? (1 << OCIE1A) : (1 << TOIE1);
            break;

        case TIMER2:
            // Set initial value
            TCNT2 = Config_Ptr->timer_InitialValue;

            // Set compare match value if in CTC mode
            if (Config_Ptr->timer_mode == TIMER_MODE_CTC) {
                OCR2 = Config_Ptr->timer_compare_MatchValue;
                TCCR2 = (1 << WGM21);  // Set CTC mode
            } else {
                TCCR2 &= ~(1 << WGM21);  // Set Normal mode
            }

            // Set clock prescaler
            TCCR2 |= Config_Ptr->timer_clock;

            // Enable Interrupts
            TIMSK |= (Config_Ptr->timer_mode == TIMER_MODE_CTC) ? (1 << OCIE2) : (1 << TOIE2);
            break;
    }
}

void Timer_deInit(Timer_ID_Type timer_ID) {
    switch (timer_ID) {
        case TIMER0:
            TCCR0 = 0;
            TIMSK &= ~((1 << OCIE0) | (1 << TOIE0));
            break;
        case TIMER1:
            TCCR1B = 0;
            TIMSK &= ~((1 << OCIE1A) | (1 << TOIE1));
            break;
        case TIMER2:
            TCCR2 = 0;
            TIMSK &= ~((1 << OCIE2) | (1 << TOIE2));
            break;
    }
}

void Timer_setCallBack(void (*callback_ptr)(void), Timer_ID_Type timer_ID) {
    switch (timer_ID) {
        case TIMER0:
            g_timer0Callback = callback_ptr;
            break;
        case TIMER1:
            g_timer1Callback = callback_ptr;
            break;
        case TIMER2:
            g_timer2Callback = callback_ptr;
            break;
    }
}

// Interrupt Service Routines for each timer

ISR(TIMER0_COMP_vect) {
    if (g_timer0Callback != NULL) {
        g_timer0Callback();
    }
}

ISR(TIMER0_OVF_vect) {
    if (g_timer0Callback != NULL) {
        g_timer0Callback();
    }
}

ISR(TIMER1_COMPA_vect) {
    if (g_timer1Callback != NULL) {
        g_timer1Callback();
    }
}

ISR(TIMER1_OVF_vect) {
    if (g_timer1Callback != NULL) {
        g_timer1Callback();
    }
}

ISR(TIMER2_COMP_vect) {
    if (g_timer2Callback != NULL) {
        g_timer2Callback();
    }
}

ISR(TIMER2_OVF_vect) {
    if (g_timer2Callback != NULL) {
        g_timer2Callback();
    }
}
*/


