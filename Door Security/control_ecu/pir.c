/*
 * pir.c
 *
 *  Created on: Nov 3, 2024
 *      Author: Manar
 */

#include "gpio.h"
#include "pir.h"

/*
 * Description :
 * Function to initialize the PIR driver
 */
void PIR_init(void)
{
	GPIO_setupPinDirection(PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);
}

/*
 * Description :
 *  Function to return PIR State
 */
uint8 PIR_getState(void)
{
	return GPIO_readPin(PIR_PORT_ID, PIR_PIN_ID);
}
