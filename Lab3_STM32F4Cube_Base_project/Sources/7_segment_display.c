////////////////////////////////////////////////////////////////////////////////
//	File Name				: 7_segment_display.c
//	Description			: Provides definition & function to support the 7 Seg Display
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Oct 24th, 2016
////////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "7_Segment_Display.h"
#include "stm32f4xx_hal_def.h"

 

void init_Display_Pins(void) {
	GPIO_InitTypeDef GPIO_InitDef; 		
	SEGMENT_PORT_CLK_EN;
	
	
	GPIO_InitDef.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_HIGH;

	
	
	HAL_GPIO_Init(SEGMENT_PORT, &GPIO_InitDef);

	
	HAL_GPIO_WritePin(SEGMENT_PORT, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
}

//performs the mx function to enable a certain digit
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
				
			case 10://C
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); 
			
			case 11://n
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
			
			case 12://A
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 
			
			default:
				break;
		}
}
//undisplay the decimal points
void undisplay_Decimal(void){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
}
//display the decimal points
void display_Decimal (void) {
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
}
//display the degee point
void display_Degrees (void) {
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 			
}
//funciton that displays the number
void display_Number(number,time_counter,modeap){
	int numArray[3];
	if (number > 999){
		number = number - 1000*(int)(number/1000);
	}
	numArray[0] = (int)(number/100); //breaks up the number in a array of size 4
	numArray[1] = (int)(number/10 - numArray[0]*10);
	numArray[2] = (int)(number - (numArray[0]*100 + numArray[1]*10));
	//numArray[3] = (int)(number - (numArray[0]*1000 + numArray[1]*100 + numArray[2]*10));

		if (time_counter % 4 == 0){ //displays the first number
			enable_Digit(1);
			write_Number(numArray[0]);
			undisplay_Decimal();
		}
		else if (time_counter % 4 == 1){ //display the second number
			enable_Digit(2);
			write_Number(numArray[1]);
			display_Decimal();				
		}
		else if (time_counter % 4 == 2){ //display the third number
			enable_Digit(3);
			undisplay_Decimal();
			write_Number(numArray[2]);
			
		}	
		else if (time_counter % 4 == 3 && (modeap == 1 || modeap == 2)){ //display the A or P
			enable_Digit(4);
			undisplay_Decimal();
			if (modeap == 2){
				write_Number(11);
			}
			else if (modeap == 1){
				write_Number(12);
			}
		}			

}





