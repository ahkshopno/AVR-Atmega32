// https://newbiehack.com/TheButtonGame.aspx



#include <avr/io.h>
#include <util/delay.h>

void ProcessPressedButton(int ButtonPressed);
void ProcessReleasedButton(int ButtonReleased);
int Pressed_Confidence_Level[2];
int Released_Confidence_Level[2];
int Pressed[2];
int LEDNumber[2];
int main(void)
{
	DDRB = 0b01111111;
	DDRD = 0b01111111;
	PORTB = 0b10000000;
	PORTD = 0b10000000;

	while (1)
	{
		if (bit_is_clear(PINB, 7))
		{
			ProcessPressedButton(0);
		}
		else
		{
			ProcessReleasedButton(0);
		}
		if (bit_is_clear(PIND, 7))
		{
			ProcessPressedButton(1);
		}
		else
		{
			ProcessReleasedButton(1);
		}
	}
}

void ProcessPressedButton(int ButtonPressed)
{
	Pressed_Confidence_Level[ButtonPressed] ++;
	if (Pressed_Confidence_Level[ButtonPressed] > 500)
		{
			if (Pressed[ButtonPressed] == 0)
				{
			Pressed[ButtonPressed] = 1;
			if (ButtonPressed == 0) PORTB |= 1 << LEDNumber[ButtonPressed];
			if (ButtonPressed == 1) PORTD |= 1 << LEDNumber[ButtonPressed]; 
			LEDNumber[ButtonPressed] ++;
			if (LEDNumber[ButtonPressed] >6)
			{
			for(int i=0;i < 10;i++)
			{
				if (ButtonPressed == 0) PORTB = 0b11111111;
				if (ButtonPressed == 1) PORTD = 0b11111111;
				_delay_ms(10);
				if (ButtonPressed == 0) PORTB = 0b10000000;
				if (ButtonPressed == 1) PORTD = 0b10000000;
				_delay_ms(10);
			}
			LEDNumber[0] = 0;
			LEDNumber[1] = 0;
			PORTB = 0b10000000;
			PORTD = 0b10000000;
			}
			}
			Pressed_Confidence_Level[ButtonPressed] = 0;
		}
}

void ProcessReleasedButton(int ButtonReleased)
{
		Released_Confidence_Level[ButtonReleased] ++;
		if (Released_Confidence_Level[ButtonReleased] > 500)
		{
			Pressed[ButtonReleased] = 0;
			Released_Confidence_Level[ButtonReleased] = 0;
		}
}