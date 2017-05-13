//FILE: task_exe.c
//DESCRIPTION: Task Executive command functions
//DATE: 06 MAR 2017
//PROGRAMMER: PUSKAR PANDEY

#include <stdio.h>
#include <stdlib.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h" 
#include "common.h"

typedef struct Task_s {
	void (*f)(void *data); /* Task function */
	void *data; /* Private data pointer for this task */
} Task_t;
 

int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskKill(int id);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);
void display(void* data);

volatile int rc;
Task_t tasks[MAX_TASKS];
int currentTask = 0;

//NAME: CmdTaskAdd
//DESCRIPTION: command to call TaskAdd function
//PARAMETER: mode
//RETURNS: none
void CmdTaskAdd(int mode){

	if(mode != CMD_INTERACTIVE) {
    return;
	}

	void (*f)(void *data);
	int x = 0;
	void *u;
	int result;

	u = &currentTask;
	f = display;
	result = TaskAdd(f, u);
	if (result == -1){
		printf("No slot avialable\n");
		
	}
	else{
		x=(int)result;
		printf("New task %d is added\n", x);
		x++;
	}
return;

}

ADD_CMD("addtask",CmdTaskAdd," ...........................Add new task")


//NAME: CmdTaskKill
//DESCRIPTION: command to call TaskKill function 
//PARAMETER: mode
//RETURNS: none
void CmdTaskKill(int mode){
	if(mode != CMD_INTERACTIVE) {
    	return;
	}
	int32_t id;
	rc = fetch_int32_arg(&id);

	if(rc){
		printf("Enter task ID to kill\n");
		return;
	}

	rc = TaskKill((int)id);
	if (rc == -1){
		printf("Invalid task id: %d\n", (int)id);
		return;
	}	
}

ADD_CMD("killtask",CmdTaskKill," ...........................Add new task")


//NAME: CmdTaskTest
//DESCRIPTION: command to calls TaskSwitcher to test the task
//PARAMETER: mode
//RETURNS: none
void CmdTaskTest(int mode){
	if(mode != CMD_INTERACTIVE) {
    	return;
	}
	//int counter = 0;
	int switchFlag;
	int i;
	
	printf ("testtask started\n");
	for(i=0; i<SWITCH_TIME; i++){
		switchFlag = TaskSwitcher();	
		if (switchFlag == -1){
			printf("No task to execute. Add new task\n");
			return;
		}
		
	
	}

}
ADD_CMD("testtask",CmdTaskTest," ...........................Test the task")

//NAME: CmdTaskCurrent
//DESCRIPTION: command to calls TaskCurrent to display current task
//PARAMETER: mode
//RETURNS: none
void CmdTaskCurrent(int mode){
	if(mode != CMD_INTERACTIVE) {
    	return;
	}
	int32_t current;
	current = TaskCurrent();
	printf("current task : %ld\n", current);
}
ADD_CMD("currenttask",CmdTaskCurrent," ...........................displays current task")
