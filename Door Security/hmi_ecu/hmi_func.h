/*
 * hmi_func.h
 *
 *  Created on: Nov 6, 2024
 *      Author: Manar
 */

#ifndef HMI_FUNC_H_
#define HMI_FUNC_H_

#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "std_types.h"
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
 * Get password input from keypad
 */
void HMI_enterPassword(uint8* password, uint8 row, uint8 col);

/*
 * Description :
 * Checks password with Control ECU
 */
uint8 HMI_checkPassword(void);

/*
 * Description :
 * Requests password change
 */
uint8 HMI_changePassword(void);

/*
 * Description :
 * Sets initial password
 */
uint8 HMI_putPassword(void);

/*
 * Description :
 * Lock system if 3 incorrect attempts
 */
void HMI_errorCheck(uint8 count);

/*
 * Description :
 * Timer callback to increment seconds
 */
void HMI_timerCallback(void);

#endif /* HMI_FUNC_H_ */
