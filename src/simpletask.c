//FILE: simpletask.c
//DESCRIPTION: TERMINAL COUNTER
//DATE: 30 JAN 2017
//PROGRAMMER: PUSKAR PANDEY

#include <stdio.h>
#include "stm32f3xx_hal.h"
#include "common.h"

static uint32_t Counter;
static uint32_t savedCounter; //holds previous counter value
//NAME: TaskCounter
//DESCRIPTION: prints counter value and reduce the counter by 1.
//PARAMETER: none
//RETURNS: none

void TaskCounter(){
	if (Counter != 0){
		printf("The Counter is : %u\n", (unsigned int)Counter);
		Counter -= 1;
	}
	
}


//NAME: CmdCount
//DESCRIPTION:initiates the counter 
//PARAMETER: mode
//RETURNS: none
void CmdCount(int mode){
	int32_t val = 0; //variable to hold fetched counter value from user
	if(mode != CMD_INTERACTIVE) {
    		return;
  	}
	fetch_int32_arg(&val);
	if (val <= 0 || val > 100){
		printf("Enter the count value greater than O and less than 100 \n");	
		Counter = 0;
		return;
	}
	else { 
		savedCounter += val;
		Counter = savedCounter;
	}
}

ADD_CMD("count", CmdCount,"                Sets the counter value")

//NAME: resetCount
//DESCRIPTION:resets counter 
//PARAMETER: mode
//RETURNS: none
void resetCount(int mode){
	if(mode != CMD_INTERACTIVE) {
    		return;
  	}
	savedCounter = 0;
}
ADD_CMD("reset-count", resetCount,"                Resets counter value to 0")
