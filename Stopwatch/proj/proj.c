/*
 * proj.c
 *
 *  Created on: Sep 10, 2024
 *      Author: Manar
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*-----------------------------Prototypes-----------------------------*/
void TIMER1_init(void);
void INT0_init(void);
void INT1_init(void);
void INT2_init(void);
void TimeDisplay(void); // Display time on the 7-segment
void Display(unsigned char); // Put a number in PORTC to send it to the 7-segment
void CheckMode(void); // Checks if the mode changed

/*--------------------------Global Variables--------------------------*/
unsigned char hour = 0, min = 0, sec = 0;
unsigned char mode = 1;

/*----------------------------Main Program----------------------------*/
int main(void)
{
	DDRA |= 0x3F; // Multiplexed 7-Segment Enables
	DDRB &= 0x0; // Control pushbuttons
	DDRC |= 0x0F; // 7 Segment decoder
	DDRD |= 0x31; // Mode LEDs & Buzzer
	DDRD &= ~(0x0C); // Control pushbuttons

	PORTB |= 0x7F; // Enable internal pull up resistor
	PORTD |= (1<<PD2);

	PORTD |= (1<<PD4); // Count Up LED

	PORTB |= (1<<PB7); // Count Up Mode

	SREG |= (1<<7); // Global Interrupt Enable
	INT0_init();
	INT1_init();
	INT2_init();
	TIMER1_init();

	while(1)
	{
		TimeDisplay(); // Display hours, minutes, seconds on the 7-segment

		CheckMode(); // Check mode button status

		// Manual time adjustment
		if( !(PINB & (1<<PB0)) ) // Decrement hours
		{
			if(hour > 0)
				hour--;
			while( !(PINB & (1<<PB0)) )
				TimeDisplay(); // Display until button is released
		}
		if( !(PINB & (1<<PB1)) ) // Increment hours
		{
			if(hour < 99)
				hour++;
			while( !(PINB & (1<<PB1)) )
				TimeDisplay(); // Display until button is released
		}
		if( !(PINB & (1<<PB3)) ) // Decrement minutes
		{
			if(min > 0)
				min--;
			else if( (min == 0) && (hour > 0) )
			{
				hour--;
				min = 59;
			}
			while( !(PINB & (1<<PB3)) )
				TimeDisplay(); // Display until button is released
		}
		if( !(PINB & (1<<PB4)) ) // Increment minutes
		{
			if(min == 59)
			{
				hour++;
				min = 0;
			}
			else
				min++;
			while( !(PINB & (1<<PB4)) )
				TimeDisplay(); // Display until button is released
		}
		if( !(PINB & (1<<PB5)) ) // Decrement seconds
		{
			if( (sec == 0) && (min == 0) && (hour > 0) )
			{
				hour--;
				min = 59;
				sec = 59;
			}
			else if( (sec == 0) && (min > 0) )
			{
				min--;
				sec = 59;
			}
			else
				sec--;
			while( !(PINB & (1<<PB5)) )
				TimeDisplay(); // Display until button is released
		}
		if( !(PINB & (1<<PB6)) ) // Increment seconds
		{
			if(sec == 59)
			{
				sec = 0;
				if(min == 59)
				{
					hour++;
					min = 0;
				}
				else
					min++;
			}
			else
				sec++;
			while( !(PINB & (1<<PB6)) )
				TimeDisplay(); // Display until button is released
		}
	}
}

/*-------------------------------Functions-------------------------------*/
void TIMER1_init(void)
{
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12); // Prescaler = 1024
	TCNT1 = 0;
	OCR1A = 15624; // TOP value
	TIMSK |= (1<<OCIE1A); // Timer1 Compare Mode Interrupt Enable
}

void INT0_init(void)
{
	GICR |= (1<<INT0); // INT0 Enable
	MCUCR |= (1<<ISC01); // For falling edge
	MCUCR &= ~(1<<ISC00);
}

void INT1_init(void)
{
	GICR |= (1<<INT1); // INT1 Enable
	MCUCR |= (1<<ISC11) | (1<<ISC10); // For rising edge
}

void INT2_init(void)
{
	GICR |= (1<<INT2); // INT2 Enable
	MCUCSR &= ~(1<<ISC2); // For falling edge
}

void TimeDisplay(void) // Display time on the 7-segment
{
	PORTA = (PORTA & 0xC0) | (0x01); // Enable first 7-segment
	Display(hour / 10); // Display tens place of hours
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) & (0x00); // Disable all 7-segments to prevent numbers overlapping

	PORTA = (PORTA & 0xC0) | (0x02); // Enable second 7-segment
	Display(hour % 10); // Display ones place of hours
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) & (0x00); // Disable all 7-segments to prevent numbers overlapping

	PORTA = (PORTA & 0xC0) | (0x04); // Enable third 7-segment
	Display(min / 10); // Display tens place of minutes
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) & (0x00); // Disable all 7-segments to prevent numbers overlapping

	PORTA = (PORTA & 0xC0) | (0x08); // Enable fourth 7-segment
	Display(min % 10); // Display ones place of minutes
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) & (0x00); // Disable all 7-segments to prevent numbers overlapping

	PORTA = (PORTA & 0xC0) | (0x10); // Enable fifth 7-segment
	Display(sec / 10); // Display tens place of seconds
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) & (0x00); // Disable all 7-segments to prevent numbers overlapping

	PORTA = (PORTA & 0xC0) | (0x20); // Enable sixth 7-segment
	Display(sec % 10); // Display ones place of seconds
	_delay_ms(2);
	PORTA = (PORTA & 0xC0) & (0x00); // Disable all 7-segments to prevent numbers overlapping
}

void Display(unsigned char num) // Put a number in PORTC to send it to the 7-segment
{
	PORTC = (PORTC & 0xF0) | (num & 0x0F);
}

void CheckMode(void) // Checks if the mode changed
{
	if( !(PINB & (1<<PB7)) )
	{
		mode = !mode;
		if(mode)
		{
			// Count Up Mode
			PORTD |= (1<<PD4); // Enable Count Up LED
			PORTD &= ~(1<<PD5);  // Disable Count Down LED
			PORTD &= ~(1<<PD0); // Disable Buzzer
		}
		else
		{
			// Count Down Mode
			PORTD |= (1<<PD5); // Enable Count Down LED
			PORTD &= ~(1<<PD4); // Disable Count Up LED
		}
		while( !(PINB & (1<<PB7)) )
			TimeDisplay(); // Display until button is released
	}
}

/*---------------------------------ISRs---------------------------------*/
ISR(TIMER1_COMPA_vect)
{
	if(mode)
	{
		// Count Up Mode
		sec++;
		if(sec == 60)
		{
			min++;
			sec = 0;
		}
		if(min == 60)
		{
			hour++;
			min = 0;
		}
	}
	else
	{
		// Count Down Mode
		if(sec == 0)
		{
			if(min == 0)
			{
				if(hour > 0)
				{
					hour--;
					min = 59;
					sec = 59;
				}
				else
					PORTD |= (1<<PD0);
			}
			else
			{
				min--;
				sec = 59;
			}
		}
		else
			sec--;
	}
}



ISR(INT0_vect) // Reset
{
	hour = 0;
	min = 0;
	sec = 0;
}

ISR(INT1_vect) // Pause
{
	TCCR1B &= 0xF8; // Clear the prescaler to turn OFF Timer1
}

ISR(INT2_vect) // Resume
{
	TCCR1B |= (1<<CS12) | (1<<CS10); // Put the prescaler = 1024 to turn ON Timer1
}

