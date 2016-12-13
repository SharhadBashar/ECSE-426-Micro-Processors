////////////////////////////////////////////////////////////////////////////////
//	File Name					: Temp_thread.c
//	Description				: Temperature calculation based off TIM3.
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Nov 11th, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "Temp_thread.h" 
#include <stm32f4xx_hal.h>
#include <cmsis_os.h>
#include "temperature.h"
#include "kalman.h"

//Declaration
void Temp_thread(void const *args);

struct kalman_state tstate;
float temp;
osMutexDef (temp_mut);
osMutexId temp_mut_id;

osThreadId Temp_thread_ID;
osThreadDef(Temp_thread, osPriorityNormal, 1, 0);
TIM_HandleTypeDef TIM3_handle;

//Brief:		Initializes TIM periphs and Kalman Filter. TIM3 provides interrups every second.
//Params:		None
//Return:		None
void Temp_thread_periph_init(void) {
	// Timers for temperature calculation.
	adc_Temp_Init();
	
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	TIM3_handle.Instance = TIM3;
	TIM3_handle.Init.Prescaler					= 9999;        
	TIM3_handle.Init.CounterMode				= TIM_COUNTERMODE_DOWN;     
	TIM3_handle.Init.Period							= 8399;           
	TIM3_handle.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;    
	TIM3_handle.Init.RepetitionCounter	= 0;
	
	HAL_TIM_Base_Init(&TIM3_handle);
	HAL_TIM_Base_Start_IT(&TIM3_handle);
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);
	
	//Sharhad don't define GPIO Pins in here anymore.
	
	kalman_init(&tstate, 1.00, 1.00, 0.02, 0.10, 30.00, 2.00);
	
	temp_mut_id = osMutexCreate(osMutex(temp_mut));
}

//Brief:		Starts the Temperature thread in the OS (from Inactive into the Lifecycle)
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void start_Temp_thread(void *args) {
	Temp_thread_ID = osThreadCreate(osThread(Temp_thread), args);
}

//Brief:		The Temperature thread function in the OS
//					Waits for a signal from the TIM3 interrupt handler and then 
//					calculates the Temperature.
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void Temp_thread(void const *args) {
	Temp_thread_periph_init();
	while(1) {
		osSignalWait(0x00000002, osWaitForever);
		//Calculate Temperature.
		osMutexWait(temp_mut_id, 0);
		temp = Kalmanfilter_C(get_Data_Sensor(), &tstate);
//		printf("Temp = %f C \n", temp);
		osMutexRelease(temp_mut_id);
	}
}
