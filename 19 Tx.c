//theory 
//https://newbiehack.com/USARTDetailed.aspx

//https://newbiehack.com/UART-OneWay-ChipToChip.aspx


#define numberOfButtons 1

include <avr/io.h>
#include"ButtonPress.h"

int main(void)
{
DDRB |= 1 << PINB1;
DDRB &= ~(1 << PINB0);
PORTB |= 1 << PINB0;

int UBBRValue = 25;

//Put the upper part of the baud number here (bits 8 to 11)
UBRR0H = (unsigned char) (UBBRValue >> 8);

//Put the remaining part of the baud number here
UBRR0L = (unsigned char) UBBRValue; 

//Enable the receiver and transmitter
UCSR0B = (1 << RXEN0) | (1 << TXEN0);

//Set 2 stop bits and data bit length is 8-bit
UCSR0C = (1 << USBS0) | (3 << UCSZ00); 

while (1)
{
if (ButtonPressed(0, PINB, 0, 100))
{
PORTB ^= 1 << PINB1;

//Wait until the Transmitter is ready
while (! (UCSR0A & (1 << UDRE0)) ); 

//Get that data outa here!
UDR0 = 0b11110000;
}
}
}