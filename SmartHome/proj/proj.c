/*
 * proj.c
 *
 *  Created on: Oct 3, 2024
 *      Author: Manar
 */

#include "adc.h"
#include "buzzer.h"
#include "dc_motor.h"
#include "flame_sensor.h"
#include "lcd.h"
#include "ldr_sensor.h"
#include "led.h"
#include "lm35_sensor.h"
#include <util/delay.h>

int main(void)
{
	/* Modules initialization */
	ADC_init();
	Buzzer_init();
	DcMotor_Init();
	FlameSensor_init();
	LCD_init();
	LEDS_init();

	/* Initialize display for Temp and LDR */
	LCD_moveCursor(1, 0);
	LCD_displayString("Temp=  ");
	LCD_moveCursor(1, 8);
	LCD_displayString("LDR=   %");

	while(1)
	{
		/* Flame sensor check */
		if(FlameSensor_getValue())
		{
			LCD_clearScreen();
			LCD_moveCursor(0, 0);
			LCD_displayString("Critical alert!");
			Buzzer_on();

			/* Wait until flame is no longer detected */
			while(FlameSensor_getValue());
			LCD_clearScreen();
			Buzzer_off();

			/* Reinitialize the screen after critical alert */
			LCD_moveCursor(1, 0);
			LCD_displayString("Temp=  ");
			LCD_moveCursor(1, 8);
			LCD_displayString("LDR=   %");
		}
		else
		{
			/* Update temperature */
			uint8 temp = LM35_getTemperature();

			/* Display temperature */
			LCD_moveCursor(1, 5);
			LCD_intgerToString(temp);

			/* Clear any extra digits */
			if(temp < 10)
				LCD_displayString(" ");

			/* Fan speed control based on temperature */
			if(temp < 25)
			{
				LCD_moveCursor(0, 4);
				LCD_displayString("FAN is OFF  ");
				DcMotor_Rotate(STOP, 0);
			}
			else if( (temp >= 25) && (temp < 30) )
			{
				LCD_moveCursor(0, 4);
				LCD_displayString("FAN is ON   ");
				DcMotor_Rotate(CW, 25);
			}
			else if( (temp >= 30) && (temp < 35) )
			{
				LCD_moveCursor(0, 4);
				LCD_displayString("FAN is ON   ");
				DcMotor_Rotate(CW, 50);
			}
			else if( (temp >= 35) && (temp < 40) )
			{
				LCD_moveCursor(0, 4);
				LCD_displayString("FAN is ON   ");
				DcMotor_Rotate(CW, 75);
			}
			else
			{
				LCD_moveCursor(0, 4);
				LCD_displayString("FAN is ON   ");
				DcMotor_Rotate(CW, 100);
			}

			/* Update light intensity */
			uint8 light = LDR_getLightIntensity();

			/* Display light intensity */
			LCD_moveCursor(1, 12);
			LCD_intgerToString(light);

			/* Clear any extra digits */
			if( (light < 100) && (light > 10) )
				LCD_displayString(" ");
			else if(light < 10)
				LCD_displayString("  ");

			/* LED control based on light intensity */
			if(light <= 15)
			{
				LED_on(LED_BLUE);
				LED_on(LED_GREEN);
				LED_on(LED_RED);
			}
			else if( (light >= 16) && (light <= 50) )
			{
				LED_off(LED_BLUE);
				LED_on(LED_GREEN);
				LED_on(LED_RED);
			}
			else if( (light >= 51) && (light <= 70) )
			{
				LED_off(LED_BLUE);
				LED_off(LED_GREEN);
				LED_on(LED_RED);
			}
			else
			{
				LED_off(LED_BLUE);
				LED_off(LED_GREEN);
				LED_off(LED_RED);
			}
		}
	}
}
