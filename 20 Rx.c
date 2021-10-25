//theory 
//https://newbiehack.com/USARTDetailed.aspx

//https://newbiehack.com/UART-OneWay-ChipToChip.aspx



include <avr/io.h>
int main(void)
{
	DDRB |= (1 << PINB0);

	//Communication UART specifications (Parity, stop bits, data bit length) 
	int UBRR_Value = 25; //This is for 2400 baud
	UBRR0H = (unsigned char) (UBRR_Value >> 8);
	UBRR0L = (unsigned char) UBRR_Value;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);

	unsigned char receiveData;
	while (1)
	{
		while (! (UCSR0A & (1 << RXC0)) );

		receiveData = UDR0;

		if (receiveData == 0b11110000) PORTB ^= (1 << PINB0);
	}
}