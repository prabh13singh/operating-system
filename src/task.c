//FILE: task.c
//DESCRIPTION: Task Executive
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
 
extern int currentTask;
int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskKill(int id);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);
static int32_t TaskNext(void);
volatile int rc;
Task_t tasks[MAX_TASKS];

const uint32_t LED[] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

void display(void* data); 



//NAME: TaskKill
//DESCRIPTION:  Kill the task with given id
//PARAMETER: id of the task
//RETURNS: returns task id that is killed or else return -1 if given id is invalid
int32_t TaskKill(int id){
	if(tasks[id].f != NULL){
		tasks[id].f = NULL;    //add test conditions here
		tasks[id].data = NULL;
		currentTask--;
		return id ;
	}
	else{
	 	return -1;
	} 
}


//NAME: TaskAdd
//DESCRIPTION:  Add new task
//PARAMETER: add new function and data int the task array
//RETURNS: position of current task
int32_t TaskAdd(void (*f)(void *data), void *data)
{

	int i;
	/* Try to find an empty slot */
	for(i=0; i<MAX_TASKS; i++) {
		if(tasks[i].f == NULL) {
			tasks[i].f = f;
			tasks[i].data = data;
			currentTask = i;
			return i;
		}
	}
	/* No slots available, return -1 */
	return -1;
}


//NAME: TaskNext
//DESCRIPTION: find next task in a loop 
//PARAMETER: none
//RETURNS: position of next task to be executed
static int32_t TaskNext(void){
	int32_t i;
	uint32_t count=0;
	i = currentTask;
	do {
		i = (i + 1) % MAX_TASKS;
		count++;
	} while((tasks[i].f == NULL) && (count <= MAX_TASKS));

	return (count <= MAX_TASKS) ? i : -1;
}


//NAME: TaskCurrent
//DESCRIPTION: return the id of current task
//PARAMETER: none
//RETURNS: position of currnet task to be executed
int32_t TaskCurrent(){
	return currentTask;
}


//NAME: TaskSwitcher
//DESCRIPTION: switches between the taskes in loop starting from current task 
//PARAMETER: none
//RETURNS: position of current task
int32_t TaskSwitcher(){
	currentTask = TaskNext();
	if(currentTask < 0) {
		return -1;
	}
	tasks[currentTask].f(tasks[currentTask].data);
	printf("task switched to %d.\n", currentTask);

	return currentTask;
}


//NAME: display
//DESCRIPTION: void function with same prototype as structre function
//PARAMETER: pointer to a void datatype
//RETURNS: none
void display(void* data){
	int i = *((int*)data);
	printf("Task: %d\n", i);
	HAL_GPIO_WritePin(GPIOE, LED[i], 1);
	HAL_Delay(150);
	HAL_GPIO_WritePin(GPIOE, LED[i], 0);
	return;
}


