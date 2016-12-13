/**
  ******************************************************************************
  * File Name					: keypad.c
	*	Description				: contains function for initializing  & polling keypad
	*	Author						: Stephan Greto-McGrath
	*	Date							: Oct 4th, 2016
  ******************************************************************************
  */
	
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <stdio.h>
#include <interrupts.h>
#include <keypad.h>

////////////////////////////////////////////////////////////////////////////////
//	declarations
////////////////////////////////////////////////////////////////////////////////

// declare functions to be used withing this file
void keypad_input(uint16_t GPIO_Pin);
void keypad_output(uint16_t GPIO_Pin);
int read_col(void);
int read_row(void);
int concantenate(int x, int y);
char get_letter(int code);
void keypad_timer_init(void);
char keypad_poll(void);
char read_keypad(void);

// threading declarations
osThreadDef(keypad_thread, osPriorityNormal, 1,0);
osThreadId keypad_thread_ID;
osSemaphoreId key_semaphore;
osSemaphoreDef(key_semphore);

// globals for reading chars from keypad
char input = '-';								// final value to be read
char reading = '-';							// value used for debounce function
char prev_reading = '-';				// value used fo debounce function



////////////////////////////////////////////////////////////////////////////////
//	initilization functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief initializes the keypad
	* @param void
	*	@return void
	*/
void keypad_init(void){
	KEY_PORT_CLK_EN; 
	keypad_input(COL_KEYS);
	keypad_output(ROW_KEYS);
}

/** 
	* @brief sets gpios as output
	*	@param uint16_t GPIO_Pin GPIO pin(s) to set as output
	* @return void
	*/
void keypad_output(uint16_t GPIO_Pin){
	GPIO_InitTypeDef GPIO_InitDef; 			
	GPIO_InitDef.Pin = GPIO_Pin; 
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_LOW;	
	HAL_GPIO_Init(KEY_PORT, &GPIO_InitDef);
	HAL_GPIO_WritePin(KEY_PORT, GPIO_Pin, GPIO_PIN_SET);	
}

/** 
	* @brief sets gpios as input
	*	@param uint16_t GPIO_Pin GPIO pin(s) to set as input
	* @return void
	*/
void keypad_input(uint16_t GPIO_Pin){
	GPIO_InitTypeDef GPIO_InitDef; 			
	GPIO_InitDef.Pin = GPIO_Pin; 
	GPIO_InitDef.Mode = GPIO_MODE_INPUT; 
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_LOW;	
	HAL_GPIO_Init(KEY_PORT, &GPIO_InitDef);
}




////////////////////////////////////////////////////////////////////////////////
//	threading functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief starts the keypad thread
	*	@param void *args
	* @return void
	*/
void start_keypad_thread(void *args) {
	keypad_thread_ID = osThreadCreate(osThread(keypad_thread), args);
}

/** 
	* @brief keypad thread routine
	*	@param void const *args
	* @return void
	*/
void keypad_thread(void const *args) {
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		if(keypad_time_elapsed == 1){
			keypad_time_elapsed = 0;
			osSemaphoreWait(key_semaphore, osWaitForever);
			input = read_keypad();
			osSemaphoreRelease(key_semaphore);
		}
	}
}

/** 
	* @brief function to retrieve keypad input
	*	@param void
	* @return char input returns the debounced keypad input
	*/
char get_key(void){
	osSemaphoreWait(key_semaphore, osWaitForever);
	char key = input;
	osSemaphoreRelease(key_semaphore);
	return key;
}


////////////////////////////////////////////////////////////////////////////////
//	reading functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief polls the keypad using a single sweep
	*	@param void
	* @return returns the char being pressed pr '-' if not pressed
	*/
char keypad_poll(void){
	int col, row = 0;
	char letter = '-';
	HAL_GPIO_WritePin(KEY_PORT, ROW_KEYS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_PORT, ROW_1, GPIO_PIN_SET);
	col = read_col();
	if (col!=0){
		row = 1;
		letter = get_letter(concantenate(row,col));
		HAL_GPIO_WritePin(KEY_PORT, ROW_KEYS, GPIO_PIN_SET);
		return letter;
	}
	HAL_GPIO_WritePin(KEY_PORT, ROW_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_PORT, ROW_2, GPIO_PIN_SET);
	col = read_col();
	if (col!=0){
		row = 2;
		letter = get_letter(concantenate(row,col));
		HAL_GPIO_WritePin(KEY_PORT, ROW_KEYS, GPIO_PIN_SET);
		return letter;
	}
	HAL_GPIO_WritePin(KEY_PORT, ROW_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_PORT, ROW_3, GPIO_PIN_SET);
	col = read_col();
	if (col!=0){
		row = 3;
		letter = get_letter(concantenate(row,col));
		HAL_GPIO_WritePin(KEY_PORT, ROW_KEYS, GPIO_PIN_SET);
		return letter;
	}
	HAL_GPIO_WritePin(KEY_PORT, ROW_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_PORT, ROW_4, GPIO_PIN_SET);
	col = read_col();
	if (col!=0){
		row = 4;
		letter = get_letter(concantenate(row,col));
		HAL_GPIO_WritePin(KEY_PORT, ROW_KEYS, GPIO_PIN_SET);
		return letter;
	}
	return letter;
}

/** 
	* @brief reads the columns
	*	@param void
	*	@return int returns the number of the column being pressed (1-4)
	*/
int read_col(void){
	if(HAL_GPIO_ReadPin(KEY_PORT, COL_1)!=0){
		return 1;
	}
	if(HAL_GPIO_ReadPin(KEY_PORT, COL_2)!=0){
		return 2;
	}
	if(HAL_GPIO_ReadPin(KEY_PORT, COL_3)!=0){
		return 3;
	}
	if(HAL_GPIO_ReadPin(KEY_PORT, COL_4)!=0){
		return 4;
	}
	return 0;
}

/** 
	* @brief esstially just a debounce for polling the keypad
	* @param void
	* @return char returns a debounced char
	*/
char read_keypad(void){
	char keypad_reading = '-';
	reading = keypad_poll();
	if (reading!= '-' && reading != prev_reading){
		keypad_reading = reading;
	}
	if (reading != '-' && prev_reading == '-'){
		keypad_reading = reading;
	} 
	return keypad_reading;

}


////////////////////////////////////////////////////////////////////////////////
//	miscellaneous helper functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief concatenates two integers
	*	@param int x first int
	*	@param int y second int
	* @return int returns int xy
	*/
int concantenate(int x, int y) {
    int pow = 10;
    while (y >= pow){
        pow *= 10;
		}
    return x * pow + y;
}

/** 
	* @brief takes a code for the keypad and return its corresponding letter
	*	@param int code  is the letter's place in the keypad matrix
	* @return char returns the char corresponding to the input code
	*/
char get_letter(int code){
	switch(code){
		case 11:		return '1';	
		case 12:		return '2';	
		case 13:		return '3';	
		case 14:		return 'A';	
		case 21:		return '4';	
		case 22:		return '5';	
		case 23:		return '6';	
		case 24:		return 'B';	
		case 31:		return '7';	
		case 32:		return '8';	
		case 33:		return '9';	
		case 34:		return 'C';	
		case 41:		return '*';	
		case 42:		return '0';	
		case 43:		return '#';	
		case 44:		return 'D';	
		default:		return '-';	
	}
}


