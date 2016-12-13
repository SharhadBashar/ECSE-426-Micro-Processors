////////////////////////////////////////////////////////////////////////////////
//	File Name					: Temp_thread.h
//	Description				: Header file for Temperature thread
//	Author						: Anthony Ubah, Sharhad Bashar
//	Date							: Oct 28, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _TEMP_THREAD
#define _TEMP_THREAD

#include "cmsis_os.h"
//		Exported Functios		//
void start_Temp_thread(void *args);
extern osThreadId Temp_thread_ID;
extern float temp;
extern osMutexId temp_mut_id;
	
#endif
