////////////////////////////////////////////////////////////////////////////////
//	File Name					: gpio_example.c
//	Description				: Sample code to help use the GPIO driver
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <gpio_example.h>

/*Brief: Sets up the desired(refer to header) GPIO pin to output mode
**			 and initializes its value to 0
**Params: None
**Return: None
*/
void gpio_example_init(void) {
	GPIO_InitTypeDef GPIO_InitDef; 								
	EXAMPLE_PORT_CLK_EN; 									
	
	GPIO_InitDef.Pin = EXAMPLE_PIN; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	
	HAL_GPIO_Init(EXAMPLE_PORT, &GPIO_InitDef);
	HAL_GPIO_WritePin(EXAMPLE_PORT, EXAMPLE_PIN, GPIO_PIN_RESET);
}

/*Brief: Toggles the value of the desired pin
**Params: None
**Return: None
*/
void gpio_example(void) {
	HAL_GPIO_TogglePin(EXAMPLE_PORT, EXAMPLE_PIN);
}

