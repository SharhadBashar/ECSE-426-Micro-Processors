////////////////////////////////////////////////////////////////////////////////
//	File Name					: Acc_thread.c
//	Description				: Accelerometer calculation based off GPIO EXTI.
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Nov 11th, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "Acc_thread.h" 
#include <stm32f4xx_hal.h>
#include <cmsis_os.h>
#include "accelerometer.h"
#include "math.h"
#include "kalman.h"
#include <arm_math.h>
#include <LIS3DSH.h>

//Declaration
void Acc_thread(void const *args);

struct kalman_state xstate, ystate, zstate;
float pitch, roll, axes_vals[3], x, y, z, offsetx, offsety, offsetz;
osMutexDef (acc_mut);
osMutexId acc_mut_id;

osThreadId Acc_thread_ID;
osThreadDef(Acc_thread, osPriorityNormal, 1, 0);

//Brief:		Initializes EXTI periphs and Kalman Filter. 
//Params:		None
//Return:		None
void Acc_thread_periph_init(void) {
	// Accelerometer initialization.
	acc_init();
	acc_calibrate();
	
	//Sharhad don't define GPIO Pins in here anymore.
	
	kalman_init(&xstate, 1.00, 1.00, 0.02, 1.00, 0.00, 2.00);
	kalman_init(&ystate, 1.00, 1.00, 0.02, 1.00, 0.00, 2.00);
	kalman_init(&zstate, 1.00, 1.00, 0.02, 1.00, 0.00, 2.00);
	
	acc_mut_id = osMutexCreate(osMutex(acc_mut));
}

//Brief:		Starts the accelerometer thread in the OS (from Inactive into the Lifecycle)
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void start_Acc_thread(void *args) {
	Acc_thread_ID = osThreadCreate(osThread(Acc_thread), args);
}

//Brief:		The accelerometer thread function in the OS
//					Waits for a signal from the GPIO EXTI interrupt handler and then 
//					calculates the pich and roll.
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void Acc_thread(void const *args) {
	Acc_thread_periph_init();
	while(1) {
		osSignalWait(0x00000003, osWaitForever);
		LIS3DSH_ReadACC(axes_vals);
		
		osMutexWait(acc_mut_id, 0);
		
		x = Kalmanfilter_C(axes_vals[0] - offsetx, &xstate);
		y = Kalmanfilter_C(axes_vals[1] - offsety, &ystate);
		z = Kalmanfilter_C(axes_vals[2] - offsetz, &zstate);
		pitch = pitch_calc(x, y, z);
		roll = roll_calc(x, y, z);
		
		osMutexRelease(acc_mut_id);
		
//		printf("Pitch: %f \t\t\t", pitch);
//		printf("Roll: %f \n", roll);
//		printf("x: %f \t y: %f \t z: %f \t \n",x,y,z);
	}
}

//Brief:		The GPIO interrupt callback. Sends a signal to the Acc_thread
//					if the interrupt was recived from GPIO EXTI
//Params:		Pointer to the TIM handle that caused the interrupt
//Return:		None
void HAL_GPIO_EXTI_Callback(uint16_t Gpio_Pin){
	if (Gpio_Pin == GPIO_PIN_0){
		osSignalSet(Acc_thread_ID, 0x00000003);
	}
}

//Brief:		Calibration of accelerometer.
//Params:		None
//Return:		None
void acc_calibrate(void){
	float sumx, sumy, sumz;
	int counter = 1;
	while(counter <= 40){
		osSignalWait(0x00000003, osWaitForever);
		
		if (counter == 1) {
			printf("Calibrating Accelerometer. \n");
		}
		
		osMutexWait(acc_mut_id, 0);
		
		LIS3DSH_ReadACC(axes_vals);
		sumx += axes_vals[0];
		sumy += axes_vals[1];
		sumz += axes_vals[2];
		
		osMutexRelease(acc_mut_id);
		
		counter++;
//		printf(" Counter: %d \n", counter);
//		printf("Sum X: %f \n", sumx);
//		printf("Sum Y: %f \n", sumy);
//		printf("Sum Z: %f \n", sumz);
	}
	//offset for kalman
	offsetx = sumx/(float)counter;
	offsety = sumy/(float)counter;
	offsetz = sumz/(float)(counter-1);
	
}