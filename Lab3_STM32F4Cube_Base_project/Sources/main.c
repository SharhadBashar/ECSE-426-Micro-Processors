////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Course						: ECSE 426
//	Description				: Program Entry
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 24th, 2016
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

// three modes, to be selected using keypad
enum {
	ACCEL, 
	PIEZO,
	INPUT,
	GAME
}mode;


//variable delaration
int key_val = 0;
int key_val_num = 0;
uint8_t acc_flag = 0;
uint8_t tim_flag3 = 0;
uint8_t tim_flag2 = 0;
uint8_t tim_flag4 = 0;
float axes_vals[3] = {0, 0 , 0};
int time_counter = 0;
int time_count = 0;
float piezoVal, piezoValLast;
float max = 0.00;
int pulse_end = 0;

int number = 0;
float offsetx, offsety, offsetz;
float x, y, z;

struct kalman_state kstatex;
struct kalman_state kstatey;
struct kalman_state kstatez;

int main(void) {
	//Initialize everything
	HAL_Init();
	SystemClock_Config();
	timer_init2();
	timer_init3();
	timer_init4();
	float	finalPiezo;
	adc_piezo_init();
	acc_init();
	keypad_init();
	init_Display_Pins();
	acc_calibrate();
	int modeap = 0;
	int target;
	float pitch; 
	float piezo_target;
	kalman_init(&kstatex, 1.0, 1.0, 0.02, 0.10, 0.0, 2.00);
	kalman_init(&kstatey, 1.0, 1.0, 0.02, 0.10, 0.0, 2.00);
	kalman_init(&kstatez, 1.0, 1.0, 0.02, 0.10, 0.0, 2.00);
	
	//counter for the 7 segment display
	int time_counter = 0;
	while (1){
		//Timer for the 7 segment display
		if (tim_flag3 == 1){
			tim_flag3 = 0;
			display_Number(number,time_counter,modeap);//display the number on the board
			time_counter++;
		}
		//timer for the kepad
		if (tim_flag2 == 1){
			tim_flag2 = 0;
			key_val = keypad_read();
			switch(key_val){	
				case 0:
					
					if(mode == INPUT){
						//Read 0
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
				//Read 1
				case 1:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					
					break;
				//Read 2
				case 2:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
				//Read 3
				case 3:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
				//Read 4
				case 4:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
				//Read 5
				case 5:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
				
					break;
				//Read 6
				case 6:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
				//Read 7
				case 7:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
				//Read 8
				case 8:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
				//Read 9
				case 9:
					if(mode == INPUT){
						
						if (number <= 0){
							number = key_val;
						}
						else if (number < 10 && number > 0){
							number = number*10 + key_val;						
						}
						else if (number < 100 && number > 10){
							number = number*10 + key_val;
						}
						
					}
					break;
					//select the mode: INPUT				
				case 10://Key A
					mode = INPUT;
					printf("Enter Numbers: \n");
					break;
				//Unused, for now
				case 11://Key B
					mode = INPUT;
					printf("B\n");
					break;
				//confirm the input
				case 12://Key C
					mode = INPUT;
					//printf("C\n");
					target = number;//set the target for the game
					printf("TARGET: %f\n",((float)target/10.0f)); 
					break;
				//reset the input
				case 13://Key D
					mode = INPUT;
					printf("Reset\n");
					number = 0;
					modeap = 0;
					break;
				//Select accelorometer
				case -1: 
					mode = ACCEL;
					modeap = 1;
					printf("ACCELEROMETER");
					break;
				//select piezo
				case -2:
					mode = PIEZO;
					modeap = 2;
					printf("PIEZO");
					break;
				
				default:
					break;
			}
		}
		//get the accel values
		if (acc_flag == 1 && mode == ACCEL){
			acc_flag = 0;
			if (time_count ++== 1){
				time_count =0;
				LIS3DSH_ReadACC(axes_vals);//rad value from sensor
				//cal kalman filter
				x = Kalmanfilter_C(axes_vals[0] - offsetx, &kstatex);
				y = Kalmanfilter_C(axes_vals[1] - offsety, &kstatey);
				z = Kalmanfilter_C(axes_vals[2] - offsetz, &kstatez);		
				pitch = pitch_calc(x,y,z);
				if (pitch > target){//for the game
					printf("Target reached. Pitch:  %f\n", pitch);
				}
				printf("Pitch: %f Roll: %f \n", pitch_calc(x,y,z) ,roll_calc(x,y,z));	
				printf("x: %f y: %f z: %f \n", x, y, z);	
			}
		}
		//timer for the piezo
		if (tim_flag4 == 1 && mode == PIEZO){
			tim_flag4 = 0;
			piezoValLast = piezoVal;
			piezoVal = adc_piezo();
			//Calibrating the piezo, to find the 0.18, which is the max level for noise
			//if(piezoVal > maxCal){
			//	maxCal = piezoVal;
			//}

				if (piezoVal > 0.18f){
					if (piezoVal > max){
						max = piezoVal;
					}else if(pulse_end){
						if(piezoValLast - piezoVal < -0.3f && max != 0.0f){
							pulse_end = 0;
							max = 0.0f;
							finalPiezo = 0.0f;
						}
					}
					if (max - piezoVal > 0.0f && pulse_end != 1){
						finalPiezo = max *32.0f;
						//printf("Force: %f \n", finalPiezo);//print the piezo force value
						if (finalPiezo > (float)target/10.0f){//for the game
							number = (finalPiezo)*10;
							printf("TARGET REACHED. Force: %f \n", finalPiezo);
						}
						pulse_end = 1;
					}
			}
		}
	}

}
//interrupt callbacks
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim -> Instance == TIM3){//inerrput for the 7 segment
		tim_flag3 = 1;
	}
	if(htim -> Instance == TIM2){//interrupt for the keypad
		tim_flag2 = 1;
	}
	if(htim -> Instance == TIM4){//interrupt for the piezo
		tim_flag4 = 1;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t Gpio_Pin){//interrupt for the accel
	if (Gpio_Pin == GPIO_PIN_0){
		acc_flag = 1;
	}
}

//calibrate the accel
void acc_calibrate(void){
	int sumx, sumy, sumz;
	int counter = 1;
	while(counter <= 60){
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
