/*
 * hmi_func.c
 *
 *  Created on: Nov 6, 2024
 *      Author: Manar
 */

#include "hmi_func.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the counted seconds by the timer */
volatile uint8 g_seconds = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Get password input from keypad
 */
void HMI_enterPassword(uint8* password, uint8 row, uint8 col)
{
	LCD_moveCursor(row, col);

    for(uint8 i = 0; i < PASSWORD_LENGTH; i++)
    {
        uint8 key = KEYPAD_getPressedKey();
        _delay_ms(200); /* Debounce delay */
        if(key >= 0 && key <= 9)
        {
            password[i] = key + '0'; /* Store key as ASCII digit */
            LCD_displayString("*"); /* Mask password display */
        }
    }
    password[5] = '\0'; /* Null-terminate the string */
    while(KEYPAD_getPressedKey() != '='); /* Wait for Enter key */
}

/*
 * Description :
 * Sets initial password
 */
uint8 HMI_putPassword(void)
{
	uint8 flag, password[PASSWORD_LENGTH + 1] = {0};

	do{
		UART_sendByte(PUT_PASS); /* Send password put request */

		LCD_clearScreen();
		LCD_displayString("plz enter pass:");

		HMI_enterPassword(password, 1, 0);
		UART_sendString(password); /* Send entered password to Control ECU */
		_delay_ms(50);

		LCD_clearScreen();
		LCD_displayString("plz re-enter the");
		LCD_moveCursor(1, 0);
		LCD_displayString("same pass:");

		HMI_enterPassword(password, 1, 10);
		UART_sendString(password); /* Send second password to Control ECU */
		_delay_ms(50);

		flag = UART_recieveByte(); /* Receive confirmation or error */
		_delay_ms(50);

	}while(flag == ERROR_PASS); /* Retry if passwords do not match */

	return flag;
}

/*
 * Description :
 * Checks password with Control ECU
 */
uint8 HMI_checkPassword(void)
{
	uint8 flag, password[PASSWORD_LENGTH + 1] = {0};

	uint8 i = 0;
	for(i = 0; i < PASSWORD_ATTEMPTS; i++) /* Allow up to 3 attempts */
	{
		UART_sendByte(CHECK_PASS); /* Send password check request */

		LCD_clearScreen();
		LCD_displayString("plz enter pass:");
		HMI_enterPassword(password, 1, 0);

		UART_sendString(password); /* Send entered password to Control ECU */

		flag = UART_recieveByte(); /* Receive confirmation or error */

		if(flag == CONFIRM_PASS)
		{
			LCD_clearScreen();
			LCD_displayString("Door unlocking");
			LCD_moveCursor(1, 3);
			LCD_displayString("Please wait");

			/* Wait 15 seconds for door to close */
			g_seconds = 0;
			while(g_seconds < DOOR_OPEN_TIME);

			LCD_clearScreen();
			LCD_displayString("Wait for people");
			LCD_moveCursor(1, 4);
			LCD_displayString("to enter");

			flag = UART_recieveByte(); /* Wait for confirmation to close door */

			LCD_clearScreen();
			LCD_displayString("Door locking");
			LCD_moveCursor(1, 3);
			LCD_displayString("Please wait");

			/* Wait 15 seconds for door to close */
			g_seconds = 0;
			while(g_seconds < DOOR_CLOSE_TIME);

			break;
		}
	}

	/*------ Lock system if 3 incorrect attempts ------*/
	HMI_errorCheck(i);

	return flag;
}

/*
 * Description :
 * Requests password change
 */
uint8 HMI_changePassword(void)
{
	uint8 flag, password[PASSWORD_LENGTH + 1] = {0};

	uint8 i = 0;
	for(i = 0; i < PASSWORD_ATTEMPTS; i++) /* Allow up to 3 attempts */
	{
		UART_sendByte(CHANGE_PASS); /* Send password change request */

		LCD_clearScreen();
		LCD_displayString("plz enter old");
		LCD_moveCursor(1, 0);
		LCD_displayString("pass:");

		HMI_enterPassword(password, 1, 5);
		UART_sendString(password); /* Send entered password to Control ECU */
		_delay_ms(50);

		flag = UART_recieveByte();
		_delay_ms(50);

		if(flag == CONFIRM_PASS) /* Allow password change if entered password is matched */
		{
			flag = HMI_putPassword();
			break;
		}
	}

	/*------ Lock system if 3 incorrect attempts ------*/
	HMI_errorCheck(i);

	return flag;
}

/*
 * Description :
 * Lock system if 3 incorrect attempts
 */
void HMI_errorCheck(uint8 count)
{
	if(count == PASSWORD_ATTEMPTS)
	{
		LCD_clearScreen();
		LCD_displayString("System locked");
		LCD_moveCursor(1, 0);
		LCD_displayString("wait for 1 min");

		/* Wait for 60 seconds to reset attempts */
		g_seconds = 0;
		while(g_seconds < LOCK_TIME_SECONDS);
	}
}

/*
 * Description :
 * Timer callback to increment seconds
 */
void HMI_timerCallback(void)
{
	g_seconds++;
}


