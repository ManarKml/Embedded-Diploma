/*
 * flame_sensor.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#ifndef FLAME_SENSOR_H_
#define FLAME_SENSOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Flame sensor Port Configurations */
#define FLAME_SENSOR_PORT_ID          PORTD_ID
#define FLAME_SENSOR_PIN_ID           PIN2_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initializes the flame sensor pin direction.
 */
void FlameSensor_init(void);

/*
 * Description :
 * Reads the value from the flame sensor and returns it.
 */
uint8 FlameSensor_getValue(void);

#endif /* FLAME_SENSOR_H_ */
