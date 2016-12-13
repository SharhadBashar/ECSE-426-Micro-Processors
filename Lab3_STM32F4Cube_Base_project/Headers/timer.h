////////////////////////////////////////////////////////////////////////////////
//	File Name					: timer.h
//	Description				: header file for timer.c
//	Author						: Sharhad Bashar
//										: Anthony Ubah
//	Date							: Oct 17th, 2016
////////////////////////////////////////////////////////////////////////////////
#ifndef _TIMER_H

#define _TIMER_H
#include "stm32f4xx_hal.h"
void timer_init2(void); //for the keypad
void timer_init3(void); //for the display
void timer_init4(void); // for the piezo
#endif