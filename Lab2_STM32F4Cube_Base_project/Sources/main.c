////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <LED_Control.h>
#include <7_Segment_Display.h>
#include <temperature.h>
#include <kalman.h>
#include <piezo_Sensor.h>


//		Global variables		//
volatile int systick_flag; // flag to tell if a systick interrup has occured
float out;


int main(void){
	 
   // Initialize variables used in temperature conversion from base12 ADC to temperature in degrees Celsius
	float temp_Val;
	int i = 0;
	float ADC_val;
	adc_example_init();
  //		MCU Configuration		//
  //	Reset of all peripherals, Initializes the Flash interface and the Systick	//
  HAL_Init();
	
  //	Configure the system clock	//
  SystemClock_Config();

  //	GPIO Onboard LED, GPIOExx Pins and ADC Temperature initialization	//
  LED_Control_init();
	init_Display_Pins();
	adc_Temp_Init();


	
	//	Kalman State vector construction	//
	struct kalman_state kstate; 
	kstate.f = 1.00;
	kstate.q = 0.20;
	kstate.r = 0.10;
	kstate.h = 1.00;
	kstate.k = 0.00;
	kstate.p = 2.00;
	kstate.x = 30.0;
	
	//Counter initialization for polling ADC temperature sensor //
	int time_counter = 0;
	int time_counters = 0;

	//Output initialization and display number array initialization
	float out = 0.00;
	int numArray[3];
	while(1) { //run code forever 
			if(systick_flag == 1) { // Checking for system interrupton systick_flag
				systick_flag = 0;
				
				if(time_counter % 10 == 0) { //counter for polling sensor increments every 10 ms, used to count when 1 second has passed
					
					temp_Val = get_Data_Sensor();	//get the temperature value in celcius
					out = Kalmanfilter_C(temp_Val, &kstate); //get the filtered value
					//printf("%f Celcius\n", temp_Val); //print temp 
					if (out > 40.00){ //alarm
						display_Single_LED(i);
						i++;
					}	
					if (time_counter %10 == 0){ //prints temperature on 7 seg display
						time_counters = 0;
						while (time_counters < 1000){//counter for segment display	
							if(systick_flag == 1) { // checking for interrupt again (code didn't execute properly without this)
								systick_flag = 0;
								int number = out*10;
								//displaying all values on 4-digit 7-segment display
								numArray[0] = (int)(number/100); //breaks up the number in a array of size 3
								numArray[1] = (int)(number/10 - numArray[0]*10);
								numArray[2] = (int)(number - (numArray[0]*100 + numArray[1]*10));
								display_Degrees();
								if (time_counters % 4 == 0){ //displays the first number
									enable_Digit(1);
									write_Number(numArray[0]);
									undisplay_Decimal();
								}
								else if (time_counters % 4 == 1){ //display the second number
									enable_Digit(2);
									write_Number(numArray[1]);
									display_Decimal();				
								}
								else if (time_counters % 4 == 2){ //display the third number
									undisplay_Decimal();
									enable_Digit(3);
									write_Number(numArray[2]);
								}	
								else if (time_counters % 4 == 3){ //display the c
									undisplay_Decimal();
									enable_Digit(4);
									write_Number(10);
								}								
								time_counters++; //counter for segment display increment	
							}
						}
					}
														
				}
					time_counter++;	//counter for sensor polling increment
			}								
	}

}
	

