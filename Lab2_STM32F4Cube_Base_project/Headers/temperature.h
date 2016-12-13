////////////////////////////////////////////////////////////////////////////////
//	File Name					: temperature.h
//	Description				: header file for temperature.c
//	Course						: ECSE 426
//	Authors						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H

#define POLL_TIMEOUT 10000

#define TEMP_ADC ADC1
#define TEMP_ADC_CHANNEL ADC_CHANNEL_16
#define TEMP_ADC_CLK_EN 	__HAL_RCC_ADC1_CLK_ENABLE()

#define TEMP_ADC_GPIO_PORT GPIOA
#define TEMP_ADC_GPIO_PIN GPIO_PIN_5 // Pin A5 is connected to the ADC1 Channel 5 input
#define TEMP_ADC_GPIO_PORT_CLK_EN __HAL_RCC_GPIOA_CLK_ENABLE();

void adc_Temp_Init(void);
float vol_To_Temp(float voltage);
float get_Data_Sensor(void);
#endif