////////////////////////////////////////////////////////////////////////////////
//	File Name					: Kp_thread.c
//	Description				: Keypad polling based off TIM4.
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Nov 11th, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "Kp_thread.h" 
#include <stm32f4xx_hal.h>
#include <cmsis_os.h>
#include "key_pad.h"


//Declaration
void Kp_thread(void const *args);
int key_val;
mode_type mode = ACCEL; 
tilt_type tilt = PTH;

osThreadId Kp_thread_ID;
osThreadDef(Kp_thread, osPriorityNormal, 1, 0);
osMutexDef (mode_mut);
osMutexDef (tilt_mut);
osMutexId mode_mut_id;
osMutexId tilt_mut_id;

TIM_HandleTypeDef TIM4_handle;

//Brief:		Initializes TIM periphs. TIM4 provides interrups every [insert time here].
//Params:		None
//Return:		None
void Kp_thread_periph_init(void) {
	// Timers for temperature calculation.
	keypad_init();
	
	__HAL_RCC_TIM4_CLK_ENABLE();
	
	TIM4_handle.Instance = TIM4;
	TIM4_handle.Init.Prescaler					= 9999;        
	TIM4_handle.Init.CounterMode				= TIM_COUNTERMODE_DOWN;     
	TIM4_handle.Init.Period							= 2099;           
	TIM4_handle.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;    
	TIM4_handle.Init.RepetitionCounter	= 0;
	
	HAL_TIM_Base_Init(&TIM4_handle);
	HAL_TIM_Base_Start_IT(&TIM4_handle);
	
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
	
	mode_mut_id = osMutexCreate(osMutex(mode_mut));
	tilt_mut_id = osMutexCreate(osMutex(tilt_mut));
	
	//Sharhad don't define GPIO Pins in here anymore.
}

//Brief:		Starts the keypad thread in the OS (from Inactive into the Lifecycle)
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void start_Kp_thread(void *args) {
	Kp_thread_ID = osThreadCreate(osThread(Kp_thread), args);
}

//Brief:		The keypad thread function in the OS
//					Waits for a signal from the TIM4 interrupt handler and then 
//					polls the keypad.
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void Kp_thread(void const *args) {
	Kp_thread_periph_init();
	printf("I initialized \n");
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		//read keypad
		key_val = keypad_read();
		
		osMutexWait(mode_mut_id, 0);
		osMutexWait(tilt_mut_id, 0);
		
		switch(key_val){
			case -1:
				if(mode == ACCEL){
					tilt = PTH;
					//printf("Type Change Pitch \n");
				}
				break;
				
			case -2:
				if(mode == ACCEL){
					tilt = ROL;
					//printf("Type Change Roll \n");
				}
				break;
				
			case 10:
				mode = ACCEL;
				//printf("Mode Change Acceleration \n");
				break;
			
			case 11:
				mode = TEMP;
				//printf("Mode Change Temeperature \n");
				break;
			
			default:
				//printf("%d \n", key_val);
				break;
		}
		
		osMutexRelease(mode_mut_id);
		osMutexRelease(tilt_mut_id);
	}
}