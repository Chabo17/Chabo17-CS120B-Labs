/*
 * cbott001_lab4_part1.c
 *
 * Created: 4/10/2019 5:17:54 PM
 * Author : Chad
 */ 

#include <avr/io.h>

enum states { offRelease, onPress, onRelease, offPress };
	
	
int main(void)
{
    	DDRD = 0x00; PORTD = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
    	DDRB = 0xFF; PORTB = 0x00;
			
		// so I need 4 states
		// 1 for off release, on Press, on release, off press
		
		
			
		enum states FSM = offRelease;
		
    while (1) 
    {
		unsigned char tmpA = PINA;
		unsigned char tmpB= 0x01;
		
		switch(FSM){
		case offRelease:
		if((tmpA & 0x01) == 0x01){
			FSM = onPress;
		}
		break;
		
		case onPress:
		if((tmpA & 0x01) != 0x01){
			FSM = onRelease;
		}
		tmpB = 0x02;
		break;
		
		case onRelease:
		tmpB = 0x02;
		if((tmpA & 0x01) == 0x01){
			FSM = offPress;
		}
		break;
		
		case offPress:
		tmpB = 0x01;
		if((tmpA & 0x01) != 0x01){
			FSM = offRelease;
		}
		break;
		
		default:
		FSM = offRelease;
		break;	
			
		}
		PORTB = tmpB;
    }
}

