////////////////////////////////////////////////////////////////////////////////
//	File Name					: adc_example.c
//	Description				: Sample code to help use the ADC driver
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <adc_example.h>

//		Global variables		//
ADC_HandleTypeDef example_ADC_Handle;

/*Brief: Sets up the desired(refer to header) ADC and corresponding GPIO for input
**Params: None
**Return: None
*/
void adc_example_init(void) {
	ADC_InitTypeDef example_ADC_Init; 																	// definition of ADC1 initialiation struct
	ADC_ChannelConfTypeDef example_ADC_Channel;												// definition of ADC1 channel struct
	HAL_LockTypeDef example_ADC_Lock; 																	// define ADC1 locking object
	ADC_MultiModeTypeDef example_ADC_Mode; 														// define ADC1 mode struct
	
	/*  initialize ADC init struct */
	example_ADC_Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;					// ADC Clock frequency 42MHz (168/4)
	example_ADC_Init.Resolution = ADC_RESOLUTION_12B;										// 12 bit resolution, better but slower
	example_ADC_Init.DataAlign = ADC_DATAALIGN_RIGHT;										// align the 12 bits data at the right of the 32 bits words
	example_ADC_Init.ScanConvMode = DISABLE;														// single channel mode
	example_ADC_Init.EOCSelection = ADC_EOC_SEQ_CONV;										// perform ADC conversions without having to read all conversion data
	example_ADC_Init.ContinuousConvMode = DISABLE;											// single mode convertion
	example_ADC_Init.DMAContinuousRequests = DISABLE;										// single mode DMA request
	example_ADC_Init.NbrOfConversion = 1;																// one conversion
	example_ADC_Init.DiscontinuousConvMode = ENABLE;										// enable discountinuous mode
	example_ADC_Init.NbrOfDiscConversion = 1;														// one conversion
	example_ADC_Init.ExternalTrigConv = ADC_SOFTWARE_START;							// no external trigger
	example_ADC_Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;					// no external trigger
	
	/* initialize ADC handle struct */
	example_ADC_Handle.Instance = EXAMPLE_ADC;
	example_ADC_Handle.Init = example_ADC_Init;
	example_ADC_Handle.NbrOfCurrentConversionRank = 1;
	example_ADC_Handle.Lock = example_ADC_Lock;
	example_ADC_Handle.State = 0;
	example_ADC_Handle.ErrorCode = HAL_ADC_ERROR_NONE;
	
	/* initialize ADC channel struct */
	example_ADC_Channel.Channel = EXAMPLE_ADC_CHANNEL;
	example_ADC_Channel.Rank = 1;																			// use to determine the rank in which this channel is sampled
	example_ADC_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;				// time for the internal capacitor to charge. longuer means more accurate
	example_ADC_Channel.Offset = 0;
	
	/* initialize ADC mode struct */
	example_ADC_Mode.Mode = ADC_MODE_INDEPENDENT;														
	example_ADC_Mode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	example_ADC_Mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	EXAMPLE_ADC_CLK_EN;
	HAL_ADC_ConfigChannel(&example_ADC_Handle, &example_ADC_Channel);
	HAL_ADCEx_MultiModeConfigChannel(&example_ADC_Handle, &example_ADC_Mode);	/* configure mode */

	GPIO_InitTypeDef GPIO_InitDef; 								
	EXAMPLE_ADC_GPIO_PORT_CLK_EN; 									
	
	GPIO_InitDef.Pin = EXAMPLE_ADC_GPIO_PIN; 	
	GPIO_InitDef.Mode = GPIO_MODE_ANALOG;   			
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	
	HAL_GPIO_Init(EXAMPLE_ADC_GPIO_PORT, &GPIO_InitDef);
}

/*Brief: Performs the steps to poll the ADC, and converts the obtained value to a voltage
**Params: None
**Return: None
*/
float adc_example(void) {
	float val = 0;
	HAL_ADC_Start(&example_ADC_Handle);
	if(HAL_ADC_PollForConversion(&example_ADC_Handle, POLL_TIMEOUT) == HAL_OK)
		val = HAL_ADC_GetValue(&example_ADC_Handle);
	HAL_ADC_Stop(&example_ADC_Handle);
	return (val*3.0)/409.60;
}

