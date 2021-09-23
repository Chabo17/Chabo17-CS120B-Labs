#include <avr/io.h>
#include <bit.h>
#include <avr/interrupt.h>
#include <timer.h>
#include <stdio.h>


typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

unsigned char GetKeypadKey() {

	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('2'); }
	// ... *****FINISH*****

	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	// ... *****FINISH*****

	// Check keys in col 4
	// ... *****FINISH*****

	return('\0'); // default value

}

int SMTick1(int state){
			unsigned char x = GetKeypadKey();
			switch (x) {
				case '\0': PORTB = 0x1F; break; // All 5 LEDs on
				case '1': PORTB = 0x01; break; // hex equivalent
				case '2': PORTB = 0x02; break;
				case '3': PORTB = 3; break;
				case '4': PORTB = 4; break;
				case '5': PORTB = 5; break;
				case '6': PORTB = 6; break;
				case '7': PORTB = 7; break;
				case '8': PORTB = 8; break;
				case '9': PORTB = 9; break;
				case 'A': PORTB = 10; break;
				case 'B': PORTB = 11; break;
				case 'C': PORTB = 12; break;
				case 'D': PORTB = 0x0D; break;
				case '*': PORTB = 0x0E; break;
				case '0': PORTB = 0x00; break;
				case '#': PORTB = 0x0F; break;
				default: PORTB = 0x1B; break; // Should never occur. Middle LED off.
			}
			return 0;
}
// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/


int main(void)
{
	
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	task1.state = -1;//Task initial state.
	task1.period = 1;//Task Period.
	task1.elapsedTime = 0;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.
	
	while(1) {
		while(1) {
			// Scheduler code
			unsigned char i;
			for ( i = 0; i < numTasks; i++ ) {
				// Task is ready to tick
				if ( tasks[i]->elapsedTime == tasks[i]->period ) {
					// Setting next state for task
					tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
					// Reset the elapsed time for next tick.
					tasks[i]->elapsedTime = 0;
				}
				tasks[i]->elapsedTime += 1;
			}
			while(!TimerFlag);
			TimerFlag = 0;
		}

	}
}
