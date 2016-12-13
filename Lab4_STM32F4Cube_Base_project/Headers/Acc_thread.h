////////////////////////////////////////////////////////////////////////////////
//	File Name					: Acc_thread.h
//	Description				: Header file for accelerometer thread
//	Author						: Anthony Ubah, Sharhad Bashar
//	Date							: Oct 28, 2016
////////////////////////////////////////////////////////////////////////////////
#include "cmsis_os.h"

#ifndef _ACC_THREAD
#define _ACC_THREAD

//		Exported Functios		//
void start_Acc_thread(void *args);
extern osMutexId acc_mut_id;
extern float pitch, roll;
	
#endif