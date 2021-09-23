/*
 * cbott001_lab8_part3.c
 *
 * Created: 5/1/2019 9:41:51 PM
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
		if(val < (0xE0)/2){
			PORTB = 0;
			PORTD = 0x00;
		}else {
			PORTD = 0xFF;
			PORTB = 0xFF;
		}
	}
}
