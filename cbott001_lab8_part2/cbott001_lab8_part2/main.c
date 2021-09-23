/*
 * cbott001_lab8_part2.c
 *
 * Created: 5/1/2019 9:37:18 PM
 * Author : Chad
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


int main(void)
{
	/* Replace with your application code */
	ADC_init();
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTB = 0x00;
	while (1)
	{
		unsigned short val = ADC; // 0xABCD
		unsigned char B = val & 0xFF;
		PORTB = B;
		unsigned char D = val >> 8;
		PORTD = D;
		
	}
}
