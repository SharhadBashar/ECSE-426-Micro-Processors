////////////////////////////////////////////////////////////////////////////////
//	File Name				: Disp_thread.c
//	Description			: Displaying based off TIM5.
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Nov 11th, 2016
////////////////////////////////////////////////////////////////////////////////

	
//		Includes		//
#include "Kp_thread.h" 
#include "Temp_thread.h"
#include "Acc_thread.h"
#include <stm32f4xx_hal.h>
#include <cmsis_os.h>
#include "7_segment_display.h"


//Declaration
void Disp_thread(void const *args);
float temperature, pth, rol;
osThreadId Disp_thread_ID;
osThreadDef(Disp_thread, osPriorityNormal, 1, 0);
mode_type m;
int alarm;
tilt_type t;
TIM_HandleTypeDef TIM5_handle;

//Brief:		Initializes TIM periphs. TIM5 provides interrups every [insert time here].
//Params:		None
//Return:		None
void Disp_thread_periph_init(void) {
	// Timers for temperature calculation.
	init_Display_Pins();
	
	__HAL_RCC_TIM5_CLK_ENABLE();
	
	TIM5_handle.Instance = TIM5;
	TIM5_handle.Init.Prescaler					= 50;        
	TIM5_handle.Init.CounterMode				= TIM_COUNTERMODE_DOWN;     
	TIM5_handle.Init.Period							= 100;           
	TIM5_handle.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;    
	TIM5_handle.Init.RepetitionCounter	= 0;
	
	HAL_TIM_Base_Init(&TIM5_handle);
	HAL_TIM_Base_Start_IT(&TIM5_handle);
	
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	HAL_NVIC_SetPriority(TIM5_IRQn, 1, 2);
	
}

//Brief:		Starts the display thread in the OS (from Inactive into the Lifecycle)
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void start_Disp_thread(void *args) {
	Disp_thread_ID = osThreadCreate(osThread(Disp_thread), args);
}

//Brief:		The display thread function in the OS
//					Waits for a signal from the TIM5 interrupt handler and then 
//					displays values.
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void Disp_thread(void const *args) {
	Disp_thread_periph_init();
	int time_counter = 0;
	while(1) {
		osSignalWait(0x00000004, osWaitForever);
		
		osMutexWait(mode_mut_id, 0);
		osMutexWait(tilt_mut_id, 0);
		m = mode;
		t = tilt;
		osMutexRelease(mode_mut_id);
		osMutexRelease(tilt_mut_id);
		
		if(m == ACCEL && t == PTH){
			osMutexRelease(mode_mut_id);
			osMutexRelease(tilt_mut_id);
			
			osMutexWait(acc_mut_id, 0);
			pth = pitch;
			osMutexRelease(acc_mut_id);
			
			//Display pth 
			alarm = 0;
			if (temperature > 40.0){
				alarm = 1;
			}
			if (temperature > 40.0){
				osDelay(400);
			}
			display_Number(pth, time_counter,alarm);
			//printf("Pitch: %f \t\t\t", pitch);
			time_counter++;	
		}
		else if(m == ACCEL && t == ROL){
			
			osMutexWait(acc_mut_id, 0);
			rol = roll;
			osMutexRelease(acc_mut_id);
			
			//Display roll
			alarm = 0;
			display_Number(rol, time_counter,alarm);
			if (temperature > 40.0){
				alarm = 1;
			}
			if (temperature > 40.0){
				osDelay(400);
			}
			//printf("Roll: %f \n", roll);
			time_counter++;	
		}
		else if(m == TEMP){
			
			osMutexWait(temp_mut_id, 0);
			temperature = temp;
			osMutexRelease(temp_mut_id);
			
			//Display temperature
			alarm = 0;
			if (temperature > 40.0){
				alarm = 1;
			}
			display_Number(temperature, time_counter, alarm);
			if (temperature > 40.0){
				osDelay(400);
			}
			//printf("Temp = %f C \n", temp);
			time_counter++;
		}
	}
}