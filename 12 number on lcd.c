// https://newbiehack.com/MicrocontrollersMakingtheLCDShowNumbers.aspx


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define MrLCDsCrib PORTB
#define DataDir_MrLCDsCrib DDRB
#define MrLCDsControl PORTD
#define DataDir_MrLCDsControl DDRD
#define LightSwitch 5
#define ReadWrite 7
#define BiPolarMood 2

char firstColumnPositionsForMrLCD[4] = {0, 64, 20, 84};
void Check_IF_MrLCD_isBusy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *StringOfCharacters);
void GotoMrLCDsLocation(uint8_t x, uint8_t y);

int main(void)
{
DataDir_MrLCDsControl |= 1<<LightSwitch | 1<<ReadWrite | 1<<BiPolarMood;
_delay_ms(15);

Send_A_Command(0x01); //Clear Screen 0x01 = 00000001
_delay_ms(2);
Send_A_Command(0x38);
_delay_us(50);
Send_A_Command(0b00001110);
_delay_us(50);

char positionString[4];

while(1)
{
for(int y = 1; y<=4; y++)
{
for(int x = 1;x<=20;x++)
{

itoa(x, positionString, 10);
GotoMrLCDsLocation(12, 3);
Send_A_String("X = ");
Send_A_String(positionString);

itoa(y, positionString, 10);
GotoMrLCDsLocation(12, 4);
Send_A_String("Y = ");
Send_A_String(positionString);

GotoMrLCDsLocation(x, y);
Send_A_String("x");

_delay_ms(50);

GotoMrLCDsLocation(x, y);
Send_A_String(" ");

itoa(x, positionString, 10);
GotoMrLCDsLocation(12, 3);
Send_A_String(" ");
itoa(y, positionString, 10);
GotoMrLCDsLocation(12, 4);
Send_A_String(" ");
}
}
}
}

void Check_IF_MrLCD_isBusy()
{
DataDir_MrLCDsCrib = 0;
MrLCDsControl |= 1<<ReadWrite;
MrLCDsControl &= ~1<<BiPolarMood;

while (MrLCDsCrib >= 0x80)
{
Peek_A_Boo();
}

DataDir_MrLCDsCrib = 0xFF; //0xFF means 0b11111111
}

void Peek_A_Boo()
{
MrLCDsControl |= 1<<LightSwitch;
asm volatile ("nop");
asm volatile ("nop");
MrLCDsControl &= ~1<<LightSwitch;
}

void Send_A_Command(unsigned char command)
{
Check_IF_MrLCD_isBusy();
MrLCDsCrib = command;
MrLCDsControl &= ~ ((1<<ReadWrite)|(1<<BiPolarMood));
Peek_A_Boo();
MrLCDsCrib = 0;
}

void Send_A_Character(unsigned char character)
{
Check_IF_MrLCD_isBusy();
MrLCDsCrib = character;
MrLCDsControl &= ~ (1<<ReadWrite);
MrLCDsControl |= 1<<BiPolarMood;
Peek_A_Boo();
MrLCDsCrib = 0;
}

void Send_A_String(char *StringOfCharacters)
{
while(*StringOfCharacters > 0)
{
Send_A_Character(*StringOfCharacters++);
}
}

void GotoMrLCDsLocation(uint8_t x, uint8_t y)
{
Send_A_Command(0x80 + firstColumnPositionsForMrLCD[y-1] + (x-1));
}
      PrivacyBadger has replaced this Pinterest button.  PrivacyBadger has replaced this Google+ button. About Us - Site Map - Donate