// https://newbiehack.com/MicrocontrollersSeparatingtheLCDCodeintoaLibrary.aspx


We've produced a bit of reusable code to talk to the LCD, sending commands and characters. We enabled as many functions for the LCD as we need at the moment (we will later implement the 4-wire - 4-bit - mode to use fewer pins on the microcontroller), so this is a good time to take the code and make a library out of it. There are a couple of ways we can make these libraries. The library can reside in a ".h" file and a ".c" file, or it can all reside in the ".h" file. The latter is a bit easier, but is not recommended if there is a huge amount of code.

First, a very important condition must be included in both methods. Sometimes, these libraries may be included in multiples files in the overall project. But, libraries cannot be included more than once. You will see that the #include <avr/io.h> will be included in the main file, and in the library files because both of these files need the information in the io.h file, but this file cannot be loaded twice, so a condition must be added so that the code within the ".h" file is not loaded twice. The #ifndef conditional statement is used in this case.

At the beginning of the ".h" file, we use a #define statement to essentially label the file. Let's say, the io.h file has a #define statement at the beginning like this: #define _IO_H_. The #define statement in this case is just telling the compiler that the label _IO_H_ is defined. That's it! now, if after this statement has been processed and _IO_H_ has been defined, you could ask the compiler: "has _IO_H_ been defined?". The compiler would say yes. We actually do the opposite. We ask if the _IO_H_ has not been defined, and then we tell the compiler to go ahead and process the code. This is where the #ifndef comes in. #ifndef is short for "if not defined". So, in the case of the io.h file, the following would exist:

#ifndef _IO_H_
#define _IO_H_

//All of the code in the .h file

#endif
The #endif would be at the very end of the file. If this file was processed once, you know that the code would be processed because _IO_H_ would not have been defined yet (hence the if not defined). If the code had been processed previously (from another file) then the #define _IO_H_ would have already been processed and the compiler would say to the #ifndef and say, yes it has been defined and I'm not going to process the following code. In fact, I'm going all the way to the end of the file, to the #end if statement and getting out of this file and continuing where I left off!!

We need to do this to our ".h" file. Since I am calling the .h and .c file MrLCD.h and MrLCD.c, we will call the label in the #define statement simply MrLCD, like this:

#ifndef MrLCD
#define MrLCD

//All of the code in our LCD library

#endif
The First Method:

So, now that we got that out of the way, let's talk about the first method, even though I will be using the second. As I said, in the first method, two files will be created, a ".h" file and a ".c" file. The ".h" file will contain all of the macros (#define) statements, and all of the prototypes. The ".c" file will contain all of the actual routines. The main reason the ".h" file is created with only the definitions and prototypes is to give you a convenient place to change configurations. In the case of the LCD library, the #define statements contain information that will most likely change, such as the port that is connected to the data lines of the LCD, and the port and pins that will receive the control lines of R/W (Read/Write), RS (Register Select) and E (enable).

In the tutorial, I name the ".c" and ".h" files MrLCD.c and MrLCD.h. We will create these file by clicking on File -> New -> C / C++. When you do this, you will see a new tab appear called "new". You will do this two times, one for MrLCD.c and MrLCD.h. To name the tabs, you will need to use the "Save As..." menu selection and you will be prompted to name the file. Optionally, you can name the files when you close the files since the program will ask you for the name of the file.

Now we need to copy the information from the main file into the ".h" file and ".c" file. For the .h file, you will need to copy the #include statements, #define statements and prototype information:

#ifndef MrLCD
#define MrLCD

//These are the Includes
#include <avr/io.h>
#include <util/delay.h>

//These are the define statements
#define MrLCDsCrib PORTB
#define DataDir_MrLCDsCrib DDRB
#define MrLCDsControl PORTD
#define DataDir_MrLCDsControl DDRD
#define LightSwitch 5
#define ReadWrite 7
#define BiPolarMood 2

//These are the prototypes for the routines
void Check_IF_MrLCD_isBusy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *StringOfCharacters);
void GotoMrLCDsLocation(uint8_t x, uint8_t y);
void InitializeMrLCD(void);

#endif
Notice the #ifndef and the #endif. This code would only happen one time in the entire project, even if I included this file in many other files. This will come in handy in larger projects, but we are using include statements for other files more than once.

The ".c" file will receive the all of the routines (except the main routine) and the ".c" file will also receive any global variables that the routines use. We have one called: firstColumnPositionsForMrLCD which is declared at the top of the file, but this could have been defined just before the routine that uses it. The ".c" file also has the #include statements that the routines use.

The ".c" file may look like this:

//These are the Includes
#include <avr/io.h>
#include <util/delay.h>
#include "Mr.LCD"

//These are the Routines
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

void InitializeMrLCD()
{
DataDir_MrLCDsControl |= 1<<LightSwitch | 1<<ReadWrite | 1<<BiPolarMood;
_delay_ms(15);

Send_A_Command(0x01); //Clear Screen 0x01 = 00000001
_delay_ms(2);
Send_A_Command(0x38);
_delay_us(50);
Send_A_Command(0b00001110);
_delay_us(50);
}
You might notice a few differences. First, there is no need to use the #ifndef since the ".h" file already has that. Second, there is a need to add an include to the "Mr.LCD.h" to get the prototypes. And finally, there is a new routine called InitializeMrLCD. This routine just packages up the first few lines of code pertaining to the LCD. We don't want that code taking up any room in our main file. Using it this way, all we need to do it add a line in the main routine:

InitializeMrLCD();
The first method also requires you to inform the makefile of the ".c" file that will be included. In this file, you will have the remark: "# List C source files here." Just under that statement, you will see "SRC = $(TARGET).c ". The new ".c" file that you just created will be included here. This is what that are might look like:

# List C source files here. (C dependencies are automatically generated.)
SRC = $(TARGET).c MrLCD.c
That is all you need to do in the makefile.

The Second Method:

The second method is the easiest, but as the program gets very long, it may pose an issue on readability, so for very large files, the first method is the best one to use. There is a big benefit to using this method, and I used the word easiest, because it is easy. You are not going to mess with the makefile and add the MrLCD.c reference. You will only copy and paste the code pertaining to the LCD into a ".h" file. We will call this file "MrLCD.h", just like before. After that, all you need to do is have an #include "MrLCD.h" at the top of your main file and add the InitializeMrLCD() in the main routine. Below is the main file and the MrLCD.h file and how they may appear:

The Main File (main.c):

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "MrLCD.h"
int main(void)
{
InitializeMrLCD();

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
The Header file (MrLCD.h):

#ifndef MrLCD
#define MrLCD

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
void InitializeMrLCD(void);

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

void InitializeMrLCD()
{
DataDir_MrLCDsControl |= 1<<LightSwitch | 1<<ReadWrite | 1<<BiPolarMood;
_delay_ms(15);

Send_A_Command(0x01); //Clear Screen 0x01 = 00000001
_delay_ms(2);
Send_A_Command(0x38);
_delay_us(50);
Send_A_Command(0b00001110);
_delay_us(50);
}

#endif