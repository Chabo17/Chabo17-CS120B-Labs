/*
 * cbott001_lab3_part5.c
 *
 *	Chandler Bottomley cbott001@ucr.edu
 *	Josh Torres
 *	Lab Section: 23
 *	Assignment: Lab #  Exercise #
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Chandler Bottomley
 */ 

#include <avr/io.h>




unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
    //so for this challenge we have a 9 bit register and we have to account for that fact that B0 is also an input
	DDRD = 0x00; PORTD = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFE; PORTB = 0x01;
	
    while (1) 
    {
		unsigned short total =PIND;
		unsigned char tmpB = PINB;
		total = total * 2;
		if(tmpB >=1){
			total = total +1;
		}	
		if(total > 70){
			tmpB = 0x02;
		} else if(total > 5){
			tmpB = 0x04;
		} else {
			tmpB = 0x00;
		}
		PORTB = tmpB;
	}
	return 1;
}
	
