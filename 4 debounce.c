// https://newbiehack.com/UnderstandingButtonDebouncing.aspx

#include <avr/io.h>
int main(void)
{
	DDRB |= 1 << PINB0; //Set Direction for output on PINB0
	PORTB ^= 1 << PINB0; //Toggling only Pin 0 on port b
	DDRB |= 1 << PINB2; //Set Direction for Output on PINB2
	DDRB &= ~(1 << PINB1); //Data Direction Register input PINB1
	PORTB |= 1 << PINB1; //Set PINB1 to a high reading
	int Pressed = 0; //Initialize/Declare the Pressed variable

	while (1)
	{
		if (bit_is_clear(PINB, 1)) //Check is the button is pressed
			{
			//Make sure that the button was released first
			if (Pressed == 0) 
				{
				PORTB ^= 1 << PINB0; //Toggle LED in pin 0
				PORTB ^= 1 << PINB2; //Toggle LED on pin 2
				Pressed = 1;
				}
			}
		else
			{
			//This code executes when the button is not pressed.
			Pressed = 0;
			}
	}
}