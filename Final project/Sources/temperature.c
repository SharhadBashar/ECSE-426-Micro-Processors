/**
  ******************************************************************************
  * File Name					: temperature.c
	*	Description				: function for using the adc for internal temp reading 
	*	Author						: Stephan Greto-McGrath
	*	Date							: Oct 4th, 2016
  ******************************************************************************
  */
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <cmsis_os.h>
#include <temperature.h>
#include <kalman_filter.h>
#include <interrupts.h>
#define Measurement_dimension 1
#define State_dimension 1
#define Length 1

////////////////////////////////////////////////////////////////////////////////
//	declarations
////////////////////////////////////////////////////////////////////////////////

// matrices to be used in the kalman filter
float temp_kf[State_dimension*State_dimension] = {1.0};
float temp_kh[Measurement_dimension*State_dimension] = {1.0};
float temp_kq[State_dimension*State_dimension] = {0.02};
float temp_kr[Measurement_dimension*Measurement_dimension] = {50.0};
float temp_kx[State_dimension] = {30.0};
float temp_kp[State_dimension*State_dimension] = {500.0};
float temp_kk[State_dimension*Measurement_dimension] = {0.0};
kalman_state temp_kstate;
// input and output of the kalman filter
float out[1] = {0.0};
float* filter (float in);
data d; // data struct 
// adc setup
ADC_HandleTypeDef temp_ADC_Handle;

// thread declariations
osThreadDef(temp_thread, osPriorityNormal, 1,0);
osThreadId temp_thread_ID;
osSemaphoreId temp_semaphore;
osSemaphoreDef(temp_semphore);
// funcion declarations
void temp_timer_init(void);
void filter_init(void);
float temp_poll(void);
float celcius_conv(float raw);
void read_temp(void);

////////////////////////////////////////////////////////////////////////////////
//	initilization functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief initializes the ADC for internal temperature readings
	*	@param void
	* @return none
	*/
void temp_adc_init(void) {
	ADC_InitTypeDef temp_ADC_Init; 																	// definition of ADC1 initialiation struct
	ADC_ChannelConfTypeDef temp_ADC_Channel;												// definition of ADC1 channel struct
	HAL_LockTypeDef temp_ADC_Lock; 																	// define ADC1 locking object
	ADC_MultiModeTypeDef temp_ADC_Mode; 														// define ADC1 mode struct
	
	/*  initialize ADC init struct */
	temp_ADC_Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;					// ADC Clock frequency 42MHz (168/4)
	temp_ADC_Init.Resolution = ADC_RESOLUTION_12B;										// 12 bit resolution, better but slower
	temp_ADC_Init.DataAlign = ADC_DATAALIGN_RIGHT;										// align the 12 bits data at the right of the 32 bits words
	temp_ADC_Init.ScanConvMode = DISABLE;														// single channel mode
	temp_ADC_Init.EOCSelection = ADC_EOC_SEQ_CONV;										// perform ADC conversions without having to read all conversion data
	temp_ADC_Init.ContinuousConvMode = DISABLE;											// single mode convertion
	temp_ADC_Init.DMAContinuousRequests = DISABLE;										// single mode DMA request
	temp_ADC_Init.NbrOfConversion = 1;																// one conversion
	temp_ADC_Init.DiscontinuousConvMode = ENABLE;										// enable discountinuous mode
	temp_ADC_Init.NbrOfDiscConversion = 1;														// one conversion
	temp_ADC_Init.ExternalTrigConv = ADC_SOFTWARE_START;							// no external trigger
	temp_ADC_Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;					// no external trigger
	
	/* initialize ADC handle struct */
	temp_ADC_Handle.Instance = TEMP_ADC;
	temp_ADC_Handle.Init = temp_ADC_Init;
	temp_ADC_Handle.NbrOfCurrentConversionRank = 1;
	temp_ADC_Handle.Lock = temp_ADC_Lock;
	temp_ADC_Handle.State = 0;
	temp_ADC_Handle.ErrorCode = HAL_ADC_ERROR_NONE;
	
	/* initialize ADC channel struct */
	temp_ADC_Channel.Channel = TEMP_ADC_CHANNEL;
	temp_ADC_Channel.Rank = 1;																			// use to determine the rank in which this channel is sampled
	temp_ADC_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;				// time for the internal capacitor to charge. longuer means more accurate
	temp_ADC_Channel.Offset = 0;
	
	/* initialize ADC mode struct */
	temp_ADC_Mode.Mode = ADC_MODE_INDEPENDENT;														
	temp_ADC_Mode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	temp_ADC_Mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	TEMP_ADC_CLK_EN;
	HAL_ADC_ConfigChannel(&temp_ADC_Handle, &temp_ADC_Channel);
	HAL_ADCEx_MultiModeConfigChannel(&temp_ADC_Handle, &temp_ADC_Mode);	/* configure mode */
	HAL_ADC_Start(&temp_ADC_Handle);
	filter_init();
}


/** 
* @brief configures filter inputs
*	@param void
* @return void
*/
void filter_init(void){
		arm_mat_init_f32(&temp_kstate.f, State_dimension, State_dimension, temp_kf);
		arm_mat_init_f32(&temp_kstate.h, Measurement_dimension, State_dimension, temp_kh);
		arm_mat_init_f32(&temp_kstate.q, State_dimension, State_dimension, temp_kq);
		arm_mat_init_f32(&temp_kstate.r, Measurement_dimension, Measurement_dimension, temp_kr);
		arm_mat_init_f32(&temp_kstate.x, State_dimension, 1, temp_kx);
		arm_mat_init_f32(&temp_kstate.p, State_dimension, State_dimension, temp_kp);
		arm_mat_init_f32(&temp_kstate.k, State_dimension, Measurement_dimension, temp_kk);
}

////////////////////////////////////////////////////////////////////////////////
//	threading functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief starts the temperature thread
	*	@param void *args
	* @return void
	*/
void start_temp_thread(void *args) {
	temp_thread_ID = osThreadCreate(osThread(temp_thread), args);
}

/** 
	* @brief temperature thread routine
	*	@param void const *args
	* @return void
	*/
void temp_thread(void const *args) {
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
 		if (temp_time_elapsed == 1){
			temp_time_elapsed = 0;
			osSemaphoreWait(temp_semaphore, osWaitForever);
			read_temp();
			osSemaphoreRelease(temp_semaphore);
		} 
	}
}

/** 
	* @brief makes the temperature data accesible from other threads
	*	@param void 
	* @return float ret is the filtered temperature reading
	*/
float get_temp(void){
	float ret;
	osSemaphoreWait(temp_semaphore, osWaitForever);
	ret = d.filtered;
	osSemaphoreRelease(temp_semaphore);
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//	temperature reading functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief filters the input and stores in in a struc along with raw data
	*	@param void 
	* @return data d this is the readings (raw and filtered)
	*/
void read_temp(void){
	d.raw = celcius_conv(temp_poll());
	d.filtered = celcius_conv(*filter(temp_poll()));
}

/** 
	* @brief polls the ADC
	*	@param void 
	* @return float value from ADC converted to V
	*/
float temp_poll(void){
	float val = 0;
	HAL_ADC_Start(&temp_ADC_Handle);
	if(HAL_ADC_PollForConversion(&temp_ADC_Handle, POLL_TIMEOUT) == HAL_OK)
		val = HAL_ADC_GetValue(&temp_ADC_Handle);
	HAL_ADC_Stop(&temp_ADC_Handle);
	return (val*3.0)/4096.0;
}

/** 
	* @brief converts V to celcius
	*	@param floa raw reading to be converted
	* @return float value in V converted to C
	*/
float celcius_conv(float raw){
	float avg_slope = 0.0025;
	float v25 = 0.76;
	return (raw - v25)/avg_slope + 25;
}
/** 
	* @brief wrapper for kalman filter to not have to input everything on each call
	*	@param float in data to be filtered
	* @return float value kalman filtered output
	*/
float* filter (float in){	
		kalman_filter(&in, out, &temp_kstate, Length, State_dimension, Measurement_dimension);
		return out;
}
