//FILE: dc_motor.c
//DESCRIPTION: command functions to execute cooperative multitasking
//DATE: 10 April 2017
//PROGRAMMER: PUSKAR PANDEY

#include <stdio.h>
#include <stdlib.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h" 
#include "common.h"


GPIO_InitTypeDef  GPIO_InitStruct;
TIM_HandleTypeDef tim15;
HAL_StatusTypeDef rc;

// Name: DcMotorInit
// Description: Initializes peripheral for dc motor
// Parameters: mode
// Return: None
void DcMotorInit(int mode){
    if(mode != CMD_INTERACTIVE) {
      return;
      }

	 __GPIOF_CLK_ENABLE();
    /* Configure GPIO pins */
    GPIO_InitStruct.Pin = (GPIO_PIN_10);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // for alternate function
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 3;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    __GPIOA_CLK_ENABLE();
    /*Configure GPIO pins for PF4*/
    GPIO_InitStruct.Pin = (GPIO_PIN_2 | GPIO_PIN_3);
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Initialize PWM */
     __TIM15_CLK_ENABLE();
    tim15.Instance = TIM15; 
    tim15.Init.Prescaler = 72;
    tim15.Init.CounterMode   = TIM_COUNTERMODE_UP; 
    tim15.Init.Period        = 1000; 
    tim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
    tim15.Init.RepetitionCounter = 0; 

    /*Initalize the Timer*/
    rc = HAL_TIM_Base_Init(&tim15);
    if(rc != HAL_OK) 
     {
      printf("Unable to initalize Timer, rc=%d\n",(unsigned)rc);
      return;
     }

    /*Start the timer*/
     rc = HAL_TIM_Base_Start(&tim15);
     if(rc != HAL_OK) 
     {
      printf("Unable to start the timer, rc=%d\n",(unsigned)rc);
      return;
     }

    /*Configure output:*/
    TIM_OC_InitTypeDef sConfig; 
    sConfig.OCMode       = TIM_OCMODE_PWM1; 
    sConfig.Pulse        = 500; 
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH; 
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_LOW; 
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE; 
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET; 
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET; 
    HAL_TIM_PWM_ConfigChannel(&tim15,&sConfig,TIM_CHANNEL_2);

    return;
}
ADD_CMD("dcinit",DcMotorInit," .............................Initialize DC motor")


// Name: dcRun
// Description: starts the dc motor
// Parameters: mode
// Return: None
void dcRun(){
    TIM15->CCR2= 500;    
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET); //enable the motor
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET); //disable the motor
    HAL_TIM_PWM_Start(&tim15,TIM_CHANNEL_2); 
    return;
}


// Name: dcStop
// Description: stops dc motor
// Parameters: None
// Return: None
void dcStop(){
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET); //Disable the motor
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET); //disable the motor
    HAL_TIM_PWM_Stop(&tim15,TIM_CHANNEL_2);
    return;
}


