/*
 * cbott001_lab5_part3.c
 *
 * Created: 4/16/2019 5:01:42 PM
 * Author : Chad
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure PORTB as outputs, initialize to 0s
	unsigned char led = 7;	
	unsigned char button = 0x00;
	unsigned char b2 = 0x00;
	unsigned char state =0x00;
	while(1)
	{
		// if PA0 is 1, set PB1PB0=01, else =10
		// 1) Read inputs
		button = ~PINA & 0x01; // button is connected to A0
		
		switch(state){
		case 0:
			led = 0x55;
			if(button==0x01){
				state = 2;
				b2=1;
			}
		break;
		case 1:
			led = 0xAA;
			if(button==0x01){
				state =2;
				b2=0;
			}
			break;
		case 2:
			led =0x00;
			if(button == 0x00){
				state = b2;
			}
			break;
		default:
			state =0;
			break;
			}
		PORTB = led;		
	}
}
