////////////////////////////////////////////////////////////////////////////////
//	File Name				: 7_GPIO_PIN_5isplay.c
//	Description			: Provides definition & function to support the 7 Seg Display
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "7_Segment_Display.h"
#include <sysclk_config.h>

 

void init_Display_Pins(void) {
	GPIO_InitTypeDef GPIO_InitDef; 		
	SEGMENT_PORT_CLK_EN;
	
	
	GPIO_InitDef.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_HIGH;

	//GPIO_InitDef.Pin = select1|select2|select3|select4; 	
	//GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	//GPIO_InitDef.Pull = GPIO_NOPULL;
	//GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;	
	
	HAL_GPIO_Init(SEGMENT_PORT, &GPIO_InitDef);
//	HAL_GPIO_Init(SELECT_PORT, &GPIO_InitDef);
	
	HAL_GPIO_WritePin(SEGMENT_PORT, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(SELECT_PORT,SELECT_1|SELECT_2|SELECT_3|SELECT_4, GPIO_PIN_RESET);
}

void enable_Digit (int digit) {
	switch (digit) {
		case 1: 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
			break;
		case 2: 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
			break;
		case 3: 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
			break;
		case 4: 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
			break;	
		
		case 11: 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
			break;	
	}
}

void write_Number(int value) {
	//Write number that corresponds to value from 0-9
	switch(value) {
			case 0:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); 
				break;
			
			case 1:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); 
				break;

			case 2:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 
				break;
			
			case 3:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 
				break;
			
			case 4:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 			
				break;
			
			case 5:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 
				break;
			
			case 6:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 
				break;

			case 7:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); 
				break;
			
			case 8:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 
				break;
			
			case 9:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 			
				break;
				
			case 10://a case for values greater than 9999
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); 
		}
}

void undisplay_Decimal(void){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
}

void display_Decimal (void) {
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
}

void display_Degrees (void) {
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 			
}

void display_Number (int out){
	//HAL_Init();
	//volatile int systick_flag = 1;
	int time_counter = 0;
	int numArray[3];
	int number = out*10;
	numArray[0] = (int)(number/100);
	numArray[1] = (int)(number/10 - numArray[0]*10);
	numArray[2] = (int)(number - (numArray[0]*100 + numArray[1]*10));
	
	while (time_counter < 10000){	
		
		//if(systick_flag == 1) { // Please look in stm32f4xx_it.c to understand how this variable is set and used
		//		systick_flag = 0;
			 
				display_Degrees();
				if (time_counter % 3 == 0){
					enable_Digit(1);
					write_Number(numArray[0]);
					undisplay_Decimal();
				}
				else if (time_counter % 3 == 1){
					enable_Digit(2);
					write_Number(numArray[1]);
					display_Decimal();				
				}
				else if (time_counter % 3 == 2){
					undisplay_Decimal();
					enable_Digit(3);
					write_Number(numArray[2]);
				}						
			//}
			
			
			
		time_counter++;	
	}
}





int extract_Hundreds (int value){
	return value/100;
}
		
int extract_Tens (int value){
			if (value >= 100)
				value = (value/10) % 10;
			else
				value = value/10;
			return value;
}
		
int extract_Ones (int value){
			return value % 10;
}

int extract_Tenth (float value){
	value = value*10;
	return (int)value % 10;
}




