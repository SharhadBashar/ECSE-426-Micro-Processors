////////////////////////////////////////////////////////////////////////////////
//	File Name				: temperature.c
//	Description			: Reads the Voltage and convertes it to degree Celcius
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Nov 11th, 2016
////////////////////////////////////////////////////////////////////////////////

#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include "temperature.h"

ADC_HandleTypeDef temp_ADC_Handle;

void adc_Temp_Init(void){
	
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
	temp_ADC_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;				// time for the internal capacitor to charge. longer means more accurate
	temp_ADC_Channel.Offset = 0;
	
	/* initialize ADC mode struct */
	temp_ADC_Mode.Mode = ADC_MODE_INDEPENDENT;														
	temp_ADC_Mode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	temp_ADC_Mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	TEMP_ADC_CLK_EN;
	HAL_ADC_ConfigChannel(&temp_ADC_Handle, &temp_ADC_Channel);
	HAL_ADCEx_MultiModeConfigChannel(&temp_ADC_Handle, &temp_ADC_Mode);	/* configure mode */

	GPIO_InitTypeDef GPIO_InitDef; 								
	TEMP_ADC_GPIO_PORT_CLK_EN; 									
	
	GPIO_InitDef.Pin = TEMP_ADC_GPIO_PIN; 	
	GPIO_InitDef.Mode = GPIO_MODE_ANALOG;   			
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	
	HAL_GPIO_Init(TEMP_ADC_GPIO_PORT, &GPIO_InitDef);
}


float vol_To_Temp(float voltage){
	
	/** Temperature(°C) = ((V_SENSE - V_25) / Avg_Slope) + 25
			* where V_SENSE is voltage read from the internal temp sensor
			*V_25 is the reference voltage at 25°C -> 0.76V
			*Avg_Slope is the average slope of temperature vs V_SENSE curve -> 2.5mV/°C
			**/
	
	float temp_Celcius;		
	temp_Celcius = (((voltage - 0.76f)/0.002f)+25.0f);
	return temp_Celcius;
}

float get_Data_Sensor(void){
	/**
	Conversion Of Base 12 Voltage readings to decimal system and then to raw temperature readings.
	Takes no arguments.
	Returns raw temperature readings.
	**/
	float voltage_Raw = 0;
	float voltage;
	float temperature;
	
	
	HAL_ADC_Start(&temp_ADC_Handle);
	if(HAL_ADC_PollForConversion(&temp_ADC_Handle, POLL_TIMEOUT) == HAL_OK) //gives the raw data from the sensor if sensor is available
		voltage_Raw = HAL_ADC_GetValue(&temp_ADC_Handle);
	HAL_ADC_Stop(&temp_ADC_Handle);
	voltage = (voltage_Raw*3.0)/4096.0; //convert to voltage from base 12 ADC
	temperature = vol_To_Temp(voltage); //Convert voltage to celcius
	return temperature;
}	
