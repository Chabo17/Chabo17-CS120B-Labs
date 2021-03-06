/*
 * cbott001_lab9_part2.c
 *
 * Created: 5/2/2019 9:07:16 AM
 * Author : Chad
 */ 


#include <avr/io.h>

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	float notes[8] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};
	unsigned char b = ~PINA;
    /* Replace with your application code */
	unsigned char i=3;
	unsigned char FSM = 1;
	unsigned char pow = 0x00;
	//PWM_on();
    while (1) 
    {
		switch(FSM){
			case 1:
			if(~PINA & 0x0F){
				b = ~PINA & 0x0F;
				FSM = 2;
			}
			break;
			case 2:
			if((~PINA & 0x0F) == 0x00){
				FSM = 3;
			}
			break;
			case 3:
				if((b & 0x01) == 0x01){
					if(pow == 1){
						PWM_off();
						pow = 0;
					}
					else{
						PWM_on();
						
						pow = 1;
					}
				} else if((b & 0x02) == 0x02){
					if(i<7){
						i=i+1;
					}
				}
				else if((b & 0x04) == 0x04){
					if(i>0){
						i=i-1;
					}
				}
				else{
					PWM_off();
				}
				FSM = 1;
				break;
			
		}		
		//PWM_off();
		set_PWM(notes[i]);
		PORTD = i;
    }
}

