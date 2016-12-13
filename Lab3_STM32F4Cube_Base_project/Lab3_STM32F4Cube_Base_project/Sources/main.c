////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Course						: ECSE 426
//	Description				: Program Entry
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <key_pad.h>
#include <piezo.h>
#include <accelerometer.h>
#include <timer.h>
#include <7_segment_display.h>
#include <kalman.h>

enum {
	ACCEL, 
	PIEZO,
	INPUT
}mode;

//int col;
//int key; 
int key_val = 0;
int key_val_num = 0;
uint8_t acc_flag = 0;
uint8_t tim_flag = 0;
float axes_vals[3] = {0, 0 , 0};
int time_counter = 0;
int time_count = 0;
float piezoVal;
int numArray[4];
int number = 0000;
float offsetx, offsety, offsetz;
float x, y, z;

struct kalman_state kstatex;
struct kalman_state kstatey;
struct kalman_state kstatez;

int main(void) {
	//Initialize everything
	HAL_Init();
	SystemClock_Config();
	timer_init();
	adc_piezo_init();
	acc_init();
	keypad_init();
	init_Display_Pins();
	acc_calibrate();
	kalman_init(&kstatex, 1.0, 1.0, 0.02, 0.10, 0.0, 2.00);
	kalman_init(&kstatey, 1.0, 1.0, 0.02, 0.10, 0.0, 2.00);
	kalman_init(&kstatez, 1.0, 1.0, 0.02, 0.10, 0.0, 2.00);
	//Variable declaration
	
	//HAL_delay(1000);
	//mode = ACCEL;

	int time_counter = 0;
	while (1){
		if (tim_flag == 1){
			tim_flag = 0;
			
			key_val = keypad_read();
			
			switch(key_val){
				default:
					mode = INPUT;
					break;		
				
				case -1: 
					mode = ACCEL;
					break;
				
				case -2:
					mode = PIEZO;
					break;

			}
		}
		
		if (acc_flag == 1 && mode == ACCEL){
			acc_flag = 0;
			if (time_count ++== 1){
				time_count =0;
				LIS3DSH_ReadACC(axes_vals);
				x = Kalmanfilter_C(axes_vals[0] - offsetx, &kstatex);
				y = Kalmanfilter_C(axes_vals[1] - offsety, &kstatey);
				z = Kalmanfilter_C(axes_vals[2] - offsetz, &kstatez);					
				printf("Pitch: %f Roll: %f \n", pitch_calc(x,y,z) ,roll_calc(x,y,z));	
			}

		}
		if (mode == PIEZO){
			piezoVal = adc_piezo();
			if (piezoVal > 1.00f){
				printf("Force: %f\n", piezoVal);
			}				
		}
		if (mode == INPUT){			
			key_val = keypad_read();
			display_Number(number,time_counter);
			switch(key_val){
				
								
				case 10:
					//enable_Digit(1);
					key_val_num = keypad_read();
					number = number + num_display(key_val_num)*1000;
					break;
				
				case 11:
					//enable_Digit(2);
					key_val_num = keypad_read();
					number = number + num_display(key_val_num)*100;
					break;
				
				case 12:
					//enable_Digit(3);
					key_val_num = keypad_read();
					number = number + num_display(key_val_num)*10;
					break;
				
				case 13:
					//enable_Digit(4);
					key_val_num = keypad_read();
					number = number + num_display(key_val_num);
					break;
			}
			time_counter++;
				
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim -> Instance ==TIM3){
		tim_flag = 1;
		//or set a flag, which is reset, just like with sistick
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t Gpio_Pin){
	if (Gpio_Pin == GPIO_PIN_0||Gpio_Pin == GPIO_PIN_1){
		acc_flag = 1;
	}
}

void acc_calibrate(void){
	int sumx, sumy, sumz;
	int counter = 1;
	while(counter <= 30){
		if (acc_flag == 1){
				acc_flag = 0;
				if (counter == 1) {
					printf("Calibrating Accelerometer. \n");
				}
				if (time_count ++== 1){
					time_count =0;
					LIS3DSH_ReadACC(axes_vals);
					sumx += axes_vals[0];
					sumy += axes_vals[1];
					sumz += axes_vals[2];
					counter++;
				}
		}
	}
	
	offsetx = sumx/(float)counter;
	offsety = sumy/(float)counter;
	offsetz = sumz/(float)(counter-1);

}
