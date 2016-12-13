////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: header file for adc_example.h
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _ADC_EXAMPLE
#define _ADC_EXAMPLE

#define POLL_TIMEOUT 10000

#define EXAMPLE_ADC ADC1
#define EXAMPLE_ADC_CHANNEL ADC_CHANNEL_11
#define EXAMPLE_ADC_CLK_EN 	__HAL_RCC_ADC1_CLK_ENABLE()

#define EXAMPLE_ADC_GPIO_PORT GPIOC
#define EXAMPLE_ADC_GPIO_PIN GPIO_PIN_1 // Pin A5 is connected to the ADC1 Channel 5 input
#define EXAMPLE_ADC_GPIO_PORT_CLK_EN __HAL_RCC_GPIOC_CLK_ENABLE();

void adc_example_init(void);
float adc_example(void);
#endif
