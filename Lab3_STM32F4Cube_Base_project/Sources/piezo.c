////////////////////////////////////////////////////////////////////////////////
//	File Name					: piezo.c
//	Description				: Code to use the piezo sensor
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 24th, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <piezo.h>

//		Global variables		//
ADC_HandleTypeDef piezo_ADC_Handle;

/*Brief: Sets up the desired(refer to header) ADC and corresponding GPIO for input
**Params: None
**Return: None
*/
void adc_piezo_init(void) {
	ADC_InitTypeDef piezo_ADC_Init; 																	// definition of ADC1 initialiation struct
	ADC_ChannelConfTypeDef piezo_ADC_Channel;												// definition of ADC1 channel struct
	HAL_LockTypeDef piezo_ADC_Lock; 																	// define ADC1 locking object
	ADC_MultiModeTypeDef piezo_ADC_Mode; 														// define ADC1 mode struct
	
	/*  initialize ADC init struct */
	piezo_ADC_Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;					// ADC Clock frequency 42MHz (168/4)
	piezo_ADC_Init.Resolution = ADC_RESOLUTION_12B;										// 12 bit resolution, better but slower
	piezo_ADC_Init.DataAlign = ADC_DATAALIGN_RIGHT;										// align the 12 bits data at the right of the 32 bits words
	piezo_ADC_Init.ScanConvMode = DISABLE;														// single channel mode
	piezo_ADC_Init.EOCSelection = ADC_EOC_SEQ_CONV;										// perform ADC conversions without having to read all conversion data
	piezo_ADC_Init.ContinuousConvMode = DISABLE;											// single mode convertion
	piezo_ADC_Init.DMAContinuousRequests = DISABLE;										// single mode DMA request
	piezo_ADC_Init.NbrOfConversion = 1;																// one conversion
	piezo_ADC_Init.DiscontinuousConvMode = ENABLE;										// enable discountinuous mode
	piezo_ADC_Init.NbrOfDiscConversion = 1;														// one conversion
	piezo_ADC_Init.ExternalTrigConv = ADC_SOFTWARE_START;							// no external trigger
	piezo_ADC_Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;					// no external trigger
	
	/* initialize ADC handle struct */
	piezo_ADC_Handle.Instance = PIEZO_ADC;
	piezo_ADC_Handle.Init = piezo_ADC_Init;
	piezo_ADC_Handle.NbrOfCurrentConversionRank = 1;
	piezo_ADC_Handle.Lock = piezo_ADC_Lock;
	piezo_ADC_Handle.State = 0;
	piezo_ADC_Handle.ErrorCode = HAL_ADC_ERROR_NONE;
	
	/* initialize ADC channel struct */
	piezo_ADC_Channel.Channel = PIEZO_ADC_CHANNEL;
	piezo_ADC_Channel.Rank = 1;																			// use to determine the rank in which this channel is sampled
	piezo_ADC_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;				// time for the internal capacitor to charge. longuer means more accurate
	piezo_ADC_Channel.Offset = 0;
	
	/* initialize ADC mode struct */
	piezo_ADC_Mode.Mode = ADC_MODE_INDEPENDENT;														
	piezo_ADC_Mode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	piezo_ADC_Mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	PIEZO_ADC_CLK_EN;
	HAL_ADC_ConfigChannel(&piezo_ADC_Handle, &piezo_ADC_Channel);
	HAL_ADCEx_MultiModeConfigChannel(&piezo_ADC_Handle, &piezo_ADC_Mode);	/* configure mode */

	GPIO_InitTypeDef GPIO_InitDef; 								
	PIEZO_ADC_GPIO_PORT_CLK_EN; 									
	
	GPIO_InitDef.Pin = PIEZO_ADC_GPIO_PIN; 	
	GPIO_InitDef.Mode = GPIO_MODE_ANALOG;   			
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	
	HAL_GPIO_Init(PIEZO_ADC_GPIO_PORT, &GPIO_InitDef);

	
	
}

/*Brief: Performs the steps to poll the ADC, and converts the obtained value to a voltage
**Params: None
**Return: None
*/
float adc_piezo(void) {
	float val = 0;
	HAL_ADC_Start(&piezo_ADC_Handle);
	if(HAL_ADC_PollForConversion(&piezo_ADC_Handle, POLL_TIMEOUT) == HAL_OK)
		val = HAL_ADC_GetValue(&piezo_ADC_Handle);
	HAL_ADC_Stop(&piezo_ADC_Handle);
	return (3*val)/4096.0;
}

