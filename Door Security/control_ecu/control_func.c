/*
 * control_func.c
 *
 *  Created on: Nov 6, 2024
 *      Author: Manar
 */

#include "control_func.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the number of incorrect password attempts */
uint8 g_counter = 0;

/* Global variables to hold the counted seconds by the timer */
volatile uint8 g_seconds = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Set new password in EEPROM after confirmation
 */
uint8 Control_putPassword(void)
{
	uint8 flag = 0, password[PASSWORD_LENGTH + 1] = {0}, password2[PASSWORD_LENGTH + 1] = {0};

	UART_receiveString(password);
	_delay_ms(50);

	UART_receiveString(password2);
	_delay_ms(50);

	flag = PASS_MATCH; /* Assume passwords match initially */

	for(uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		if(password[i] != password2[i])
		{
			UART_sendByte(ERROR_PASS); /* Notify HMI of mismatch */
			flag = PASS_MISMATCH; /* Set flag for mismatch */
			g_counter++; /* Increment failed attempt counter */
			break;
		}
	}

	if(flag == PASS_MATCH) /* Success: save password to EEPROM */
	{
		for(uint8 i = 0; i < PASSWORD_LENGTH; i++)
		{
			EEPROM_writeByte(EEPROM_PASS_ADDRESS + i, password[i]);
			_delay_ms(10);
		}
		UART_sendByte(CONFIRM_PASS); /* Notify HMI of success */
		g_counter = 0;
	}
	return flag;
}

/*
 * Description :
 * Change password after verifying the current password
 */
uint8 Control_changePassword(void)
{
	uint8 flag = 0;

	flag = Control_checkPassword(); /* Verify current password */

	if(flag == CONFIRM_PASS)
	{
		flag = UART_recieveByte();
		flag = Control_putPassword(); /* Set new password if verified */
	}
	return flag;
}

/*
 * Description :
 * Check if entered password matches saved password
 */
uint8 Control_checkPassword(void)
{
	uint8 flag = 0, password[PASSWORD_LENGTH + 1] = {0}, password2[PASSWORD_LENGTH + 1] = {0};

	UART_receiveString(password); /* Get entered password from HMI */

	/* Compare entered password with stored password in EEPROM */
	for(uint8 i = 0; i < PASSWORD_LENGTH; i++)
	{
		flag = PASS_MATCH; /* Assume passwords match initially */

		if(EEPROM_readByte(EEPROM_PASS_ADDRESS + i, &password2[i]) != SUCCESS || password[i] != password2[i])
		{
			UART_sendByte(ERROR_PASS); /* Send mismatch signal to HMI */
			flag = ERROR_PASS;
			g_counter++; /* Increment failed attempt counter */
			break;
		}
	}

	if(flag == PASS_MATCH)
	{
		UART_sendByte(CONFIRM_PASS); /* Confirm successful match */
		g_counter = 0; /* Reset failed attempt counter */
	}

	/*------ System Lock after 3 failed attempts ------*/
	if(g_counter == PASSWORD_ATTEMPTS)
	{
		Buzzer_on(); /* Turn on buzzer to signal lock */

		/* Lock system for 60 seconds */
		g_seconds = 0;
		while(g_seconds < LOCK_TIME_SECONDS);

		Buzzer_off(); /* Turn off buzzer after lock period */

		g_counter = 0; /* Reset failed attempt counter */
	}

	return flag;
}

/*
 * Description :
 * Controls door opening and closing using the DC motor
 */
void Control_openDoor(void)
{
	g_seconds = 0;
	DcMotor_Rotate(CW, 100); /* Rotate motor to open door */

	while(g_seconds < DOOR_OPEN_TIME); /* Wait 15 seconds for door to open */

	DcMotor_Rotate(STOP, 0); /* Stop motor after door is open */

	while(PIR_getState()); /* Wait until no motion detected */

	UART_sendByte(CLOSE_DOOR); /* Notify HMI to close door */

	g_seconds = 0;
	DcMotor_Rotate(ACW, 100); /* Rotate motor to close door */

	while(g_seconds < DOOR_CLOSE_TIME); /* Wait 15 seconds for door to close */

	DcMotor_Rotate(STOP, 0); /* Stop motor */
}

/*
 * Description :
 * Timer callback to increment seconds
 */
void Control_timerCallback(void)
{
	g_seconds++;
}
