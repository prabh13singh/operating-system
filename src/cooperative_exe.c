//FILE: cooperative_exe.c
//DESCRIPTION: command functions to execute cooperative multitasking
//DATE: 10 April 2017
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
int32_t TaskKill(int32_t id);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);
void templateTask(void* data);

// __attribute__((naked)) static void TaskShell(void);

volatile int rc;
Task_t tasks[MAX_TASKS];


//NAME: CmdTaskAdd
//DESCRIPTION: command to call TaskAdd function
//PARAMETER: mode
//RETURNS: none
void CmdTaskAdd(int mode){
	TaskInit();

	if(mode != CMD_INTERACTIVE) {
    	return;
	}

	TaskAdd(task0, (void*) 0);
	TaskAdd(task1, (void*) 1);
	TaskAdd(task2, (void*) 2);
	TaskAdd(task3, (void*) 3);
	TaskAdd(task4, (void*) 4);

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

	rc = TaskKill((int32_t)id);
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
	TaskSwitcher();	

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





