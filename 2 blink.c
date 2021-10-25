//https://newbiehack.com/MicrocontrollerLEDblink.aspx

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= 1 << PINB0; 
	while (1)
		{
		PORTB ^= 1 << PINB0;
		_delay_ms(100);
		}
}
/*
//it can be impliment by this :P 
PORTB |= 1 << PINB0;
_delay_ms(100);
PORTB &= ~(1 << PINB0);
_delay_ms(100);
*/