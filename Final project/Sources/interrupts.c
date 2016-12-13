/**
  ******************************************************************************
  * File Name					: interrupts.c
	*	Description				: handles TIM and EXTI interrupt service routines
	*	Author						: Stephan Greto-McGrath
	*	Date							: Oct 28th, 2016
  ******************************************************************************
  */
#include <stm32f4xx_hal.h>
#include "acc.h"
#include "interrupts.h"
#include "display.h"
#include "temperature.h"
#include "keypad.h"
#include "mouse_thread.h"
#include "supporting_functions.h"
//#include <control.h>

// flags to set when respective TIM periods interrupt
volatile int keypad_time_elapsed;
volatile int display_time_elapsed;
volatile int temp_time_elapsed;
volatile int control_time_elapsed;
volatile int mouse_time_elapsed;

void tim2_init(void);
void tim3_init(void);
void tim4_init(void);

// interrupt handles
TIM_HandleTypeDef tim_handle_2; // TIM2 initialization
TIM_HandleTypeDef tim_handle_3; // interrupt for keypad is on TIM3
TIM_HandleTypeDef tim_handle_4;

/** 
* @brief callback on TIM handlers
*	@param TIM_HandleTypeDef *htim the handler that is calling back
* @return void
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM3){
			osSignalSet(mouse_thread_ID, 0x00000001);
			mouse_time_elapsed = 1;
			osSignalSet(keypad_thread_ID, 0x00000001);
			keypad_time_elapsed = 1;
	}
	if (htim->Instance == TIM2){
			osSignalSet(display_thread_ID, 0x00000001);
			//osSignalSet(control_thread_ID, 0x00000001);
			
			display_time_elapsed = 1;
			//control_time_elapsed = 1;
	}
	if (htim->Instance == TIM4){
			osSignalSet(temp_thread_ID, 0x00000001);
			temp_time_elapsed = 1;
	}
}
	
/** 
	* @brief callback method from EXTI
	*	@param uint16_t GPIO_Pin the GPIO the callback was coming from
	* @return void
	*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == ACC_PIN){
		osSignalSet(acc_thread_ID, 0x00000001);
		acc_interrupt = 1;
	}
}

void timers_init(int init){
	if(init & 0x01) tim2_init();
	if(init & 0x02) tim3_init();
	if(init & 0x04)	tim4_init();
}

/** 
* @brief configure TIM2
*	@param void
* @return void
*/
void tim2_init(void){
	// timer running at 168/4 = 42 Mhz (connected to APB1 - look in sysclk config)
	mouse_time_elapsed = 0;
	__TIM2_CLK_ENABLE();
	tim_handle_2.Instance = TIM2;
	tim_handle_2.Init.ClockDivision = 0;
	tim_handle_2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	tim_handle_2.Init.Period = 699;
	tim_handle_2.Init.Prescaler = 599;
	tim_handle_2.Init.RepetitionCounter = 0;
	if(HAL_TIM_Base_Init(&tim_handle_2)!=0){
		Error_Handler(TIM_INIT_FAIL);
	}
	HAL_TIM_Base_Start_IT(&tim_handle_2);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
	
}

/** 
* @brief configure TIM3
*	@param void
* @return void
*/
void tim3_init(void){
	// timer running at 168/4 = 42 Mhz (connected to APB1 - look in sysclk config)
	__TIM3_CLK_ENABLE();
	tim_handle_3.Instance = TIM3;
	tim_handle_3.Init.ClockDivision = 0;
	tim_handle_3.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	tim_handle_3.Init.Period = 6999;
	tim_handle_3.Init.Prescaler = 1199;
	tim_handle_3.Init.RepetitionCounter = 0;	
	if(HAL_TIM_Base_Init(&tim_handle_3)!=0){
		Error_Handler(TIM_INIT_FAIL);
	}
	HAL_TIM_Base_Start_IT(&tim_handle_3);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 1, 0);
}

/** 
* @brief configure TIM4
*	@param void
* @return void
*/
void tim4_init(void){
	// timer running at 168/4 = 42 Mhz (connected to APB1 - look in sysclk config)
	temp_time_elapsed = 0;
	__TIM4_CLK_ENABLE();
	tim_handle_4.Instance = TIM4;
	tim_handle_4.Init.ClockDivision = 0;
	tim_handle_4.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	tim_handle_4.Init.Period = 699;
	tim_handle_4.Init.Prescaler = 119;
	tim_handle_4.Init.RepetitionCounter = 0;
	if(HAL_TIM_Base_Init(&tim_handle_4)!=0){
		Error_Handler(TIM_INIT_FAIL);
	}
	
	HAL_TIM_Base_Start_IT(&tim_handle_4);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
}

