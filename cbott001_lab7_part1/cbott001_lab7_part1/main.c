/*
 * cbott001_lab7_part1.c
 *
 * Created: 4/26/2019 6:48:32 PM
 * Author : Chad
 **/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
#include "io.h"

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;
void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
int main(void)
{
     //Replace with your application code 
			   DDRC = 0xFF; PORTC = 0x00; // LCD data lines
			   DDRD = 0xFF; PORTD = 0x00; // LCD control lines
			   	DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
			   	DDRB = 0xFF; PORTB = 0x00; // Configure PORTB as outputs, initialize to 0s
			   	unsigned char led = 7;
			   	unsigned char button = 0x00;
			   	unsigned char b2 = 0x00;
			   	unsigned char state =0x00;
    while (1) 
    {

		   
		   // Initializes the LCD display
		   LCD_init();
		   TimerSet(10000);
		   button = ~PINA & 0x01; // button is connected to A0
		   b2 = ~PINA & 0x02;
		   b2 = b2 >> 1;
		   switch(state){
			   case 0:
			   if(button && b2){
				   led = 0;
				   state = 1;
				}else if(button){
				   if(led <9)
					led = led +1;
				   }else if(b2){
				   if(led > 0 )
				   led = led -1;
				   while(TimerFlag);
				   if(button)
					state =1;
			   }
			   break;
			   case 1:
				   if(button == b2){
					   state =0;
				    }
					
					break;
			   default:
					state =0;
					break;
		   }
		   
		   
		   
		   // 3) Write output
		   PORTB = led;
		   //PORTC = led;
		   // Starting at position 1 on the LCD screen, writes Hello World
		 
		   LCD_WriteData(led + '0');
    }
}

