////////////////////////////////////////////////////////////////////////////////
//	File Name					: accelerometer.c
//	Description				: code for accelerometer
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 17th, 2016
////////////////////////////////////////////////////////////////////////////////

#include <arm_math.h>
#include "stm32f4xx_hal.h"


#include <LIS3DSH.h>
#include <accelerometer.h>
#include <math.h>

//iniliaze the accel, including the interupts
//Accel on in E0
void acc_init(void){
	LIS3DSH_InitTypeDef acc_struct;
	LIS3DSH_DRYInterruptConfigTypeDef acc_int_struct;
	GPIO_InitTypeDef acc_int_GPIO_struct;

	acc_struct.Power_Mode_Output_DataRate 	= LIS3DSH_DATARATE_25;
	acc_struct.Axes_Enable 									= LIS3DSH_XYZ_ENABLE;
	acc_struct.Continous_Update 						= LIS3DSH_ContinousUpdate_Enabled;
	acc_struct.Full_Scale 									= LIS3DSH_FULLSCALE_2;
	acc_struct.AA_Filter_BW 								= LIS3DSH_AA_BW_50;
	acc_struct.Self_Test 										= LIS3DSH_SELFTEST_NORMAL;

	acc_int_struct.Dataready_Interrupt 	= LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	acc_int_struct.Interrupt_signal 		= LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	acc_int_struct.Interrupt_type 			= LIS3DSH_INTERRUPT_REQUEST_PULSED; 

	LIS3DSH_Init(&acc_struct);
	LIS3DSH_DataReadyInterruptConfig(&acc_int_struct);

	__HAL_RCC_GPIOE_CLK_ENABLE();
	acc_int_GPIO_struct.Pin = GPIO_PIN_0;
	acc_int_GPIO_struct.Mode = GPIO_MODE_IT_FALLING;
	acc_int_GPIO_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	acc_int_GPIO_struct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init (GPIOE, &acc_int_GPIO_struct);
	//initialize the interrupts
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn,1,2);


}
//Calculate the pitch and roll from the x,y,z values
float pitch_calc(float x, float y, float z){
	float pitch;
	pitch = (float)atan(x/sqrt(x*x + z*z))*(180/PI);
	//pitch = (float)atan2(x,z)*(180/PI);
	return pitch;
}

float roll_calc(float x, float y, float z){
	float roll;
	roll = (float)atan(y/sqrt(y*y + z*z))*(180/PI);
	//roll = (float)atan2(y,z)*(180/PI);
	return roll;
}


