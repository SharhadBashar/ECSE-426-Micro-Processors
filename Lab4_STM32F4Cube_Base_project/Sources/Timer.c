////////////////////////////////////////////////////////////////////////////////
//	File Name					: Timer.c
//	Description				: Time handler definition
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Nov 11th, 2016
////////////////////////////////////////////////////////////////////////////////
#include "cmsis_os.h"
#include "Temp_thread.h"
#include "Kp_thread.h"
#include "Disp_thread.h"
#include <stm32f4xx_hal.h>

//Brief:		The TIM interrupt callback. Sends a signal to the Temp_thread
//					if the interrupt was received from TIM3. Sends a signal to the
//					Kp_thread if interrupt received from TIM4.
//Params:		Pointer to the TIM handle that caused the interrupt
//Return:		None
//Interrupt callbacks
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {	
	if(htim->Instance == TIM3){
		osSignalSet(Temp_thread_ID, 0x00000002);
	}else if(htim->Instance == TIM4){
		osSignalSet(Kp_thread_ID, 0x00000001);
	}else if(htim->Instance == TIM5){
		osSignalSet(Disp_thread_ID, 0x00000004);
	}
}
