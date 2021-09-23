/*
 * cbott001_lab5_part1.c
 *
 * Created: 4/16/2019 8:23:02 AM
 * Author : Chad
 */ 


#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}


int main(void)
{
    //ok so A0-3 is what the fuel level is at
	// then PC5-0 should graphically show what the fuel is at starts at C5 and you add more bits when you go up by 2
	//PC6 is low fuel and it lights up when fuel is 4 or less
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00;
    while (1) 
    {
		unsigned char temp = ~PINA & 0x0F;
		unsigned char tmpC = 0x00;
		temp = temp & 0x0F;
		
		switch(temp){
		case 0:
		case 1:
		case 2:
		tmpC = 0x60;
		break;	
		
		case 3:
		case 4:
		tmpC = 0x70;
		break;
		
		case 5:
		case 6:
		tmpC = 0x38;
		break;
		
		case 7:
		case 8:
		case 9:
		tmpC = 0x3C;
		break;
		
		case 10:
		case 11:
		case 12:
		tmpC = 0x3E;
		break;
		
		case 13:
		case 14:
		case 15:
		tmpC = 0x3F;
		break ;
		
		default:
		break;
			
		}
		
		PORTB = tmpC;
    }
}

