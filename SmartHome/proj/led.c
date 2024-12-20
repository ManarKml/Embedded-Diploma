/*
 * led.c
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#include "std_types.h"
#include <avr/io.h>
#include "gpio.h"
#include "led.h"

void LEDS_init(void)
{
	GPIO_setupPinDirection(LED_BLUE_PORT_ID, LED_BLUE_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_GREEN_PORT_ID, LED_GREEN_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_RED_PORT_ID, LED_RED_PIN_ID, PIN_OUTPUT);

	GPIO_writePin(LED_BLUE_PORT_ID, LED_BLUE_PIN_ID, LOGIC_LOW);
	GPIO_writePin(LED_GREEN_PORT_ID, LED_GREEN_PIN_ID, LOGIC_LOW);
	GPIO_writePin(LED_RED_PORT_ID, LED_RED_PIN_ID, LOGIC_LOW);
}

void LED_on(LED_ID id)
{
	switch(id)
	{
	case LED_BLUE: GPIO_writePin(LED_BLUE_PORT_ID, LED_BLUE_PIN_ID, LOGIC_HIGH); break;
	case LED_GREEN: GPIO_writePin(LED_GREEN_PORT_ID, LED_GREEN_PIN_ID, LOGIC_HIGH); break;
	case LED_RED: GPIO_writePin(LED_RED_PORT_ID, LED_RED_PIN_ID, LOGIC_HIGH); break;
	}
}

void LED_off(LED_ID id)
{
	switch(id)
	{
	case LED_BLUE: GPIO_writePin(LED_BLUE_PORT_ID, LED_BLUE_PIN_ID, LOGIC_LOW); break;
	case LED_GREEN: GPIO_writePin(LED_GREEN_PORT_ID, LED_GREEN_PIN_ID, LOGIC_LOW); break;
	case LED_RED: GPIO_writePin(LED_RED_PORT_ID, LED_RED_PIN_ID, LOGIC_LOW); break;
	}
}

