/*
 * proj.c
 *
 *  Created on: Sep 30, 2024
 *      Author: Manar
 */

#include "lcd.h"
#include "keypad.h"
#include "std_types.h"
#include <util/delay.h>

int main(void)
{
	LCD_init();

	uint16 num1 = 0, num2 = 0;
	uint8 flag = 0, op = 0;

	while(1)
	{
		uint8 key = KEYPAD_getPressedKey();
		_delay_ms(200); // Small delay to avoid key bouncing

		// Handle numeric keys
		if( (key >= 0) && (key <= 9) )
		{
			LCD_intgerToString(key);
			if(flag == 0)
				num1 = num1 * 10 + key;
			else
				num2 = num2 *10 + key;
		}

		// Handle operator keys (+, -, *, /)
		else if( (key == '+') || (key == '-') || (key == '*') || (key == '%') )
		{
			LCD_displayCharacter(key);
			if(flag == 0)
			{
				op = key;
				flag = 1; // Switch to second operand input
			}
			else // If an operator is already selected, show error
			{
				LCD_moveCursor(1, 11);
				LCD_displayString("Error");
				key = 20; // Set the key to a value that is not numerical or operational to prevent repetition
			}
		}

		// Clear the screen if key 13 is pressed
		else if(key == 13)
			LCD_clearScreen();

		// Handle the equal key for calculating the result
		else if(key == '=')
		{
			uint32 result;
			switch(op)
			{
			case '+': result = num1 + num2; break;
			case '-': result = num1 - num2; break;
			case '*': result = num1 * num2; break;
			case '%':
				if(num2 != 0)  // Check for division by zero
				result = num1 / num2;
				else
				{
					LCD_moveCursor(1, 11);
					LCD_displayString("Error");
					continue;  // Skip the rest of the loop to avoid displaying incorrect result
				}
				break;
			default: result = num1; // If no operator is selected, display num1
			}
			uint8 i = 0;
			uint32 res = result;
			while(res)
			{
				i++;
				res /= 10;
			}

			// Move the cursor to display the result and reset the values
			LCD_moveCursor(1, 16 - i);
			LCD_intgerToString(result);

			// Reset variables for a new calculation
			num1 = num2 = flag = op = 0;
		}
	}
}
