/*
 * pir.h
 *
 *  Created on: Nov 3, 2024
 *      Author: Manar
 */

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Flame sensor Port Configurations */
#define PIR_PORT_ID          PORTC_ID
#define PIR_PIN_ID           PIN2_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the PIR driver
 */
void PIR_init(void);

/*
 * Description :
 *  Function to return PIR State
 */
uint8 PIR_getState(void);

#endif /* PIR_H_ */
