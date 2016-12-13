////////////////////////////////////////////////////////////////////////////////
//	File Name					: 7_segment_display.h
//	Description				: header file for 7_segment_display.c
//	Course						: ECSE 426
//	Authors						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _7_SEGMENT_DISPLAY
#define _7_SEGMENT_DISPLAY

#include <stdio.h>
#include "stm32f4xx_hal.h"

#define SEGMENT_PORT GPIOE
//#define SELECT_PORT GPIOB
//#define SEGMENT_PORT GPIOE
//#define SEGMENT_A GPIOE_PIN_2  //segment a
//#define SEGMENT_B GPIOE_PIN_3  //segment b
//#define SEGMENT_C GPIOE_PIN_4  //segment c no
//#define SEGMENT_D GPIOE_PIN_5 //segment d
//#define SEGMENT_E GPIOE_PIN_6 //segment e 
//#define SEGMENT_F GPIOE_PIN_7 //segment f
//#define SEGMENT_G GPIOE_PIN_8 //segment g
//#define SEGMENT_DP GPIOE_PIN_9 //segment dp

//#define SEGMENT_DEGREE GPIOE_PIN_10 //L3

//#define SELECT_1 GPIOB_PIN_11 //select 1
//#define SELECT_2 GPIOB_PIN_12 //select 2
//#define SELECT_3 GPIOB_PIN_13 //select 3
//#define SELECT_4 GPIOB_PIN_14 //select 4

#define SEGMENT_PORT_CLK_EN __HAL_RCC_GPIOE_CLK_ENABLE();
//#define SELECT_PORT_CLK_EN __HAL_RCC_GPIOB_CLK_ENABLE();
//#define L3_Port_CLK_EN __HAL_RCC_GPIOE_CLK_ENABLE();

void init_Display_Pins(void);
void enable_Digit(int digit);
void write_Number(int value);
void display_Decimal(void);
void undisplay_Decimal(void);
void display_Number (int number);
void display_Degrees(void);
int extract_Hundreds (int value);
int extract_Tens(int value);
int exact_Ones(int value);
int extract_Tenth (float value);

#endif