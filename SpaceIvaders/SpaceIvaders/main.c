/*
 * SpaceIvaders.c
 *
 * Created: 5/23/2019 8:14:43 AM
 * Author : Chad
 */ 

#include <avr/io.h>
#include <bit.h>
#include <avr/interrupt.h>
#include <timer.h>
#include <stdio.h>
#include <io.h>
#include <io.c> 
#include <SNES.c>
#include <stdlib.h>
#include <customChar.c>
#include <eeprom.c>



#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

typedef struct _enemey {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	unsigned char cur;
	unsigned char type;
} enemey;

unsigned char init = 0;
unsigned char dinoState = 0;
unsigned char dinoC= 0;
unsigned char dinoT= 1;
unsigned char score = 0;
unsigned char cnt = 0;
unsigned char gg = 1;
unsigned char spawnRate = 0;
unsigned char incomeSpeed  = 50;
unsigned char highscore = 0;
enemey enemeys[5];
unsigned char Encnt = 0;
unsigned char Encur = 0;



unsigned char SNESINPUT(){
	unsigned short SNES = SNES_Read();
	unsigned char input = 0x00;
	if(SNES & 16){// RB
		input |= 1;
	}
	if(SNES & 32){//LB
		input |= 2;
	}
	if(SNES & 128){//A
		input |= 4;
	}
	if(SNES & 1024){//DOWN
		input |= 8;
	}
	if(SNES & 2048){//UP
		input |= 16;
	}
	if(SNES & 4096){//START
		input |= 32;
	}
	return input;
}

void draw(){
	
	LCD_ClearScreen();
	
	LCD_Cursor(dinoC);
	LCD_WriteData(dinoT);
	
	for(int i =0; i< 5; i++){
		if(enemeys[i].cur > 0 && enemeys[i].cur < 33){
			LCD_Cursor(enemeys[i].cur);
			LCD_WriteData(enemeys[i].type);
		}
	}
	LCD_Cursor(36);
}

void initSymbol(unsigned char pos, unsigned char* str){
	LCD_WriteCommand (0x40 + (pos*8));
	for(int i = 0; i < 8; i++) {
		LCD_WriteData(str[i]);
	}
	LCD_WriteCommand(0x80);
}

int SMTick1(int state){
	unsigned char input = SNESINPUT();
	unsigned char cursor = 1;
	
	if(input & 1){
		LCD_DisplayString(cursor++ , "R");
	}
	if(input & 2){
		LCD_DisplayString(cursor++ , "L");
	}
	if(input & 4){
		LCD_DisplayString(cursor++ , "A");
	}
	if(input & 8){
		LCD_DisplayString(cursor++ , "D");
	}
	if(input & 16){
		LCD_DisplayString(cursor++ , "U");
	}
	if(input == 0){
		LCD_DisplayString(cursor++ , "0");
	}
	return 1;
}

int Test2(int state){
	LCD_WriteData(1);
	//LCD_WriteData(0);
	LCD_WriteData(3);
	LCD_WriteData(4);
	LCD_WriteData(5);
	LCD_WriteData(6);
	LCD_WriteData(7);
		return 1;
}

int GAME(int state){
	switch(state){
		case 0:
		if(1);
		unsigned char inp = SNESINPUT();
		if(init == 0){
			LCD_ClearScreen();
			LCD_DisplayString(1,"SelectDifficulty LB, RB, A");
			init = 1;
		}
		if(SNESINPUT() == 1){
			init =2;
			state = 1;
			dinoC = 17;
			dinoT= 1;
			incomeSpeed = 30;
		} 
		else if(SNESINPUT() == 2){
			init =2;
			state = 1;
			dinoC = 17;
			dinoT= 1;
			incomeSpeed = 20;
		}
		else if(SNESINPUT() == 4){
			init =2;
			state = 1;
			dinoC = 17;
			dinoT= 1;
			incomeSpeed = 10;
		}
		else if(SNESINPUT() == 19){// L R UP
			score = 99;
		}
		else if(SNESINPUT() == 11){// L R D
			highscore = 0;
			EEPROM_Write(0,highscore);
		}
		break;
		
		case 1:
		if(1);
		unsigned char input = SNESINPUT();
		if(input == 8){
			dinoC = 17;
			dinoT = 6;
		}
		else if(input == 4){
			dinoC = 17;
			dinoT = 7;
			state = 2;
		}
		else if(input == 0){
			dinoC = 17;
			dinoT = 1;	
		}
		break;
		
		case 2:
			dinoC = 1;
			dinoT = 6;
			state = 3;
		break;
		
		case 3:
			dinoC = 1;
			dinoT = 1;
			state = 4;
		break;
		
		case 7:
		dinoC = 1;
		dinoT = 1;
		state = 4;
		break;
		
		case 4:
			dinoC = 1;
			dinoT= 6;
			state = 5;
		break;
		
		case 5:
			dinoC = 17;
			dinoT = 7;
			state = 6;
		break;
		
		case 6:
			dinoC = 17;
			dinoT = 1;
			state = 1;
		break;
		
		default:
		break;
			
				
	}
	if(init == 2){
	draw();
	}
	return state;
}

int INCOMING(int state){
	for(unsigned char i = 0; i< 5; i++){
		if((enemeys[i].cur > 1 && enemeys[i].cur <= 16) || (enemeys[i].cur >17 && enemeys[i].cur <= 32)){
			enemeys[i].cur -=1;
		}
		else if(enemeys[i].cur == 1){
			if(dinoC == 1){
				if(dinoT == 1){
					gg = 0;
				}
				else if(dinoT == 6){
					if(enemeys[i].type == 3){
						score++;
					}
					else {
						gg = 0;
					}
				}else if(dinoT == 7){
					if(enemeys[i].type == 4){
						score++;
					}else{
						gg =0 ;
					}
				}
			}
			else{
				score++;
			}
			enemeys[i].cur = 36;
			Encnt--;
		}
		else if(enemeys[i].cur == 17){
			if(dinoC == 17){
				if(dinoT == 1){
					gg =  0;
				}
				else if(dinoT == 6){
					if(enemeys[i].type == 5 || enemeys[i].type == 3){
						score++;
					}else{
						gg = 0;
					}				
				}else if(dinoT == 7){
					if(enemeys[i].type == 4 || enemeys[i].type == 2){
						score++;
					}	else{
						gg =0;
					}
				}
			}
			else{
				score++;
			}
			enemeys[i].cur = 36;
			Encnt--;
		}
	}
	if(Encnt < 5){
		SPAWN();
	}
	
	return 1;
}

int SPAWN(int state){
	if(init >= 2 && (spawnRate <= cnt && Encnt < 5)){
		//LCD_DisplayString(1, "INCOMING");
		unsigned char tp;
		unsigned char row = rand() % 2;
		if(row){//top
			enemeys[Encur].cur = 16;
			tp = rand() * 5 % 3;
			if(tp == 1){
				enemeys[Encur].type = 4;
			}else if(tp == 2){
				enemeys[Encur].type = 5;
			}else{
				enemeys[Encur].type = 3;
			}
		}else{//bottom
			enemeys[Encur].cur = 32;
			tp = rand() * 7 % 4;
			if(tp == 1){
				enemeys[Encur].type = 3;
			}else if(tp ==2){
				enemeys[Encur].type = 5;
			}else if(tp == 3){
				enemeys[Encur].type = 2;
			}else{
				enemeys[Encur].type = 4;
			}
		}
		
		Encnt++;
		if(Encur >= 4){
			if(incomeSpeed > 5){
				incomeSpeed--;
			}
			Encur = 0;
		}else{
			Encur++;
		}
		spawnRate = (rand() % 5)+1;
		cnt = 0;
	}
	else{
	cnt++;
	}
	//LCD_Cursor(1);
	//LCD_WriteData(spawnRate + '0');
	//LCD_Cursor(2);
	//LCD_WriteData(cnt + '0');
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xFF; PORTC = 0x00; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x03; PORTA = 0xFF;
	
	SNES_init();
	TimerSet(1);
	TimerOn();
	LCD_init();
	
	//initSymbol(0, TALLP1);
	initSymbol(1, TREX);
	initSymbol(2, TALLP2);
	initSymbol(3, ArrowHIGHP1);
	initSymbol(4, ArrowHIGHP2);
	initSymbol(5, ArrowLOW);
	initSymbol(6, CrouchLOW);
	initSymbol(7, CrouchHIGH);
		
	static task test, test2, INIT, INCOM;
	task *tasks[] = {&INIT, &INCOM};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	if(EEPROM_Read(0) == 255){
		EEPROM_Write(0,highscore);
	}
	srand(time());
	while(1){
	
	test.state = 0;//Task initial state.
	test.period = 1;//Task Period.
	test.elapsedTime = 0;//Task current elapsed time.
	test.TickFct = &SMTick1;//Function pointer for the tick.
	
	test2.state = 0;//Task initial state.
	test2.period = 1;//Task Period.
	test2.elapsedTime = 0;//Task current elapsed time.
	test2.TickFct = &Test2;//Function pointer for the tick.

	INIT.state = 0;//Task initial state.
	INIT.period = incomeSpeed;//Task Period.
	INIT.elapsedTime = 0;//Task current elapsed time.
	INIT.TickFct = &GAME;//Function pointer for the tick.
	
	INCOM.state = 0;//Task initial state.
	INCOM.period = incomeSpeed;//Task Period.
	INCOM.elapsedTime = 0;//Task current elapsed time.
	INCOM.TickFct = &INCOMING;//Function pointer for the tick.
	
	//LCD_DisplayString(1,"HELLO WORLD");
	
	for(unsigned char i=0;i<5;i++){
		enemeys[i].cur = 36;
		enemeys[i].type = 2;
	}
	
	
	while(gg) {
		INIT.period = incomeSpeed;
		INCOM.period = incomeSpeed;
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
	LCD_ClearScreen();
	LCD_DisplayString(1, "Your score: ");
	char fin[5];
	sprintf(fin, "%d", score);
	LCD_DisplayString(12, fin);
	LCD_DisplayString(17, "High score:");
	if(score > EEPROM_Read(0)){
		highscore = score;
		EEPROM_Write(0, highscore);
	}
	char hi[5];
	sprintf(hi, "%d", EEPROM_Read(0));
	LCD_DisplayString(28, hi);
	//LCD_Cursor(28);
	//LCD_WriteData(highscore + '0');
	//LCD_Cursor(16);
	//LCD_WriteData(EEPROM_Read(0)+'0');
	while(SNESINPUT() == 0){}
		gg =1;
		score = 0;
		init = 0;
		Encnt =0;
	}

	return 1;
}

