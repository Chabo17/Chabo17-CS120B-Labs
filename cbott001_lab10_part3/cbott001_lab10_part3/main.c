/*
 * cbott001_lab10_part3.c
 *
 * Created: 5/9/2019 8:17:20 AM
 * Author : Chad
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
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

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
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

unsigned char c1 = 0;
unsigned char c2 = 0;
unsigned char c3 = 0;


unsigned char tick1(unsigned char FSM){
	c1 = c1 + 1;
	if(c1 >= 30){
		if(FSM == 0){
			FSM = 1;
		}
		else if(FSM == 1){
			FSM = 2;
		}
		else if(FSM == 2){
			FSM = 4;	
		}
		else if(FSM == 4){
			FSM = 0;
		}
		c1 = 0;
	}
	return FSM;
}

unsigned char tick2(unsigned char FSM){
	c2 = c2 +1;
	if(c2 >= 100){
		c2 = 0;
		FSM = ~FSM & 0x08;
	}
	return FSM;
	
}

unsigned char tick3(unsigned char FSM){
	if((~PINA & 0x01) == 0x01){
		c3 = c3 + 1;
		if(c3 >= 2){
			FSM = ~FSM & 0x10;
			c3 = 0;
		}
		
	}
	return FSM;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char FSM1 = 0;
	unsigned char FSM2 = 0;
	unsigned char FSM3 = 0;
	TimerOn();
	TimerSet(1);
	
    while (1) 
    {
		while(!TimerFlag){}
		FSM1 = tick1(FSM1);
		FSM2 = tick2(FSM2);
		FSM3 = tick3(FSM3);
		unsigned char test = 0x00;
		if((~PINA & 0x01) == 0x01){
			test = 0x20;
		}
		PORTB =FSM2 | FSM1 | FSM3 | test;
		TimerFlag = 0;
    }
}



