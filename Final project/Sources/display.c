/**
  ******************************************************************************
  * File Name					: display.c
	*	Description				: functions to run a 4 digit 7 segment on gpio
	*	Author						: Stephan Greto-McGrath
	*	Date							: Oct 4th, 2016
  ******************************************************************************
  */
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <stdio.h>
#include <display.h>
#include <interrupts.h>
#define DISPLAY_SIZE 4

////////////////////////////////////////////////////////////////////////////////
//	declarations
////////////////////////////////////////////////////////////////////////////////

char display[DISPLAY_SIZE] = {0};	// string to be displayed updated via the set_display() function
char display_flag = '-';
int time_counter_display = 0; // counter to select digit on display
int over_counter_display = 0; // used to flash display on and off

// thread declarations
osThreadDef(display_thread, osPriorityNormal, 1,0);
osThreadId display_thread_ID;
osSemaphoreId display_semaphore;
osSemaphoreDef(display_semphore);
// function declarations
void digit_sel(int num);
void write_num(int num);
void write_letter(char let);
void display_str(char str[]);
void display_timer_init(void);
void set_dec(void);
void reset_display(void);

////////////////////////////////////////////////////////////////////////////////
//	initilization functions
////////////////////////////////////////////////////////////////////////////////

/** 
* @brief initializes the pins used for the display
*	@param void
* @return void
*/
void display_init(void) {
	GPIO_InitTypeDef GPIO_InitDef; 								
	DISPLAY_PORT_CLK_EN; 									
	GPIO_InitDef.Pin = ALL_PINS; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	HAL_GPIO_Init(DISPLAY_PORT, &GPIO_InitDef);
	HAL_GPIO_WritePin(DISPLAY_PORT, ALL_PINS, GPIO_PIN_RESET);
}


////////////////////////////////////////////////////////////////////////////////
//	threading functions
////////////////////////////////////////////////////////////////////////////////

/** 
	* @brief starts the display thread
	*	@param void *args
	* @return none
	*/
void start_display_thread(void *args) {
	display_thread_ID = osThreadCreate(osThread(display_thread), args);
}

/** 
	* @brief display thread routine
	*	@param void const *args
	* @return none
	*/
void display_thread(void const *args) {

	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		if(display_time_elapsed == 1){
			display_time_elapsed = 0;
			time_counter_display++;		
		}
		osSemaphoreWait(display_semaphore, osWaitForever);
		display_str(display);
		osSemaphoreRelease(display_semaphore);
	}
}

/** 
	* @brief used to set what appears on the display, used from other threads
	*	@param char display_set[] string to be displayed
	* @return void
	*/
void set_display(char display_set[], char flag){
	osSemaphoreWait(display_semaphore, osWaitForever);
	display_flag = flag;
	int i;
	for (i=0; i<DISPLAY_SIZE; i++){
		display[i] = display_set[i];	
	}
	osSemaphoreRelease(display_semaphore);
}

////////////////////////////////////////////////////////////////////////////////
//	display functions
///////////////////////////////////////////////////////	/////////////////////////

/** 
	* @brief prints char* to 7 segment
	*	@param num a char* argument to be displayed on the 4 digit 7 segment
	* @return void
	*/
void display_str(char str[]) {
	
	switch(time_counter_display){
		case 1:
			digit_sel(1);
			write_letter(str[0]);
			break;
		case 2:
			digit_sel(2);
			write_letter(str[1]);
			if (display_flag == 't')set_dec();
			break;
		case 3:
			digit_sel(3);
			write_letter(str[2]);
			if (display_flag == 'a')set_dec();
			break;
		case 4:
			digit_sel(4);
			write_letter(str[3]);
			time_counter_display = 0;
			break;
		default:
			break;
	}
}		

/** 
* @brief based on int input selects a which of the 4 digits of the 7 segment is turned on
*	@param num an int argument (1-4) designating which of the 4 numbers to use
* @return void
*/
void digit_sel(int num){
	HAL_GPIO_WritePin(DISPLAY_PORT, LINE_PINS, GPIO_PIN_RESET); 
	switch(num){
		case 1: 	HAL_GPIO_WritePin(DISPLAY_PORT, PIN_DIG_1, GPIO_PIN_SET);		break;
		case 2:		HAL_GPIO_WritePin(DISPLAY_PORT, PIN_DIG_2, GPIO_PIN_SET);		break;
		case 3:		HAL_GPIO_WritePin(DISPLAY_PORT, PIN_DIG_3, GPIO_PIN_SET);		break;
		case 4:		HAL_GPIO_WritePin(DISPLAY_PORT, PIN_DIG_4, GPIO_PIN_SET);		break;
		default:	HAL_GPIO_WritePin(DISPLAY_PORT, PIN_DIG_4, GPIO_PIN_SET);		
	}	
}

/** 
	* @brief turns on the decimal point segment
	*	@param void
	* @return void
	*/
void set_dec(void){
	HAL_GPIO_WritePin(DISPLAY_PORT, PIN_DEC, GPIO_PIN_SET);
}

/** 
	* @brief turns off the decimal point segment
	*	@param void
	* @return void
	*/
void reset_dec(void){
	HAL_GPIO_WritePin(DISPLAY_PORT, PIN_DEC, GPIO_PIN_RESET);
}

/** 
	* @brief turns off all segments on all digits
	*	@param void
	* @return void
	*/
void reset_display(void){
	switch(time_counter_display){
		case 1:
			digit_sel(1);
			HAL_GPIO_WritePin(DISPLAY_PORT, SEGMENT_PINS, GPIO_PIN_RESET);
			break;
		case 2:
			digit_sel(2);
			HAL_GPIO_WritePin(DISPLAY_PORT, SEGMENT_PINS, GPIO_PIN_RESET);
			break;
		case 3:
			digit_sel(3);
			HAL_GPIO_WritePin(DISPLAY_PORT, SEGMENT_PINS, GPIO_PIN_RESET);
			break;
		case 4:
			digit_sel(4);
		HAL_GPIO_WritePin(DISPLAY_PORT, SEGMENT_PINS, GPIO_PIN_RESET);
			break;
		default:
			break;
	}
}
/** 
* @brief based on int input sets the correct segments to diplay that int
*	@param num an int argument (0-9) designating which number to display
* @return void
*/
void write_num(int num){
	HAL_GPIO_WritePin(DISPLAY_PORT, SEGMENT_PINS, GPIO_PIN_RESET);
	switch(num){
		case 0:		HAL_GPIO_WritePin(DISPLAY_PORT, SEG_0, GPIO_PIN_SET);		break;
		case 1: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_1, GPIO_PIN_SET);		break;
		case 2: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_2, GPIO_PIN_SET);		break;
		case 3: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_3, GPIO_PIN_SET);		break;
		case 4: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_4, GPIO_PIN_SET);		break;
		case 5: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_5, GPIO_PIN_SET);		break;
		case 6: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_6, GPIO_PIN_SET);		break;
		case 7: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_7, GPIO_PIN_SET);		break;
		case 8: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_8, GPIO_PIN_SET);		break;
		case 9: 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_9, GPIO_PIN_SET);		break;
		default:	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_0, GPIO_PIN_SET);
	}
}

/** 
* @brief based on int input sets the correct segments to diplay that char
*	@param let an char argument (0-9 & a-z & A-Z) designating which char to display
* @return void
*/
void write_letter(char let){
	HAL_GPIO_WritePin(DISPLAY_PORT, SEGMENT_PINS, GPIO_PIN_RESET);
	switch(let){
		case 'a':		HAL_GPIO_WritePin(DISPLAY_PORT, SEG_A, GPIO_PIN_SET);		break;
		case 'b': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_B, GPIO_PIN_SET);		break;
		case 'c':		HAL_GPIO_WritePin(DISPLAY_PORT, SEG_C, GPIO_PIN_SET);		break;
		case 'd': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_D, GPIO_PIN_SET);		break;
		case 'e': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_E, GPIO_PIN_SET);		break;
		case 'f': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_F, GPIO_PIN_SET);		break;
		case 'g': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_G, GPIO_PIN_SET);		break;
		case 'h': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_H, GPIO_PIN_SET);		break;
		case 'i': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_I, GPIO_PIN_SET);		break;
		case 'j': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_J, GPIO_PIN_SET);		break;
		case 'k': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_K, GPIO_PIN_SET);		break;
		case 'l': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_L, GPIO_PIN_SET);		break;
		case 'm': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_M, GPIO_PIN_SET);		break;
		case 'n': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_N, GPIO_PIN_SET);		break;
		case 'o': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_O, GPIO_PIN_SET);		break;
		case 'p': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_P, GPIO_PIN_SET);		break;
		case 'q': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_Q, GPIO_PIN_SET);		break;
		case 'r': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_R, GPIO_PIN_SET);		break;
		case 's': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_S, GPIO_PIN_SET);		break;
		case 't': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_T, GPIO_PIN_SET);		break;
		case 'u': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_U, GPIO_PIN_SET);		break;
		case 'v': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_V, GPIO_PIN_SET);		break;
		case 'w': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_W, GPIO_PIN_SET);		break;
		case 'x': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_X, GPIO_PIN_SET);		break;
		case 'y': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_J, GPIO_PIN_SET);		break;
		case 'z': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_J, GPIO_PIN_SET);		break;
		case 'A':		HAL_GPIO_WritePin(DISPLAY_PORT, SEG_A, GPIO_PIN_SET);		break;
		case 'B': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_B, GPIO_PIN_SET);		break;
		case 'C': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_C, GPIO_PIN_SET);		break;
		case 'D': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_D, GPIO_PIN_SET);		break;
		case 'E': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_E, GPIO_PIN_SET);		break;
		case 'F': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_F, GPIO_PIN_SET);		break;
		case 'G': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_G, GPIO_PIN_SET);		break;
		case 'H': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_H, GPIO_PIN_SET);		break;
		case 'I': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_I, GPIO_PIN_SET);		break;
		case 'J': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_J, GPIO_PIN_SET);		break;
		case 'K': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_K, GPIO_PIN_SET);		break;
		case 'L': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_L, GPIO_PIN_SET);		break;
		case 'M': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_M, GPIO_PIN_SET);		break;
		case 'N': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_N, GPIO_PIN_SET);		break;
		case 'O': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_O, GPIO_PIN_SET);		break;
		case 'P': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_P, GPIO_PIN_SET);		break;
		case 'Q': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_Q, GPIO_PIN_SET);		break;
		case 'R': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_R, GPIO_PIN_SET);		break;
		case 'S': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_S, GPIO_PIN_SET);		break;
		case 'T': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_T, GPIO_PIN_SET);		break;
		case 'U': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_U, GPIO_PIN_SET);		break;
		case 'V': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_V, GPIO_PIN_SET);		break;
		case 'W': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_W, GPIO_PIN_SET);		break;
		case 'X': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_X, GPIO_PIN_SET);		break;
		case 'Y': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_J, GPIO_PIN_SET);		break;
		case 'Z': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_J, GPIO_PIN_SET);		break;
		case '0':		HAL_GPIO_WritePin(DISPLAY_PORT, SEG_0, GPIO_PIN_SET);		break;
		case '1': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_1, GPIO_PIN_SET);		break;
		case '2': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_2, GPIO_PIN_SET);		break;
		case '3': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_3, GPIO_PIN_SET);		break;
		case '4': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_4, GPIO_PIN_SET);		break;
		case '5': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_5, GPIO_PIN_SET);		break;
		case '6': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_6, GPIO_PIN_SET);		break;
		case '7': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_7, GPIO_PIN_SET);		break;
		case '8': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_8, GPIO_PIN_SET);		break;
		case '9': 	HAL_GPIO_WritePin(DISPLAY_PORT, SEG_9, GPIO_PIN_SET);		break;
		case '-':																														break;
		default:		HAL_GPIO_WritePin(DISPLAY_PORT, SEG_0, GPIO_PIN_SET);
	}
}
