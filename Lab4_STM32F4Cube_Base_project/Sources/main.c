////////////////////////////////////////////////////////////////////////////////
//	File Name				: main.c
//	Description			: Code for the main
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Nov 11th, 2016
////////////////////////////////////////////////////////////////////////////////

	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <lis3dsh.h>
#include <arm_math.h>
#include "Temp_thread.h"
#include "Acc_thread.h"
#include "Kp_thread.h"
#include "Disp_thread.h"
#include <cmsis_os.h>

//Brief:	main program
//				
//Params:	None
//Return:	None
int main(void) {
  //		MCU Configuration		//
  //	Reset of all peripherals, Initializes the Flash interface and the Systick	//
  HAL_Init();
	
  //	Configure the system clock	//
  SystemClock_Config();
	
	//Start the threads
	start_Kp_thread(NULL);
	start_Temp_thread(NULL);
	start_Acc_thread(NULL);
	start_Disp_thread(NULL);
	//start_LED_thread(NULL);
	
	//Kernel instructions
	osKernelInitialize();
	osKernelStart();
	osDelay(osWaitForever);

	return 0;
}
