////////////////////////////////////////////////////////////////////////////////
//	File Name					: piezo.c
//	Description				: Code to use the key pad
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 17th, 2016
////////////////////////////////////////////////////////////////////////////////

#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <timer.h>
#include <piezo.h>
#include <7_segment_display.h>

TIM_HandleTypeDef tim_handle;
TIM_HandleTypeDef poll_handle;

void timer_init(void){
	__TIM3_CLK_ENABLE();
	tim_handle.Instance = TIM3;
	tim_handle.Init.ClockDivision = 0;
	tim_handle.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	tim_handle.Init.Period = 100;
	tim_handle.Init.Prescaler = 50; //prescalar is the counter in a counter
	tim_handle.Init.RepetitionCounter = 0;

	if(HAL_TIM_Base_Init(&tim_handle)) Error_Handler(TIM_INIT_FAIL);  
	
	HAL_TIM_Base_Start_IT(&tim_handle);
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn); //enable interrupt
	HAL_NVIC_SetPriority(TIM3_IRQn,1,0); //set priority
	
	init_Display_Pins();
}


