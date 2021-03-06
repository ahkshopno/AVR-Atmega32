// https://newbiehack.com/MicrocontrollersADC10Bits.aspx

#include <avr/io.h>
#include <avr/interrupt.h>
#include "MrLCD.h"
int main(void)
{
InitializeMrLCD(); 
Send_A_StringToMrLCDWithLocation(1,1,"ADC Result:");
ADCSRA |= 1<<ADPS2;
ADMUX |= (1<<REFS0) | (1<<REFS1);
ADCSRA |= 1<<ADIE;
ADCSRA |= 1<<ADEN;

sei();

ADCSRA |= 1<<ADSC;

while (1)
{
}
}
ISR(ADC_vect)
{
uint8_t theLowADC = ADCL;
uint16_t theTenBitResults = ADCH<<8 | theLowADC;
Send_An_IntegerToMrLCD(13,1,theTenBitResults, 4);

ADCSRA |= 1<<ADSC; 
}