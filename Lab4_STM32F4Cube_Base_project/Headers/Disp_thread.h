////////////////////////////////////////////////////////////////////////////////
//	File Name					: Disp_thread.h
//	Description				: Header file for display thread
//	Author						: Anthony Ubah, Sharhad Bashar
//	Date							: Oct 28, 2016
////////////////////////////////////////////////////////////////////////////////
#include "cmsis_os.h"

#ifndef _DISP_THREAD
#define _DISP_THREAD
//		Exported Functios		//
void start_Disp_thread(void *args);
extern osThreadId Disp_thread_ID;
#endif
