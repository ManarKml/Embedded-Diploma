/*
 * buzzer.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Buzzer Port Configurations */
#define BUZZER_PORT_ID             PORTC_ID
#define BUZZER_PIN_ID              PIN5_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initializes the buzzer pin direction and turn off the buzzer.
 */
void Buzzer_init(void);

/*
 * Description :
 * Activates the buzzer.
 */
void Buzzer_on(void);

/*
 * Description :
 * Deactivates the buzzer.
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
