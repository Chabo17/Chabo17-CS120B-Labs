/*
 * cbott001_lab3_part4.c
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


int main(void)
{
    /* Replace with your application code */
		DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
		DDRC = 0xFF; PORTC = 0x00;
		DDRB = 0xFF; PORTB = 0x00;
    while (1) 
    {
		unsigned char tmpB = PINA;
		unsigned char tmpC = PINA;
		tmpB = PINA & 0xF0;
		tmpC = PINA & 0x0F;
		tmpB = tmpB >> 4;
		tmpC = tmpC << 4;
		PORTB = tmpB;
		PORTC = tmpC;
    }
}

