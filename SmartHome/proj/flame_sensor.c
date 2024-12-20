/*
 * flame_sensor.c
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#include "gpio.h"
#include "flame_sensor.h"

/*
 * Description :
 * Initializes the flame sensor pin direction.
 */
void FlameSensor_init(void)
{
	GPIO_setupPinDirection(FLAME_SENSOR_PORT_ID, FLAME_SENSOR_PIN_ID, PIN_INPUT);
}

/*
 * Description :
 * Reads the value from the flame sensor and returns it.
 */
uint8 FlameSensor_getValue(void)
{
	return GPIO_readPin(FLAME_SENSOR_PORT_ID, FLAME_SENSOR_PIN_ID);
}

