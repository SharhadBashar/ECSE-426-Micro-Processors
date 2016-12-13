////////////////////////////////////////////////////////////////////////////////
//	File Name					: mouse.c
//	Description				: Example of an OS thread controlling the mouse USB 
//											interface via the on-board pushbutton
//	Author						: Harsh Aurora
//	Date							: Nov 8, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "mouse_thread.h"
#include "cc2500.h"
#include "display.h"

#define SMOOTHING 1
#define THRESHOLD 15
#define CLICK_CODE 95
#define PITCH_CODE 97
#define ROLL_CODE 99
#define DATA_SIZE 3
#define BUFFER_SIZE 30

//		Function Declaration		//
void mouse_thread(void const *args);
void set_click(int click);
int get_click(void);
void set_pitch(float pitch);
void set_roll(float roll);
void mouse_timer_init(void);
void send_data(void);
void receive_data(void);
void send_click(void);
void send_pitch(void);
void send_roll(void);
void check_data(uint8_t buffer);



//		Globals 		//
osThreadId mouse_thread_ID;
osThreadDef(mouse_thread, osPriorityNormal, 1,0);
uint8_t  mouse_in_report[4] = {0,0,0,0};
int function = 0; // sender function = 1, receiver function = 2
uint8_t buffer;
uint8_t data = 0;
// globals used to detect incoming data
uint8_t indicator[DATA_SIZE] = {0}; // pos 1 is click, 2 is pitch, 3 is roll
uint8_t data_buffer[DATA_SIZE] = {0}; // pos 1 is click, 2 is pitch, 3 is roll

/** 
* @brief initilizes everything necessary for the mouse
*	@param void
* @return void
*/
void mouse_init(int func){
	// input gpio initilization
	if (func == 1){
		GPIO_InitTypeDef mouse_GPIO_struct;
		CLICK_BANK_CLK_EN();
		mouse_GPIO_struct.Pin	= BUTTONS;
		mouse_GPIO_struct.Mode = GPIO_MODE_INPUT;
		mouse_GPIO_struct.Pull	= GPIO_PULLDOWN;
		mouse_GPIO_struct.Speed	= GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(CLICK_BANK, &mouse_GPIO_struct);
	}
	function = func;
}


/** 
* @brief creates and runs the mouse thread
*	@param void
* @return void *args unused
*/
void start_mouse_thread(void *args) {
	mouse_thread_ID = osThreadCreate(osThread(mouse_thread), args);
}

/** 
* @brief mouse functionality
*	@param void
* @return void const *args used to initilize the thread
*/
void mouse_thread(void const *args) {
	mouse_in_report[0] = 0;
	mouse_in_report[1] = 0;
	mouse_in_report[2] = 0;
	mouse_in_report[3] = 0;
	int click = 0;
	cc2500_Read(&buffer, 0x34,1);
	while(1){
		osSignalWait(0x00000001, osWaitForever);
		if (function == 1){ // sender
			set_click(get_click());
			set_pitch(get_pitch());
			set_roll(get_roll());
			send_data();
		}else if (function == 2){ // receiver
			receive_data();
			if (data == 1){
				//printf("data");
				set_display("data", 'a');
				data = 0;
				USBD_HID_GetReportTrigger(0, 0, mouse_in_report, 4);
			}else if (data == 0){
				//printf("null");
				set_display("null", 'a');
			}
		}
	}
}

/** 
* @brief sends the mouse data struct using the cc2500
*	@param void
* @return void
*/
void send_data(void){	
	send_click();
	send_pitch();
	send_roll();
}

/** 
* @brief sends the click data struct using the cc2500 with 2 check bits
*	@param void
* @return void
*/
void send_click(void){
	cc2500_SendPacket((uint8_t)CLICK_CODE); // check byte 
	osDelay(50);
	cc2500_SendPacket((uint8_t)mouse_in_report[0]); // data byte 
	osDelay(50);
	cc2500_SendPacket((uint8_t)CLICK_CODE); // check byte 
	osDelay(50);
}

/** 
* @brief sends the pitch data struct using the cc2500 with 2 check bits
*	@param void
* @return void
*/
void send_pitch(void){
	cc2500_SendPacket((uint8_t)PITCH_CODE); // check byte 
	osDelay(50);
	cc2500_SendPacket((uint8_t)mouse_in_report[1]); // data byte
	osDelay(50);
	cc2500_SendPacket((uint8_t)PITCH_CODE); // check byte 
	osDelay(50);
}

/** 
* @brief sends the roll data struct using the cc2500 with 2 check bits
*	@param void
* @return void
*/
void send_roll(void){
	cc2500_SendPacket((uint8_t)ROLL_CODE); // check byte 
	osDelay(50);
	cc2500_SendPacket((uint8_t)mouse_in_report[2]); // data byte 
	osDelay(50);
	cc2500_SendPacket((uint8_t)ROLL_CODE); // check byte 
	osDelay(50);
}



/** 
* @brief fills a buffer with incoming data and calls check_data() to identify and process this information
*	@param void
* @return void
*/
void receive_data(void){
	uint8_t incoming_buffer[BUFFER_SIZE];
	int i;
	for (i=0; i < BUFFER_SIZE; i++){
		cc2500_ReadPacket(&incoming_buffer[i]);
	}
	for (i=0; i < BUFFER_SIZE; i++){
		check_data(incoming_buffer[i]);
	}
}
/** 
* @brief receives the mouse data struct using the cc2500 and verifies the 2 check bits
*	@param void
* @return void
*/
void check_data(uint8_t buffer){
	int i;
	////////////////////////////////////////// 
	// click detect
	//////////////////////////////////////////
	if (buffer == CLICK_CODE && indicator[0]!= 1){
		// reset
		for (i=0; i < DATA_SIZE; i++){
			indicator[i] = 0;
			data_buffer [i] = 0;
		}
		indicator[0] = 1;
	}
	if (indicator[0] == 1 && buffer !=0 && buffer != CLICK_CODE){
		data_buffer[0] = buffer;
	}
	if (data_buffer[0] != 0 && buffer == CLICK_CODE){
		data=1;
		mouse_in_report[0] = data_buffer[0];
	}
	////////////////////////////////////////// 
	// pitch detect
	//////////////////////////////////////////
	/*
	if (buffer == PITCH_CODE && indicator[1]!= 1){
		// reset
		for (i=0; i < DATA_SIZE; i++){
			indicator[i] = 0;
			data_buffer [i] = 0;
		}
		indicator[1] = 1;
	}
	if (indicator[1] == 1 && buffer !=0 && buffer != PITCH_CODE){
		data_buffer[1] = buffer;
	}
	if (data_buffer[1] != 0 && buffer == PITCH_CODE){
		data=1;
		mouse_in_report[1] = data_buffer[1];
	}
	*/
	////////////////////////////////////////// 
	// roll detect
	//////////////////////////////////////////
	/*
	if (buffer == ROLL_CODE && indicator[2]!= 1){
		// reset
		for (i=0; i < DATA_SIZE; i++){
			indicator[i] = 0;
			data_buffer [i] = 0;
		}
		indicator[2] = 1;
	}
	if (indicator[2] == 1 && buffer !=0 && buffer != ROLL_CODE){
		data_buffer[2] = buffer;
	}
	if (data_buffer[2] != 0 && buffer == ROLL_CODE){
		data=1;
		mouse_in_report[2] = data_buffer[2];
	}
	*/
}

/** 
* @brief sets the appropriate bits based on the mouse GPIO reading
*	@param int click the number corresponds to which button has been pressed
* @return void
*/
void set_click(int click){
	switch(click){
		case 0: // no button has been pressed
			mouse_in_report[0] = 0x00;
			break;
		case 1: // set first bit for left left
			mouse_in_report[0] = 0x01;
			break;
		case 2: // set second bit for right click
			mouse_in_report[0] = 0x02;
			break;
		case 3: // set third bit for middle click
			mouse_in_report[0] = 0x04;
			break;
		default:
			break;
	}
}

/** 
* @brief reads all GPIOS to see which one has been clicked (no debounce)
*	@param void
* @return int the number corresponds to which button has been clicked (1 = left, 2 = right, 3 = middle)
*/
int get_click(void){
	if(HAL_GPIO_ReadPin(CLICK_BANK, BUTTON_1) == SET){
		return 1;
	}else if(HAL_GPIO_ReadPin(CLICK_BANK, BUTTON_2) == SET){ 
		return 2;
	}else if(HAL_GPIO_ReadPin(CLICK_BANK, BUTTON_3) == SET){
		return 3;
	}else{
		return 0;
	}
}


/** 
* @brief determines the speed and direction in which the mouse moves side to side (pitch > 20 initiates movement)
*	@param float pitch the boards position in space
* @return void
*/
void set_pitch(float pitch){
	if (pitch > THRESHOLD && pitch < 180){ // left
		mouse_in_report[1] = (int8_t)((0 - (pitch-THRESHOLD))/SMOOTHING);
	}else if (pitch < (360 - THRESHOLD) && pitch > 180){ // right
		mouse_in_report[1] = (int8_t)((360-(pitch+THRESHOLD))/SMOOTHING);
	}else{	// not moving
		mouse_in_report[1] = 0; 
	}
}

/** 
* @brief determines the speed and direction in which the mouse moves up and down (roll > 20 initiates movement)
*	@param float pitch the boards position in space
* @return void
*/
void set_roll(float roll){
		if (roll > THRESHOLD && roll < 180){ // up
		mouse_in_report[2] = (int8_t)((0 - (roll-THRESHOLD))/SMOOTHING);
	}else if (roll < (360 - THRESHOLD) && roll > 180){ // down
		mouse_in_report[2] = (int8_t)((360-(roll+THRESHOLD))/SMOOTHING);
	}else{	// not moving
		mouse_in_report[2] = 0; 
	}

}



