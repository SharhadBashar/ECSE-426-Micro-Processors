////////////////////////////////////////////////////////////////////////////////
//	File Name				: Piezo_Sensor.h
//	Description			: Provides definition & function to support the Piezo Sensor
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Oct 9th, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _PIEZO_SENSOR
#define _PIEZO_SENSOR

#define POLL_TIMEOUT 10000

#define EXAMPLE_ADC ADC1
#define EXAMPLE_ADC_CHANNEL ADC_CHANNEL_5
#define EXAMPLE_ADC_CLK_EN 	__HAL_RCC_ADC1_CLK_ENABLE()

#define EXAMPLE_ADC_GPIO_PORT GPIOA
#define EXAMPLE_ADC_GPIO_PIN GPIO_PIN_5 // Pin A5 is connected to the ADC1 Channel 5 input
#define EXAMPLE_ADC_GPIO_PORT_CLK_EN __HAL_RCC_GPIOA_CLK_ENABLE();

void adc_example_init(void);
float adc_example(void);

#endif