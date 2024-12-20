/*
 * dc_motor.c
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#include "dc_motor.h"
#include "std_types.h"
#include "gpio.h"

/*
 * Description :
 * Initializes the DC motor by setting the direction for the motor pins and stopping the motor at the beginning.
 */

void DcMotor_Init(void)
{
	GPIO_setupPinDirection(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_EN_PORT_ID, MOTOR_EN_PIN_ID, PIN_OUTPUT);

	DcMotor_Rotate(STOP, 0);
}

/*
 * Description :
 * Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based on the input duty cycle.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
	PWM_Timer0_Start( (uint8)( (uint16)(speed  * 255) / 100 ) );
	switch(state)
	{
	case STOP:
		GPIO_writePin(MOTOR_EN_PORT_ID, MOTOR_EN_PIN_ID, LOGIC_LOW);
		break;
	case ACW:
		GPIO_writePin(MOTOR_EN_PORT_ID, MOTOR_EN_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(MOTOR_EN_PORT_ID, MOTOR_EN_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
		break;
	}
}

