/*	Author: Kenghao Cai
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #11  
 *	Exercise Description: Conversion game: Random binary number to correct decimal number
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Video Demo: https://youtu.be/v7KP_-BDDy4
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"
#endif

unsigned char gamestart = 0;			//flag so that program knows game has started
unsigned char gamedifficulty = 15;		//global for storing game difficulty
unsigned char difficultySelected = 0;	//flag so program knows difficulty has been chosen
unsigned char blinkCount = 0;			//used for difficulty selector indication
unsigned char inputFinished = 0;		//flag so program knows user has stopped inputing
unsigned char inputdecimalresult = 0;	//global holder for the inputed number in decimal form
unsigned char inputdecimalconverted = 0;	//flag so program knows arr -> deci completed
unsigned char roundNumber = 0;		//number of gaming rounds
unsigned char tenSecond = 0;
unsigned char tsCounter = 0;

unsigned short totalScore = 0;

unsigned int randNum = 0;
unsigned int fixedRandNum = 0;

int inputArr[3];
unsigned int inputArrSize = (sizeof(inputArrSize)/sizeof(inputArr[0]));

enum randNumSM{rnstart, rninit, rnwait};		//display a random number in the proper range
int randNumTick(int state) {
		
	switch (state) {
		case rnstart:
			if (((GetKeypadKey() == '#') || tenSecond) && gamestart) {
				state = rninit ;
			}
			else {
				state = rnstart;
			}
			break;
		case rninit:
			if (GetKeypadKey() == '\0') {
				state = rnwait;
			}
			else {
				state = rninit;
			}
			break;
		case rnwait:
			state = rnstart;
			break;
		default:
			state = rnstart;
			break;
	}
	switch (state) {
		case rnstart:
			
			break;
		case rninit :
			
			break;
		case rnwait:
			fixedRandNum = randNum % gamedifficulty;
			roundNumber++;
			tenSecond = 0;
			tsCounter = 0;
			PORTD = fixedRandNum;
			break;
		default:
			state = rnstart;
			break;
	}
	return state;
}

enum getInputSM {gstart, gwait, getinput, grelease, gfinish};		//get input from keypad
int getInputTick (int gstate) {
	
	unsigned short input = GetKeypadKey();
	
	switch (gstate) {
		case gstart:
			if (gamestart) {
				gstate = gwait;
			}
			else {
				gstate = gstart;
			}
			break;
		case gwait:
			if ((input == 'D') || (inputArrSize >= 3)) {
				gstate = gfinish;
			}
			else if ((input > 47) && (input < 58)) {
				gstate = getinput;
			}
			else {
				gstate = gwait;
			}
			break;
		case getinput:
			gstate = grelease;
			break;
		case grelease:
			if (input == '\0') {
				gstate = gwait;
			}
			else {
				gstate = grelease;
			}
			break;
		case gfinish:
			if (input == '\0') {	
				gstate = gwait;
			}
			else {
				gstate = gfinish;
			}
			break;
		default:
			gstate = gstart;
			break;
	}
	
	switch (gstate) {
		case gstart:
			inputArrSize = 0;
			break;
		case gwait:
			
			break;
		case getinput:
			inputArr[inputArrSize] = (input - 48);
			inputArrSize++;
			inputFinished = 0;
			break;
		case grelease:
			
			break;
		case gfinish:
			inputFinished = 1;
			break;
		default:
			gstate = gstart;
			break;
	}
	return gstate;
}

enum difficultySelectSM {dsstart, dseasy, dsmedium, dshard, dswait};	//difficulty selection
int diffSelectTick (int dsstate) {
	
	switch (dsstate) {
		case dsstart:
			if (GetKeypadKey() == '\0') {
				dsstate = dsstart;
			}
			else if (GetKeypadKey() == 'A') {
				dsstate = dseasy;
				difficultySelected = 1;
			}
			else if (GetKeypadKey() == 'B') {
				dsstate = dsmedium;
				difficultySelected = 1;
			}
			else if (GetKeypadKey() == 'C') {
				dsstate = dshard;
				difficultySelected = 1;
			}
			break;
		case dseasy:
			dsstate = dswait;
			break;
		case dsmedium:
			dsstate = dswait;
			break;
		case dshard:
			dsstate = dswait;
			break;
		case dswait:
			if (GetKeypadKey() == '\0') {
				dsstate = dsstart;
			}
			else {
				dsstate = dswait;
			}
			break;
		default:
			dsstate = dsstart;
			break;
	}
	
	switch (dsstate) {
		case dsstart:
			
			break;
		case dseasy:
			gamedifficulty = 15;
			break;
		case dsmedium:
			gamedifficulty = 63;
			break;
		case dshard:
			gamedifficulty = 255;
			break;
		case dswait:
			
			break;
		default:
			dsstate = dsstart;
			break;
	}
	
	return dsstate;
}

enum blinkSM {bstart, boff, blink, bfinish};		//blink LEDs when difficulty is selected
int blinkTick (int bstate) {
	
	switch (bstate) {
		case bstart:
			if (difficultySelected) {
				bstate = blink;
			}
			else {
				bstate = bstart;
			}
			break;
		case boff:
			bstate = blink;
			break;
		case blink:
			if (blinkCount > 0) {
				bstate = boff;
			}
			else {
				bstate = bfinish;
			}
			break;
		case bfinish:
			bstate = bstart;
			break;
		default:
			bstate = bstart;
			break;
	}
	
	switch (bstate) {
		case bstart:
			blinkCount = 3;
			break;
		case boff:
			PORTD = 0;
			break;
		case blink:
			PORTD = gamedifficulty;
			blinkCount--;
			break;
		case bfinish:
			PORTD = 0;
			difficultySelected = 0;
			break;
		default:
			bstate = bstart;
			break;
	}
	
	return bstate;
}

enum arrToDeciSM {atstart, atwait, onedigit, twodigit, threedigit};	//transform input arr to dec
int arrToDeciTick (int atstate) {
	switch (atstate) {
		case atstart:
			if (inputFinished) {
				atstate = atwait;
			}
			else {
				atstate = atstart;
			}
			break;
		case atwait:
			if (inputArrSize == 1) {
				atstate = onedigit;
			}
			else if (inputArrSize == 2) {
				atstate = twodigit;
			}
			else if (inputArrSize == 3) {
				atstate = threedigit;
			}
			break;
		case onedigit:
			atstate = atstart;
			break;
		case twodigit:
			atstate = atstart;
			break;
		case threedigit:
			atstate = atstart;
			break;
		default:
			atstate = atstart;
			break;
	}
	
	switch (atstate) {
		case atstart:
			
			break;
		case atwait:
			
			break;
		case onedigit:	
			inputdecimalresult = inputArr[0];
			if (inputdecimalconverted && inputFinished) {
				PORTD = inputdecimalresult;
			}
			//inputFinished = 0;
			inputdecimalconverted = 1;
			break;
		case twodigit:
			inputdecimalresult = (inputArr[0] * 10) + inputArr[1];
			if (inputdecimalconverted && inputFinished) {
				PORTD = inputdecimalresult;
			}
			//inputFinished = 0;
			inputdecimalconverted = 1;
			break;
		case threedigit:
			inputdecimalresult = (inputArr[0] * 100) + (inputArr[1] * 10) + inputArr[2];
			if (inputdecimalconverted && inputFinished) {
				PORTD = inputdecimalresult;
			}
			//inputFinished = 0;
			inputdecimalconverted = 1;
			break;
		default:
			atstate = atstart;
			break;
	}
	
	return atstate;
}

enum deciCheckSM {dcstart, dccompare, dccorrect, dcwrong, dcgameover};	//compare what is being displayed to input
int deciCheckTick (int dcstate) {
	
	switch (dcstate) {
		case dcstart:
			if (inputdecimalconverted && inputFinished) {
				dcstate = dccompare;
			}
			else if (roundNumber > 5) {
				dcstate = dcgameover;
			}
			else {
				dcstate = dcstart;
			}
			break;
		case dccompare:
			if (fixedRandNum == inputdecimalresult) {
				dcstate = dccorrect;
			}
			else {
				dcstate = dcwrong;
			}
			break;
		case dccorrect:
			if (roundNumber < 6) {
				dcstate = dcstart;
			}
			else {
				dcstate = dcgameover;
			}
			break;
		case dcwrong:
			if (roundNumber < 6) {
				dcstate = dcstart;
			}
			else {
				dcstate = dcgameover;
			}
			break;
		case dcgameover:
			dcstate = dcstart;
			break;
		default:
			dcstate = dcstart;
			break;
	}
	
	switch (dcstate) {
		case dcstart:
			PORTA = 0;
			break;
		case dccompare:
			
			break;
		case dccorrect:
			inputdecimalconverted = 0;
			totalScore += gamedifficulty;
			if (gamedifficulty == 15) {
				PORTA = 1;
				//PORTA = 0;
			}
			else if (gamedifficulty == 63) {
				PORTA = 2;
				//PORTA = 0;
			}
			else if (gamedifficulty == 255) {
				PORTA = 4;
				//PORTA = 0;
			}
			inputArrSize = 0;
			inputFinished = 0;
			
			PORTB = PORTB & 0xDF;
			break;
		case dcwrong:
			inputdecimalconverted = 0;
			inputArrSize = 0;
			inputFinished = 0;

			PORTB = PORTB & 0xEF;
			break;
		case dcgameover:
			//display total score
			PORTD = totalScore;
			totalScore = 0;
			roundNumber = 0;
			gamestart = 0;
			PORTA = 0;
			break;
		default:
			dcstate = dcstart;
			break;
	}
	
	return dcstate;
}

enum tenSecondSM {tsstart, tenSec, tsreset};
int tenSecondTick (int tsstate) {
	
	switch (tsstate) {
		case tsstart:
			if (tsCounter < 10) {
				tsstate = tsstart;
			}
			else if ((tsCounter > 9) && gamestart) {
				tsstate = tenSec;
			}
			else {
				tsstate = tsreset;
			}
			break;
		case tenSec:
			tsstate = tsreset;
			break;
		case tsreset:
			tsstate = tsstart;
			break;
		default:
			tsstate = tsstart;
			break;
	}
	
	switch (tsstate) {
		case tsstart:
			tsCounter++;
			break;
		case tenSec:
			tenSecond = 1;
			break;
		case tsreset:
			tsCounter = 0;
			break;
		default:
			tsstate = tsstart;
			break;
	}
	return tsstate;
}

enum startgameSM {sgstart, sggamestart, sggamepause};
int startgameTick (int sgstate) {
	switch (sgstate) {
		case sgstart:
			if ((GetKeypadKey() == '*') && gamestart) {
				sgstate = sggamepause;
			}
			else if ((GetKeypadKey() == '*') && !gamestart) {
				sgstate = sggamestart;
			}
			else {
				sgstate = sgstart;
			}
			break;
		case sggamestart:
			if (GetKeypadKey() == '\0') {
				sgstate = sgstart;
			}
			else {
				sgstate = sggamestart;
			}
			break;
		case sggamepause:
			if (GetKeypadKey() == '\0') {
				sgstate = sgstart;
			}
			else {
				sgstate = sggamepause;
			}
			break;
		default:
			sgstate = sgstart;
			break;
	}
	
	switch (sgstate) {
		case sgstart:
			
			break;
		case sggamestart:
			gamestart = 1;
			tsCounter = 0;
			PORTD = 0;
			break;
		case sggamepause:
			gamestart = 0;
			break;
		default:
			sgstate = sgstart;
			break;
	}
	return sgstate;
}


int main(void) {
    //output	 //input
    DDRA = 0xFF; PORTA = 0x00; // Configure port A's 8 pins as inputs
    DDRB = 0x7F; PORTB = 0x80;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00; 

	static task task1, task2, task3, task4, task5, task6, task7, task8;
	task *tasks[] = {&task1, &task2, &task3, &task4, &task5, &task6, &task7, &task8};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	const char start = -1;
	
	task1.state = start; //initial state
	task1.period = 10;
	task1.elapsedTime = task1.period;
	task1.TickFct = &randNumTick;
	
	task2.state = start;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &getInputTick;
	
	task3.state = start;
	task3.period = 100;
	task3.elapsedTime = task3.period;
	task3.TickFct = &diffSelectTick;
	
	task4.state = start;
	task4.period = 150;
	task4.elapsedTime = task4.period;
	task4.TickFct = &blinkTick;
	
	task5.state = start;
	task5.period = 50;
	task5.elapsedTime = task5.period;
	task5.TickFct = &arrToDeciTick;
	
	task6.state = start;
	task6.period = 50;
	task6.elapsedTime = task6.period;
	task6.TickFct = &deciCheckTick;
	
	task7.state = start;
	task7.period = 1000;
	task7.elapsedTime = task7.period;
	task7.TickFct = &tenSecondTick;
	
	task8.state = start;
	task8.period = 100;
	task8.elapsedTime = task8.period;
	task8.TickFct = &startgameTick;
	
	unsigned int i = 0;
	unsigned long GCD = tasks[0]->period;
	for (i = 1; i < numTasks; i++) {
		GCD = findGCD(GCD,tasks[i]->period);
	}
	
	TimerSet(GCD);
	TimerOn();
	
	
while(1) { 	
	randNum++;
	for (i = 0; i < numTasks; i++) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += GCD;
		
    }
    PORTB = roundNumber | (gamestart << 3);
    while (!TimerFlag);
    TimerFlag = 0;
    
}

    return 0;
}


