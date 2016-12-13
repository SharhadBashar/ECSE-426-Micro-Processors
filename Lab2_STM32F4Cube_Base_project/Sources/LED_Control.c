////////////////////////////////////////////////////////////////////////////////
//	File Name					: LED_Control.h
//	Description				: Demonstrates control of on board LED
//	Course						: ECSE 426
//	Authors						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <LED_Control.h>

void LED_Control_init(void){ 
	/**
	Onboard GPIO LED Control Function For Alarm
	Takes in no arguments.
	Returns nothing.
	**/ 
	
	//Enabling LED clock and defining GPIO Initialization Type
	GPIO_InitTypeDef GPIO_InitDef; 								
	LED_Port_CLK_EN; 									
	
	//Defining GPIO LED pins.
	GPIO_InitDef.Pin = PIN_1|PIN_2|PIN_3|PIN_4; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	
	//Initializing the pins
	HAL_GPIO_Init(LED_Port, &GPIO_InitDef);
	HAL_GPIO_WritePin(LED_Port, PIN_1|PIN_2|PIN_3|PIN_4, GPIO_PIN_RESET);
	
}
void gpio_LED_Control(void){//Toggles onboard GPIO LEDs on and off
	HAL_GPIO_TogglePin(LED_Port, PIN_1|PIN_2|PIN_3|PIN_4);
}

void display_Single_LED(int LED_count){//Initiates alarm (clockwise rotation on onboard GPIO LEDs
 if (LED_count % 4 == 0){
				HAL_GPIO_WritePin (GPIOD,PIN_1,GPIO_PIN_SET);
				HAL_GPIO_WritePin (GPIOD,PIN_2|PIN_3|PIN_4,GPIO_PIN_RESET);
			}
 if (LED_count % 4 == 1){
				HAL_GPIO_WritePin (GPIOD,PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin (GPIOD,PIN_1|PIN_3|PIN_4,GPIO_PIN_RESET);
			}
 if (LED_count % 4 == 2){
				HAL_GPIO_WritePin (GPIOD,PIN_3,GPIO_PIN_SET);
				HAL_GPIO_WritePin (GPIOD,PIN_1|PIN_2|PIN_4,GPIO_PIN_RESET);
			}
 if (LED_count % 4 == 3){
				HAL_GPIO_WritePin (GPIOD,PIN_4,GPIO_PIN_SET);
				HAL_GPIO_WritePin (GPIOD,PIN_1|PIN_2|PIN_3,GPIO_PIN_RESET);
			}
}