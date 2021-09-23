/*
 * cbott001_lab4_part5.c
 *
 * Created: 4/10/2019 7:02:55 PM
 * Author : Chad
 */ 

#include <avr/io.h>

enum states { wait, zero};


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00;
	
	//so when PINA = 1 increment, when PINA = 2 decrement, when PINA = 3 reset
	enum states FSM = wait;
	unsigned char tmp = 0x00;
	unsigned char prog = 0x00;
	while (1)
	{
		unsigned char tmpA = PINA;
		
		
		switch(FSM){//A0 = X, A1= Y, A2 = #, A7 = LOCK 
			case wait:
			if(tmpA > 0){
				FSM = zero;
			}
			if(tmpA == 0x01){//X
				prog = (prog == 0x01 || prog == 0x03) ? prog + 1 : 0x00;
			}
			else if(tmpA == 0x02){//Y
				prog = prog == 0x02 ? 0x03 : 0x00;
			}
			else if(tmpA == 0x04){//#
				prog = prog == 0 ?  0x01 : 0x00;
			}
			else if(tmpA != 0x00){
				prog = 0x00;
			}
			else if(tmpA == 128){
				prog = 0x00;
				tmp =0x00;
			}
			if(prog == 0x04){
				tmp = tmp == 0x00 ? 0x01:0x00;
				prog = 0x00;
			}
			break;
			
			case zero:
			if(tmpA == 0){
				FSM = wait;
			}
			break;
			
			default:
			FSM = wait;
			break;
			
		}
		PORTB = tmp;
	}
}



