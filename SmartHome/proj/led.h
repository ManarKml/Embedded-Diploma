/*
 * led.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#ifndef LED_H_
#define LED_H_

#include "gpio.h"
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* LEDs Port Configurations */
#define LED_BLUE_PORT_ID            PORTB_ID
#define LED_BLUE_PIN_ID              PIN5_ID

#define LED_GREEN_PORT_ID            PORTB_ID
#define LED_GREEN_PIN_ID             PIN6_ID

#define LED_RED_PORT_ID              PORTB_ID
#define LED_RED_PIN_ID               PIN7_ID

/* LEDs logic configurations */
#define LED_ON                   LOGIC_HIGH
#define LED_OFF                  LOGIC_LOW

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	LED_BLUE, LED_GREEN, LED_RED
}LED_ID;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initializes all Leds (red, green, blue) pins direction.
 * Turn off all the Leds.
 */
void LEDS_init(void);

/*
 * Description :
 * Turns on the specified LED.
 */
void LED_on(LED_ID id);

/*
 * Description :
 * Turn off all the Leds.
 */
void LED_off(LED_ID id);

#endif /* LED_H_ */
