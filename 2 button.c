//https://newbiehack.com/MicrocontrollerAddingabutton.aspx


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= 1 << PINB0;
	DDRB &= ~(1 << PINB1);
	PORTB |= 1 << PINB1;

	while (1)
	{
		PORTB ^= 1 << PINB0;
		if (bit_is_clear(PINB, 1))
			{
			_delay_ms(10); //Fast
			}
		else
			{
			_delay_ms(100); //Slow, from previous
			}
	}
}

/*if (bit_is_clear(PINB, 1)) The name "bit_is_clear" 
represents a function that takes two arguments. In this
 case the first argument is PINB, which describes the set
  of pins we are specifying. The second argument represents 
  which pin we are checking, and in this case we are concerned 
  with pin #1 in the set.*/