/*
 * cbott001_lab8_part4.c
 *
 * Created: 5/1/2019 9:59:14 PM
 * Author : Chad
 */ 

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
		unsigned char itor = 0xE0/8;
		unsigned char B = 0x01;
		unsigned char temp = 0x00;
		for(unsigned char i = 0; i< 8; i++){
			if(val >= (itor*i)){
				temp =B;
			}
			B = B * 2;
		}
		PORTB = temp;
	}
}
