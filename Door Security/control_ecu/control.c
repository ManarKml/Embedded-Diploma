/*
 * control.c
 *
 *  Created on: Oct 31, 2024
 *      Author: Manar
 */

#include "control_func.h"
#include <avr/io.h>

int main(void)
{
	/*------ Modules Initialization ------*/
	UART_ConfigType UART_Configurations = {DATABITS_8, NONE, STOPBITS_1, 9600};
	UART_init(&UART_Configurations);
	TWI_ConfigType TWI_Configurations = {0x01, 0x02};
	TWI_init(&TWI_Configurations);

	Timer_ConfigType Timer_Configurations = {0, 31249, TIMER_1, F_CPU_256, COMPARE};
	Timer_init(&Timer_Configurations);
	Timer_setCallBack(Control_timerCallback, TIMER_1);

	Buzzer_init();
	DcMotor_Init();
	PIR_init();

	SREG |= (1<<7); /* Enable global interrupts */

	uint8 flag = CHANGE_PASS;

	UART_sendByte(READY_COMM); /* Signal readiness to HMI */
	_delay_ms(50);

	/*------ Main Loop ------*/
	while(1)
	{
		flag = UART_recieveByte();

		switch(flag)
		{
		case PUT_PASS:
			flag = Control_putPassword();
			break;
		case CHANGE_PASS:
			flag = Control_changePassword();
			break;
		case CHECK_PASS:
			flag = Control_checkPassword();
			if(flag == PASS_MATCH)
			{
				Control_openDoor();
			}
			break;
		}
	}
}

