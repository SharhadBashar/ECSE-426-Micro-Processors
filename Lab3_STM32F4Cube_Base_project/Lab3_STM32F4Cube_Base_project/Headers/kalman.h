	////////////////////////////////////////////////////////////////////////////////
//	File Name				: kalman.h
//	Description			: Provides definition & function to support the kalman
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Oct 24th, 2016
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
float kalman_init(struct kalman_state* kstate, float f, float h, float q, float r, float x, float p);
#endif
	