// https://newbiehack.com/MicrocontrollersADCReadingMultipleAnalogVolageSources.aspx



#include <avr/io.h>
#include <avr/interrupt.h>
#include "MrLCD.h"
int main(void)
{
InitializeMrLCD();
Send_A_StringToMrLCDWithLocation(1,1,"X:");
Send_A_StringToMrLCDWithLocation(1,2,"Y:");

ADCSRA |= 1<<ADPS2;
ADMUX |= 1<<REFS0 | 1<<REFS1;
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
uint8_t theLow = ADCL;
uint16_t theTenBitResult = ADCH<<8 | theLow;

switch (ADMUX)
{
case 0xC0:
Send_An_IntegerToMrLCD(4, 1, theTenBitResult, 4);
ADMUX = 0xC1;
break;
case 0xC1:
Send_An_IntegerToMrLCD(4, 2, theTenBitResult, 4);
ADMUX = 0xC0;
break;
default:
//Default code
break;
} ADCSRA |= 1<<ADSC;
}