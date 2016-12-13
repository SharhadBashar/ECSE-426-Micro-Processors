/**
  ******************************************************************************
  * File Name					: acc.c
	*	Description				: functions to read from the internal temperature ADC
	*	Author						: Stephan Greto-McGrath
	*	Date							: Nov 6th, 2016
  ******************************************************************************
  */


#include <supporting_functions.h>
#include <acc.h>
#include <lis3dsh.h>
#include <interrupts.h>
#include <arm_math.h>

////////////////////////////////////////////////////////////////////////////////
//	declarations
////////////////////////////////////////////////////////////////////////////////

// thread declarations
osThreadDef(acc_thread, osPriorityNormal, 1,0);
osThreadId acc_thread_ID;
osSemaphoreId acc_semaphore;
osSemaphoreDef(acc_semphore);
// interrupt flag
volatile int acc_interrupt = 0;
// reading values
float acc_val[] = {0,0,0};
float acc_val_cor[] = {0,0,0};
// funtction declarations
float pitch_conversion(float x, float y, float z);
float roll_conversion(float x, float y, float z);
void acc_correction(float* value, float* calibrated);

////////////////////////////////////////////////////////////////////////////////
//	initilization functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief initializes the accelerometer
	* @param void
	*	@return void
	*/
void acc_init(void){ // for clarifications see definitions
	LIS3DSH_InitTypeDef acc_struct;
	LIS3DSH_DRYInterruptConfigTypeDef acc_int_struct;
	GPIO_InitTypeDef acc_int_GPIO_struct;
	acc_struct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;
	acc_struct.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	acc_struct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;
	acc_struct.AA_Filter_BW = LIS3DSH_AA_BW_50;
	acc_struct.Full_Scale  = LIS3DSH_FULLSCALE_2;
	acc_struct.Self_Test = LIS3DSH_SELFTEST_NORMAL;	
	acc_int_struct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	acc_int_struct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	acc_int_struct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	LIS3DSH_Init(&acc_struct);
	LIS3DSH_DataReadyInterruptConfig(&acc_int_struct);
	__HAL_RCC_GPIOE_CLK_ENABLE();
	acc_int_GPIO_struct.Pin = ACC_PIN;
	acc_int_GPIO_struct.Mode = GPIO_MODE_IT_RISING;
	acc_int_GPIO_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	acc_int_GPIO_struct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &acc_int_GPIO_struct);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);				// interrupts received periodically on EXTI0	
	HAL_NVIC_SetPriority(EXTI0_IRQn,1,0);
}


////////////////////////////////////////////////////////////////////////////////
//	threading functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief starts the acc thread
	*	@param void *args
	* @return void
	*/
void start_acc_thread(void *args) {
	acc_thread_ID = osThreadCreate(osThread(acc_thread), args);
}

/** 
	* @brief acc thread routine
	*	@param void const *args
	* @return void
	*/
void acc_thread(void const *args) {
	//acc_init();
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		if (acc_interrupt == 1){
			acc_interrupt = 0;
			LIS3DSH_ReadACC(acc_val);
			osSemaphoreWait(acc_semaphore, osWaitForever);
			acc_correction(acc_val, acc_val_cor);
			osSemaphoreRelease(acc_semaphore);
		} 
	}
}

/** 
	* @brief makes the pitch accesible from other threads
	*	@param void 
	* @return float ret is the pitch
	*/
float get_pitch(void){
	osSemaphoreWait(acc_semaphore, osWaitForever);
	float ret = pitch_conversion(acc_val_cor[0], acc_val[1], acc_val[2]);
	osSemaphoreRelease(acc_semaphore);
	return ret;
}

/** 
	* @brief makes the pitch accesible from other threads
	*	@param void 
	* @return float ret is the roll
	*/
float get_roll(void){
	osSemaphoreWait(acc_semaphore, osWaitForever);
	float ret = roll_conversion(acc_val_cor[0], acc_val[1], acc_val[2]);
	osSemaphoreRelease(acc_semaphore);
	return ret;
}


////////////////////////////////////////////////////////////////////////////////
//	accelerometer conversion functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief get pitch from xyz values
	*	@param x X reading
	*	@param y Y reading
	*	@param z Z reading
	* @return float pitch
	*/
float pitch_conversion(float x, float y, float z){
	float ret = atan(x/sqrt(x*x + z*z))*(180/PI);
	if (ret < 0) ret = 360 + ret;
	return ret;
}

/** 
	* @brief get roll from xyz values
	*	@param x X reading
	*	@param y Y reading
	*	@param z Z reading
	* @return float roll
	*/
float roll_conversion(float x, float y, float z){
	float ret = atan(y/sqrt(y*y + z*z))*(180/PI);
	if (ret < 0) ret = 360 + ret;
	return ret;
}

/** 
	* @brief use the calibration matrix to correct XYZ values 
	*	@param float* value input to be corrected
	*	@param float* calibrated value after corection (global)
	* @return void
	*/
void acc_correction(float* value, float* calibrated){
	// using least squares method these numbers were achieved
	// 0.985722303638867;		-0.0156810488029380;		0.0122582349342607
  // -0.0283712429004606;		0.992028798330257;		-0.0123347118561540
	// -0.00483318731562110;	-0.0157062347955642;		0.986790400008687
	// 0.00167972764024671;		0.00259456495217067;		-0.00638219455126809
	calibrated[0] = (float) (0.985722303638867*value[0] + -0.0283712429004606*value[1] + -0.00483318731562110*value[2] + 0.00167972764024671*1000);
	calibrated[1] = (float) (-0.0156810488029380*value[0] + 0.992028798330257*value[1] + -0.0157062347955642*value[2] + 0.00259456495217067*1000);
	calibrated[2] = (float) (0.0122582349342607*value[0] + -0.0123347118561540*value[1] + 0.986790400008687*value[2] + -0.00638219455126809*1000);
}



