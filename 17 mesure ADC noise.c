
// https://newbiehack.com/MicrocontrollersAVRAtmegaMeasureADCNoiseADCNoiseReductionMode.aspx



#include <avr/io.h>
#include <avr/interrupt.h>
#include "MrLCD.h"
static volatile uint16_t previousResult = 0;
static volatile int totalDeflectionOverTime = 0;
static volatile uint16_t sampleCount = 0; int main(void)
{
InitializeMrLCD(); 
Send_A_StringToMrLCDWithLocation(1,1,"ADC Result:"); 
Send_A_StringToMrLCDWithLocation(1,2,"Deflection:");
Send_A_StringToMrLCDWithLocation(1,3,"Total:");

ADCSRA |= 1<<ADPS2;
ADMUX |= (1<<REFS0) | (1<<REFS1);
ADCSRA |= 1<<ADIE;
ADCSRA |= 1<<ADEN; 

sei();

MCUCR |= 1<<SM0;
MCUCR |= 1<<SE;

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

int deflection = theTenBitResults - previousResult;
if (deflection < 0 ) deflection = previousResult - theTenBitResults;
Send_An_IntegerToMrLCD(13,2,deflection, 4);
previousResult = theTenBitResults;
totalDeflectionOverTime += deflection; 

sampleCount++;

if (sampleCount >= 300)
{
Send_An_IntegerToMrLCD(8,3,totalDeflectionOverTime, 6);
totalDeflectionOverTime = 0;
sampleCount = 0;
}

ADCSRA |= 1<<ADSC; 
}