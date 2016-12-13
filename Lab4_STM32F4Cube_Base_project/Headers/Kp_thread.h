////////////////////////////////////////////////////////////////////////////////
//	File Name					: Kp_thread.h
//	Description				: Header file for key pad thread
//	Author						: Anthony Ubah, Sharhad Bashar
//	Date							: Oct 28, 2016
////////////////////////////////////////////////////////////////////////////////
#include "cmsis_os.h"

#ifndef _KP_THREAD
#define _KP_THREAD
//		Exported Functios		//
void start_Kp_thread(void *args);
extern osMutexId mode_mut_id, tilt_mut_id;
extern osThreadId Kp_thread_ID;

typedef enum {
	ACCEL, 
	TEMP
}mode_type;

typedef enum {
	PTH,
	ROL
}tilt_type;

extern mode_type mode; 
extern tilt_type tilt;
#endif

