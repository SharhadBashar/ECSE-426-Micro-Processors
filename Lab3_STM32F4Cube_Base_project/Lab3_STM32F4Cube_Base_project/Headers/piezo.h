////////////////////////////////////////////////////////////////////////////////
//	File Name					: piezo.h
//	Description				: header file for piezo.c
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 17th, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _PIEZO
#define _PIEZO

#define POLL_TIMEOUT 10000

#define PIEZO_ADC ADC1
#define PIEZO_ADC_CHANNEL ADC_CHANNEL_11
#define PIEZO_ADC_CLK_EN 	__HAL_RCC_ADC1_CLK_ENABLE()

#define PIEZO_ADC_GPIO_PORT GPIOC
#define PIEZO_ADC_GPIO_PIN GPIO_PIN_1 // Pin C1 is connected to the ADC1 Channel 11 input
#define PIEZO_ADC_GPIO_PORT_CLK_EN __HAL_RCC_GPIOC_CLK_ENABLE();

void adc_piezo_init(void);
float adc_piezo(void);
#endif
