////////////////////////////////////////////////////////////////////////////////
//	File Name					: mkey_pad.h
//	Description				: header file for key_pad.c
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 17th, 2016
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
/**
	*	GPIO ports
	*/
#define row_GPIO GPIOC
#define col_GPIO GPIOC

#define row_GPIO_CLK_EN __HAL_RCC_GPIOE_CLK_ENABLE();
#define col_GPIO_CLK_EN __HAL_RCC_GPIOE_CLK_ENABLE();

/**
	*	GPIO PINs
	*/
	
#define col_bit_0 GPIO_PIN_2
#define col_bit_1 GPIO_PIN_3
#define col_bit_2 GPIO_PIN_4
#define col_bit_3 GPIO_PIN_5

#define row_bit_0 GPIO_PIN_6
#define row_bit_1 GPIO_PIN_7
#define row_bit_2 GPIO_PIN_8
#define row_bit_3 GPIO_PIN_9	

#define STAR -1
#define POUND -2
#define A 10
#define B 11
#define C 12
#define D 13

#define NO_KEY_PRESSED -3

static int map[4][4] = {
	{1,			2,			3, 10},
	{4,			5,			6, 11},
	{7,			8,			9, 12},
	{STAR,	0,	POUND, 13}
};

//	{1,			2,			3},
//	{4,			5,			6},
//	{7,			8,			9},
//	{STAR,	0,	POUND}
//funcitons
extern int keypad_read();
extern int set_col_read(uint16_t GPIO_Pin);
extern int read_sequence(int* values, char selected);
extern void whileNotReleased(void);

extern void keypad_init(void);
int num_display(int key_pad_num);

