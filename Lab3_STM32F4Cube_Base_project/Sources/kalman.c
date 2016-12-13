////////////////////////////////////////////////////////////////////////////////
//	File Name				: kalman.c
//	Description			: Provides definition & function to support the 7 Seg Display
//	Course					: ECSE 426
//	Authors					: Sharhad Bashar
//									: Anthony Ubah
//	Date						: Oct 24th, 2016
////////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "kalman.h"


float Kalmanfilter_C(float Input, struct kalman_state* kstate)
{
	/**
	Kalman Filter Function
	Filters measurements based on previous states.
	Takes in input measurement and Kalman State Vector.
	Outputs floating value.
	**/
	//printf("%.2f\t", Input);
	kstate->x = kstate->f*kstate->x;
	kstate->p = kstate->f*kstate->p*kstate->f + kstate->q; 
	kstate->k= kstate->p/(kstate->h*kstate->p*kstate->h+kstate->r);
	kstate->p= (1-kstate->k*kstate->h)*kstate->p;
	kstate->x= kstate->x + kstate->k *(Input - kstate->h*kstate->x);
	if(kstate->x!=kstate->x)
	{
			return -1.00;
	}
	//printf("Filtered value is = %.2f\n", kstate->x);
	
	return kstate->x;
}
//initialize the kalman struct
float kalman_init(struct kalman_state* kstate, float f, float h, float q, float r, float x, float p){
	kstate->x = x;
	kstate->p = p; 
	kstate->k = 0.0f;
	kstate->q = q;
	kstate->r = r;
	kstate->f = f;
	kstate->h = h;
}


