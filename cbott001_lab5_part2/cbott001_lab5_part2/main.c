/*
 * cbott001_lab5_part2.c
 *
 * Created: 4/16/2019 9:29:45 AM
 * Author : Chad
 */ 
#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;


unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {

	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void)
{
	unsigned char outputNum = 0;
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(1000);
	TimerOn();
	

	LCD_init();

	LCD_WriteData('0');

	
	while(1){
		if((~PINA & 0x01) && (~PINA & 0x02)){
			outputNum = 0;
		}
		else if((~PINA & 0x01) && (outputNum < 9)){
			outputNum++;
		}
		else if((~PINA & 0x02) && (outputNum > 0)){
			outputNum--;
		}
		
		
		while(!TimerFlag);
		TimerFlag = 0;
		
		LCD_Cursor(1);
		LCD_WriteData('0' + outputNum);
	}
}


