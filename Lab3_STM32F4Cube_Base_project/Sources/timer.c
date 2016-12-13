////////////////////////////////////////////////////////////////////////////////
//	File Name					: timer.c
//	Description				: initialize timers
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 24th, 2016
////////////////////////////////////////////////////////////////////////////////

#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <timer.h>
#include <piezo.h>
#include <7_segment_display.h>
#include <key_pad.h>
#include <piezo.h>

TIM_HandleTypeDef tim_handle3;
TIM_HandleTypeDef tim_handle2;
TIM_HandleTypeDef tim_handle4;

//timer for the 7 segment
void timer_init3(void){
	__TIM3_CLK_ENABLE();
	tim_handle3.Instance = TIM3;
	tim_handle3.Init.ClockDivision = 0;
	tim_handle3.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	tim_handle3.Init.Period = 100;
	tim_handle3.Init.Prescaler = 50; //prescalar is the counter in a counter
	tim_handle3.Init.RepetitionCounter = 0;

	if(HAL_TIM_Base_Init(&tim_handle3)) Error_Handler(TIM_INIT_FAIL);  
	
	HAL_TIM_Base_Start_IT(&tim_handle3);
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn); //enable interrupt
	HAL_NVIC_SetPriority(TIM3_IRQn,1,0); //set priority
	
	init_Display_Pins();
}
//timer for the keypad
void timer_init2(void){
	__TIM2_CLK_ENABLE();
	tim_handle2.Instance = TIM2;
	tim_handle2.Init.ClockDivision = 0;
	tim_handle2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	tim_handle2.Init.Period = 6699;
	tim_handle2.Init.Prescaler = 5699; //prescalar is the counter in a counter
	tim_handle2.Init.RepetitionCounter = 0;

	if(HAL_TIM_Base_Init(&tim_handle2)) Error_Handler(TIM_INIT_FAIL);  
	
	HAL_TIM_Base_Start_IT(&tim_handle2);
	
	HAL_NVIC_EnableIRQ(TIM2_IRQn); //enable interrupt
	HAL_NVIC_SetPriority(TIM2_IRQn,1,3); //set priority
	keypad_init();

}
//timer for the piezo
void timer_init4(void){
	__TIM4_CLK_ENABLE();
	tim_handle4.Instance = TIM4;
	tim_handle4.Init.ClockDivision = 0;
	tim_handle4.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	tim_handle4.Init.Period = 100;
	tim_handle4.Init.Prescaler = 50; //prescalar is the counter in a counter
	tim_handle4.Init.RepetitionCounter = 0;

	if(HAL_TIM_Base_Init(&tim_handle4)) Error_Handler(TIM_INIT_FAIL);  
	
	HAL_TIM_Base_Start_IT(&tim_handle4);
	
	HAL_NVIC_EnableIRQ(TIM4_IRQn); //enable interrupt
	HAL_NVIC_SetPriority(TIM4_IRQn,1,2); //set priority
	
	adc_piezo_init();
}

