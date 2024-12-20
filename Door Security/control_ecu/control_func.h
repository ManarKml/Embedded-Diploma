/*
 * control_func.h
 *
 *  Created on: Nov 6, 2024
 *      Author: Manar
 */

#ifndef CONTROL_FUNC_H_
#define CONTROL_FUNC_H_

#include "buzzer.h"
#include "dc_motor.h"
#include "pir.h"
#include "external_eeprom.h"
#include "uart.h"
#include "std_types.h"
#include "twi.h"
#include "timer.h"
#include <util/delay.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PUT_PASS             1
#define CHANGE_PASS          2
#define CHECK_PASS           3

#define CONFIRM_PASS         4
#define ERROR_PASS           5

#define PASS_MISMATCH        6
#define PASS_MATCH           7

#define READY_COMM           8

#define CLOSE_DOOR           9

#define PASSWORD_LENGTH      5
#define PASSWORD_ATTEMPTS    3

#define EEPROM_PASS_ADDRESS  0x0311

#define LOCK_TIME_SECONDS    60
#define DOOR_OPEN_TIME       15
#define DOOR_CLOSE_TIME      15

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Set new password in EEPROM after confirmation
 */
uint8 Control_putPassword(void);

/*
 * Description :
 * Change password if current password is verified
 */
uint8 Control_changePassword(void);

/*
 * Description :
 * Check if entered password matches saved password
 */
uint8 Control_checkPassword(void);

/*
 * Description :
 * Controls door opening and closing using the DC motor
 */
void Control_openDoor(void);

/*
 * Description :
 * Timer callback to increment seconds
 */
void Control_timerCallback(void);

#endif /* CONTROL_FUNC_H_ */
