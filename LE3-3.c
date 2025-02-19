#include <xc.h> // include file for xc8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit myINTF = 0;
bit myTMR0IF = 0;

void delay (int count)
{
	int of_count = 0;
	
	while(of_count < count)
	{
		if(myTMR0IF)
		{
			of_count++;
			myTMR0IF = 0;
		}
	}
}

void interrupt ISR()
{
	GIE = 0;
	
	if(INTF)
	{
		INTF = 0;
		myINTF ^= 1;
	} 
	
	if (TMR0IF)
	{
		TMR0IF = 0;
		myTMR0IF = 1;
	}
	
	GIE = 1;
}

void main () 
{
	unsigned char counter = 0x00; // initial count = 0
	TRISB0 = 1; // input RB0 for interrupt
	TRISD = 0xFF; // input port for keypad
	TRISC = 0x00; // output to 7segment display
	
	OPTION_REG = 0xC4;
	INTE = 1;
	INTF = 0;
	
	TMR0IE = 1;
	GIE = 1;
	
	while (1)
	{
		PORTC = counter;
		
		if(myINTF == 0)
		{
			while(myINTF != 0)
			{ 
				// do nothing here just keep counting
			}
		} 
		else if (myINTF == 1 && counter == 0x09)
		{	
			delay (122);
			counter = 0x00; // set counter back to 0 if current counter 
		}
		else
		{	
			delay (122);
			counter++;
		}
	}
	
	PORTC = counter;
}
