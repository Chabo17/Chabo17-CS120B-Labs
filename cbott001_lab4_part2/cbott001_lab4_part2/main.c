/*
 * cbott001_lab4_part2.c
 *
 * Created: 4/10/2019 5:49:21 PM
 * Author : Chad
 */ 

#include <avr/io.h>


enum states { wait, increment,i2, decrement,d2, reset, r2 };


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRC = 0xFF; PORTC = 0x00;
	
	//so when PINA = 1 increment, when PINA = 2 decrement, when PINA = 3 reset
	enum states FSM = wait;
	unsigned char tmpB = 0x07;
	while (1)
	{
		unsigned char tmpA = PINA;
		
		
		switch(FSM){
			case wait:
			if(tmpA == 0x01){
				FSM = increment;
			} else if(tmpA == 0x02){
				FSM = decrement;
			} else if(tmpA == 0x03){
				FSM = reset;
			}
			break;
			
			case increment:
			tmpB = tmpB + 1;
			FSM = i2;
			break;
			
			case i2:
			if(tmpA != 0x01){
				FSM = wait;
			}
			break;
			
			case decrement:
			tmpB = tmpB -1;
			FSM = d2;
			break;
			
			case d2:
			if(tmpA != 0x02){
				FSM = wait;
			}
			break;
			
			case reset:
			tmpB = 0x00;
			FSM = r2;
			break;
			
			case r2:
			if(tmpA != 0x03){
				FSM = wait;
			}
			break;
			
			default:
			FSM = wait;
			break;
			
		}
		PORTC = tmpB;
	}
}
