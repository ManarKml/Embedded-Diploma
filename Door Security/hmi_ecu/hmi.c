/*
 * hmi.c
 *
 *  Created on: Oct 31, 2024
 *      Author: Manar
 */

#include "hmi_func.h"
#include "timer.h"
#include <avr/io.h>

int main(void)
{
	/*------ UART and LCD Initialization ------*/
	UART_ConfigType UART_Configurations = {DATABITS_8, NONE, STOPBITS_1, 9600};
	UART_init(&UART_Configurations);
	Timer_ConfigType Timer_Configurations = {0, 31249, TIMER_1, F_CPU_256, COMPARE};
	Timer_init(&Timer_Configurations);
	Timer_setCallBack(HMI_timerCallback, TIMER_1);

	LCD_init();

	SREG |= (1<<7); /* Enable global interrupts */

	uint8 flag = 0;


	while(UART_recieveByte() != READY_COMM); /* Wait for Control ECU readiness */

	flag = HMI_putPassword();

	/*------ Main Interface ------*/
	while(1)
	{
		LCD_clearScreen();
		LCD_displayString("+ : open door");
		LCD_moveCursor(1, 0);
		LCD_displayString("- : change pass");

		flag = KEYPAD_getPressedKey();

		switch (flag)
		{
		case '+':
			flag = HMI_checkPassword();
			break;
		case '-':
			flag = HMI_changePassword();
			break;
		}
	}
}





