////////////////////////////////////////////////////////////////////////////////
//	File Name					: LED_Control.h
//	Description				: Header file for LED_Control.c
//	Course						: ECSE 426
//	Authors						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _LED_CONTROL_H
#define _LED_CONTROL_H


#define LED_Port GPIOD
#define PIN_1 GPIO_PIN_12 
#define PIN_2 GPIO_PIN_13 
#define PIN_3 GPIO_PIN_14 
#define PIN_4 GPIO_PIN_15 
#define LED_Port_CLK_EN __HAL_RCC_GPIOD_CLK_ENABLE();

void LED_Control_init(void);
void gpio_LED_Control(void);
void display_Single_LED(int LED_count);

#endif