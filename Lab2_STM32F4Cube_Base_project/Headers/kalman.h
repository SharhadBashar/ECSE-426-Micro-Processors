////////////////////////////////////////////////////////////////////////////////
//	File Name				: kalman.h
//	Description			: Provides definition & function to support the 7 Seg Display
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Oct 9th, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _KALMAN
#define _KALMAN
typedef struct kalman_state{
		float q;
		float r;
		float x;
		float p;
		float k;
		float f;
		float h;
	};

float Kalmanfilter_C(float Input, struct kalman_state* kstate);
#endif