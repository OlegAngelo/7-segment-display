#include <xc.h> // Include file for XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit myINTF = 0; // we use for interrupt
bit myTMR0IF = 0; // we use for delay overflow
bit keypadFlag = 0; // indicate that keypad is pressed and need to switch case
unsigned char counter = 0x00; 
unsigned char keypadData = 0x00; 

void delay(int count)
{
	 int of_count = 0;
    
    	while (of_count < count)
   	 {
    		if (myTMR0IF)
      	 {
           		of_count++;
            	myTMR0IF = 0; // reset overflow flag 
       	 }
   	 }
}

void interrupt ISR()
{
	GIE = 0;

    	if (INTF) 
    	{
       	INTF = 0; // reset flag manually
       	myINTF ^= 1; // xor
       	
        	keypadData = PORTD & 0x0F;
        	keypadFlag = 1;
    	}

    	if (TMR0IF) // Timer overflow interrupt
    	{
        	TMR0IF = 0; // reset flag manually
        	myTMR0IF = 1; 
    	}

    	GIE = 1;
}

void processKeypad()
{
    if (keypadFlag)
    {
        keypadFlag = 0; // Reset flag
        
        switch (keypadData)
        {
            case 0x00: counter = 1; break;
            case 0x01: counter = 2; break;
            case 0x02: counter = 3; break;
            case 0x04: counter = 4; break;
            case 0x05: counter = 5; break;
            case 0x06: counter = 6; break;
            case 0x08: counter = 7; break;
            case 0x09: counter = 8; break;
            case 0x0A: counter = 9; break;
            case 0x0D: counter = 0; break;
            default: counter = counter; // Ignore invalid input
        }
    }
}

void main()
{
    	TRISB0 = 1; 
    	TRISD = 0xFF; 
    	TRISC = 0x00; 

    	OPTION_REG = 0xC4; 
    	INTE = 1;
    	INTF = 0; 

    	TMR0IE = 1; 
    	GIE = 1;

    	while (1)
    	{
	    	processKeypad();
	    	
       	PORTC = counter;
        	delay(61); 

		// uncomment below if we want to stop when we click btn/key
       	/* if(myINTF == 0)
		{
			while(myINTF != 0)
			{ 
				// do nothing here just keep counting
			}
		} */
        	
        	if (counter == 0x09) 
        	{
            	counter = 0x00;  // reset counter to 0
        	}
        	else 
	      {
	        	counter++;
	      }	
    	}
}
