//FILE: cooperative.c
//DESCRIPTION: functions to execute concept of cooperative multitasking
//DATE: 10 April 2017
//PROGRAMMER: PUSKAR PANDEY

#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include"common.h"
#include"stm32f3xx_hal.h"

//#define MAX_TASKS       6
#define TASK_STACK_SIZE 0x100



/* Space for process stacks. */
uint32_t stacks[MAX_TASKS][TASK_STACK_SIZE];
typedef struct PCB_s {
	/* Storage for r4-r14 */
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t sl;
	uint32_t fp;
	uint32_t ip;
	uint32_t lr;
	uint32_t sp; /* Note location...*/
} PCB_t;

typedef struct Task_s {
	void (*f)(void *data); /* Task function */
	void *data; /* Private data pointerfor this task */
	uint32_t *stack; /* Pointer to top oftask's stack */
	PCB_t pcb; /* Process control block */
} Task_t;

static int32_t TaskNext(void);
__attribute__((naked)) static void TaskShell(void);
//void templateTask(void* data);

int32_t currentTask = 0;
volatile int rc;
/* Space for process stacks. */
uint32_t stacks[MAX_TASKS][TASK_STACK_SIZE];

Task_t tasks[MAX_TASKS];

const uint32_t LED[] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};


void TaskNull(void* data)
{
	while(1)
		{
		}
	return;
}

int32_t TaskSwitcher(void)
{
	int32_t nextTask;
	register uint32_t *currentPCB asm("r0");
	register uint32_t *nextPCB asm("r1");
	//printf("Task switcher called\n\n");
	nextTask = TaskNext();
	if(nextTask < 0)
	{
		/* In the case of no tasks to run,return to the original thread */
		printf("TaskSwitcher(): No tasks to run!\n");
		nextTask = 0;
		return -1;
	}
	/* If the current task is the only one  to be running, just return */
	if(nextTask == currentTask){
		return 0;
	}
	currentPCB = &(tasks[currentTask].pcb.r4);
	nextPCB = &(tasks[nextTask].pcb.r4);
	currentTask = nextTask;
	asm volatile (
		"stm %[current]!,{r4-r12,r14}\n\t"
		"str sp, [%[current]]\n\t"
		"ldm %[next]!,{r4-r12,r14}\n\t"
		"ldr sp, [%[next]]\n\t"
		: /* No Outputs */
		: [current] "r" (currentPCB), [next] "r"
		(nextPCB) /* Inputs */: /* 'No' Clobbers */);
		/* We are now on the other context */
	return 0;
}


//NAME: TaskKill
//DESCRIPTION:  Kill the task with given id
//PARAMETER: id of the task
//RETURNS: returns task id that is killed or else return -1 if given id is invalid
int32_t TaskKill(int32_t id){
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
	 /* Fill in initial PCB */
	 for( int i=0; i<MAX_TASKS; i++) 
	 {
		 memset(&(tasks[i].pcb),0,sizeof(tasks[i].pcb));
		 tasks[i].pcb.sp =(uint32_t)(&(stacks[i][TASK_STACK_SIZE-4]));
		 tasks[i].pcb.fp =(uint32_t)(&(stacks[i][TASK_STACK_SIZE-4]));
		 tasks[i].pcb.lr =(uint32_t)TaskShell;
		 /* Put an initial stack frame on too */
		 stacks[i][TASK_STACK_SIZE-1]=(uint32_t)(TaskShell);
		 if(tasks[i].f == NULL)
		 {
			tasks[i].f = f;
			tasks[i].data = data;
			tasks[i].stack = stacks[i];
			//printf(" Task:  %d added\n", i);
		 	return i;
		 }
	 }
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
	printf("\n");
	do { 
		i = ((i) % (MAX_TASKS - 1))+1;
		count++;
	} while((tasks[i].f == NULL) && (count <= MAX_TASKS));

	return (count <= MAX_TASKS) ? i : -1;
}


__attribute__((naked)) static void TaskShell(void)
{
	/* call the appropriate Task Function */
	tasks[currentTask].f(tasks[currentTask].data);
	/* Terminate this task */
	TaskKill(currentTask);
	/* Call scheduler, Never returns */
	TaskSwitcher();
}


//NAME: task0
//DESCRIPTION: toggles led
//PARAMETER: void type data
//RETURNS: none
void task0(void *data){
	for(int i = 0; i < 10; i++){
		printf("Task: 0\n");
		HAL_GPIO_WritePin(GPIOE, LED[0], 1);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOE, LED[0], 0);
		TaskSwitcher();
	}
	return;
}

//NAME: task1
//DESCRIPTION: toggles led
//PARAMETER: void type data
//RETURNS: none
void task1(void *data){
	for(int i = 0; i < 10; i++){
		printf("Task: 1\n");
		HAL_GPIO_WritePin(GPIOE, LED[1], 1);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOE, LED[1], 0);
		TaskSwitcher();	
	}
	return;

}

//NAME: task2
//DESCRIPTION: toggles led
//PARAMETER: void type data
//RETURNS: none
void task2(void *data){
	for(int i = 0; i < 10; i++){
		printf("Task: 2\n");
		HAL_GPIO_WritePin(GPIOE, LED[2], 1);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOE, LED[2], 0);
		TaskSwitcher();
	}
	return;

}

//NAME: task3
//DESCRIPTION: toggles led
//PARAMETER: void type data
//RETURNS: none
void task3(void *data){
	for(int i = 0; i < 10; i++){
		printf("Task: 3\n");
		HAL_GPIO_WritePin(GPIOE, LED[4], 1);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOE, LED[4], 0);
		TaskSwitcher();
	}
	return;
}

//NAME: task4
//DESCRIPTION: Runs dcmotor
//PARAMETER: void type data
//RETURNS: none
void task4(void *data){
	for(int i = 0; i < 10; i++){
		printf("Task: 4\n");
		dcRun();
		HAL_Delay(500);
		//dcStop();
		TaskSwitcher();
	}
	return;
}


//NAME: TaskCurrent
//DESCRIPTION: return the id of current task
//PARAMETER: none
//RETURNS: position of currnet task to be executed
int32_t TaskCurrent(){
	return currentTask;
}

//NAME: TaskInit 
//DESCRIPTION: Initializes the stack header
//PARAMETER: none
//RETURNS: none
void TaskInit(void)
{
	for(int x = 0 ; x<MAX_TASKS ; x++)
	{
		tasks[x].f    = NULL;               //assign NULL the variable
		tasks[x].data = NULL;               //assign NULL to the variable
	}

	//adding TaskNull function as an initial task in the task array
	int i =0;
	memset(&(tasks[i].pcb),0,sizeof(tasks[i].pcb));
 	tasks[i].pcb.sp =(uint32_t)(&(stacks[i][TASK_STACK_SIZE-4]));
 	tasks[i].pcb.fp =(uint32_t)(&(stacks[i][TASK_STACK_SIZE-4]));
 	tasks[i].pcb.lr =(uint32_t)TaskShell;
 	/* Put an initial stack frame on too */
 	stacks[i][TASK_STACK_SIZE-1]=(uint32_t)(TaskShell);
 	if(tasks[i].f == NULL)
 	{
 		tasks[i].f = TaskNull;
 		tasks[i].data = NULL;
 		tasks[i].stack = stacks[i];
 
 	}
	currentTask = 0;
}

