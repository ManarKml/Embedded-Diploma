/*
 * proj.c
 *
 *  Created on: Oct 10, 2024
 *      Author: Manar
 */

#include "buzzer.h"
#include "lcd.h"
#include "led.h"
#include "ultrasonic.h"
#include "common_macros.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/* Enable Global Interrupts */
	SET_BIT(SREG, 7);

	/* Initialize modules */
	LCD_init();
	Ultrasonic_init();
	Buzzer_init();
	LEDS_init();

	/* Display initial message on the LCD */
	LCD_moveCursor(0, 1);
	LCD_displayString("Distance=   cm");

	while(1)
	{
		/* Read distance from Ultrasonic sensor */
		uint16 dist = Ultrasonic_readDistance();
		LCD_moveCursor(0, 10);
		LCD_intgerToString(dist);

		/* Clear any extra digits on LCD */
		if( (dist < 100) && (dist > 10) )
			LCD_displayString(" ");
		else if(dist < 10)
			LCD_displayString("  ");

		/* Control the system behavior based on distance */
		if(dist <= 5)
		{
			LCD_moveCursor(1, 6);
			LCD_displayString("STOP");

			Buzzer_on();

			LED_toggle(LED_RED);
			LED_toggle(LED_GREEN);
			LED_toggle(LED_BLUE);
			_delay_ms(50);
		}
		else if( (dist >= 6) && (dist <= 10) )
		{
			LCD_moveCursor(1, 6);
			LCD_displayString("    "); /* Clear STOP message */

			Buzzer_off(); /* Turn off buzzer */

			/* Turn all LEDs on */
			LED_on(LED_RED);
			LED_on(LED_GREEN);
			LED_on(LED_BLUE);
		}
		else if( (dist >= 11) && (dist <= 15) )
		{
			/* Turn on only RED and GREEN LEDs */
			LED_on(LED_RED);
			LED_on(LED_GREEN);
			LED_off(LED_BLUE);
		}
		else if( (dist >= 16) && (dist <= 20) )
		{
			/* Turn on only RED LED */
			LED_on(LED_RED);
			LED_off(LED_GREEN);
			LED_off(LED_BLUE);
		}
		else if(dist > 20)
		{
			/* Turn all LEDs off */
			LED_off(LED_RED);
			LED_off(LED_GREEN);
			LED_off(LED_BLUE);
		}
	}
}
