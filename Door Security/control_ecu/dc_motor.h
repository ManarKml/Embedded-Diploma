/*
 * dc_motor.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "gpio.h"
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* DC Motor Port Configurations */
#define MOTOR_IN1_PORT_ID          PORTD_ID
#define MOTOR_IN1_PIN_ID           PIN6_ID

#define MOTOR_IN2_PORT_ID          PORTD_ID
#define MOTOR_IN2_PIN_ID           PIN7_ID

#define MOTOR_EN_PORT_ID           PORTB_ID
#define MOTOR_EN_PIN_ID            PIN3_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	STOP, ACW, CW
}DcMotor_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initializes the DC motor by setting the direction for the motor pins and stopping the motor at the beginning.
 */

void DcMotor_Init(void);

/*
 * Description :
 * Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based on the input duty cycle.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);


#endif /* DC_MOTOR_H_ */
