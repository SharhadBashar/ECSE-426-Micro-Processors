////////////////////////////////////////////////////////////////////////////////
//	File Name					: accelerometer.h
//	Description				: header file for accelerometer.c
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 17th, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _ACCELEROMETER
#define _ACCELEROMETER
#define ARM_MATH_C4


#include <LIS3DSH.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include <arm_math.h>

//iniialize the functions
void acc_init(void);	
void acc_calibrate(void);
float pitch_calc(float x, float y, float z);
float roll_calc(float x, float y, float z);
#endif
