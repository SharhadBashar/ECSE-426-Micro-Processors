////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: Program Entry
//	Course					  : ECSE 426
//	Author						: Group 09
//	Date							: Dec 9th, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <arm_math.h>
#include <cmsis_os.h>
#include <rl_usb.h>   // Keil.MDK-Pro::USB:CORE
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "sysclk_config.h"
#include "acc.h"
#include "cc2500.h"
#include "mouse_thread.h"
#include "display.h"


//Brief:	main program
//				
//Params:	None
//Return:	None
 int main(void) {
  //		MCU Configuration		//
  //	Reset of all peripherals, Initializes the Flash interface and the Systick	//
	osKernelInitialize();  
  HAL_Init();
	timers_init(0x07);
	acc_init();
	display_init();
	cc2500_Init();
  mouse_init(2);

	
	
  //	Configure the system clock	//
  SystemClock_Config();
	
	USBD_Initialize(0);               /* USB Device 0 Initialization        */
  USBD_Connect(0); 
	
	// run threads
	
	start_acc_thread(NULL);
	start_display_thread(NULL);
	start_mouse_thread(NULL);
	
	osKernelStart();


}


