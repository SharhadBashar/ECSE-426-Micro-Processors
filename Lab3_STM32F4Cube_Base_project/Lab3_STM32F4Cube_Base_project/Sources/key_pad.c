////////////////////////////////////////////////////////////////////////////////
//	File Name					: piezo.c
//	Description				: Code to use the key pad
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 17th, 2016
////////////////////////////////////////////////////////////////////////////////



#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "key_pad.h"


static GPIO_InitTypeDef  GPIO_InitStructure;

char select;    //button pressed
uint8_t readrow = 0;
uint8_t readcol = 0;
int rowIndex, colIndex;
int key_found = 0;
int key_number;
/// <summary>
///	Initializes the keypad
/// </summary>
void keypad_init(void){

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*GPIO settinsgs
	GPIO_Pin_xx which specific port pins will be used
	GPIO_Mode how the pins will be used: We set the Mode to be Out
	GPIO_Speed at what port lock speed should they operate: We set it up at 50 MHz
	GPIO_OType what is their output type
	GPIO_PuPD how should the pin output be configured we have set the 
	value to Down
	*/
	row_GPIO_CLK_EN;
	col_GPIO_CLK_EN;
	
	GPIO_InitStructure.Pin = row_bit_0 | row_bit_1 | row_bit_2 | row_bit_3 ;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(row_GPIO, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOE,row_bit_0 | row_bit_1 | row_bit_2 | row_bit_3 , GPIO_PIN_SET);
	
	/* GPIO settings for Coloumn
	*/

	GPIO_InitStructure.Pin = col_bit_0 | col_bit_1 | col_bit_2 | col_bit_3;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(col_GPIO, &GPIO_InitStructure);
	
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_SetPriority(EXTI2_IRQn,1,2);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_SetPriority(EXTI3_IRQn,1,2);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_SetPriority(EXTI4_IRQn,1,2);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,2);
}


/*Read a value from the keypad
The value read by the keypad
*/

int keypad_read(){
	
	HAL_GPIO_WritePin(GPIOC,row_bit_0 | row_bit_1 | row_bit_2 | row_bit_3 , GPIO_PIN_RESET);
	
	key_found = set_col_read(row_bit_0);
	if(key_found == 1){
		HAL_GPIO_WritePin(GPIOC,row_bit_0 | row_bit_1 | row_bit_2 | row_bit_3 , GPIO_PIN_RESET);
		return map[rowIndex][colIndex];
	}
	
	key_found = set_col_read(row_bit_1);
	if(key_found == 1){
		HAL_GPIO_WritePin(GPIOC,row_bit_0 | row_bit_1 | row_bit_2 | row_bit_3 , GPIO_PIN_RESET);
		return map[rowIndex][colIndex];
	}
	
	key_found = set_col_read(row_bit_2);
	if(key_found == 1){
		HAL_GPIO_WritePin(GPIOC,row_bit_0 | row_bit_1 | row_bit_2 | row_bit_3 , GPIO_PIN_RESET);
		return map[rowIndex][colIndex];
	}
	
	key_found = set_col_read(row_bit_3);
	if(key_found == 1){
		HAL_GPIO_WritePin(GPIOC,row_bit_0 | row_bit_1 | row_bit_2 | row_bit_3 , GPIO_PIN_RESET);
		return map[rowIndex][colIndex];
	}
	
	return 42;
}

///	Set the pins to read the columns

int set_col_read(uint16_t GPIO_Pin){
	HAL_GPIO_WritePin(GPIOC, GPIO_Pin, GPIO_PIN_SET);
	
	switch(GPIO_Pin){
		case row_bit_0:
			rowIndex = 0;
			break;
		
		case row_bit_1:
			rowIndex = 1;
			break;
		
		case row_bit_2:
			rowIndex = 2;
			break;
		
		case row_bit_3:
			rowIndex = 3;
			break;
		
		default:
			break;
	}
	
	if(HAL_GPIO_ReadPin(GPIOC, col_bit_0) == GPIO_PIN_SET){
		while(HAL_GPIO_ReadPin(GPIOC, col_bit_0) == GPIO_PIN_SET){}
		colIndex = 0;
		return 1;
	}else if(HAL_GPIO_ReadPin(GPIOC, col_bit_1) == GPIO_PIN_SET){
		while(HAL_GPIO_ReadPin(GPIOC, col_bit_1) == GPIO_PIN_SET){}
		colIndex = 1;
		return 1;
	}else if(HAL_GPIO_ReadPin(GPIOC, col_bit_2) == GPIO_PIN_SET){
		while(HAL_GPIO_ReadPin(GPIOC, col_bit_2) == GPIO_PIN_SET){}
		colIndex = 2;
		return 1;
	}else if(HAL_GPIO_ReadPin(GPIOC, col_bit_3) == GPIO_PIN_SET){
		while(HAL_GPIO_ReadPin(GPIOC, col_bit_3) == GPIO_PIN_SET){}
		colIndex = 3;
		return 1;
	}
	
	return 0;
}

int num_display (int key_pad_num){
		switch(key_pad_num){
			case 1:
				key_number = 1;
			break;
			
			case 2:
				key_number = 2;
			break;
			
			case 3:
				key_number = 3;
			break;
			
			case 4:
				key_number = 4;
			break;
			
			case 5:
				key_number = 5;
			break;
			
			case 6:
				key_number = 6;
			break;
			
			case 7:
				key_number = 7;
			break;
			
			case 8:
				key_number = 8;
			break;
			
			case 9:
				key_number = 9;
			break;
			
			case 0:
				key_number = 0;
			break;
		}
}
