// https://newbiehack.com/IntroductiontoInterrupts.aspx
//all isr list https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html


#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
sei();

DDRB |= 1<<PINB0;

TCCR1B |= 1<<CS10 | 1<<CS11 | 1<<WGM12;
TIMSK |= 1<<OCIE1A; //If using atmega324, this regester is TIMSK1
OCR1A = 15624; 

while(1)
{
}
}

ISR(TIMER1_COMPA_vect)
{
PORTB ^= 1<<PINB0;
}