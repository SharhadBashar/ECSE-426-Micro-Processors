/**
  ******************************************************************************
  * File Name					: kalman_filter.c
	*	Description				: multidensional kalman filter
	*	Author						: Stephan Greto-McGrath
	*	Date							: Oct 4th, 2016
  ******************************************************************************
  */
#include <stdio.h>
#include "arm_math.h"
#include <kalman_filter.h>

/** 
	* @brief applies a kalaman filter to given data
	*	@param InputArray float* input data/signal
	*	@param OutputArray float* output data/signal
	*	@param kalman_state* kstate	struct to store the k-states the filter uses
	*	@param Length int length of data measurement / sample points
	*	@param State_Dimension int self-explanatory
	*	@param Measurement_Dimension int self-explanatory
	* @return int not really used... might as well be void
	*/
int kalman_filter(float* InputArray, float* OutputArray, struct kalman_state* kstate, int Length, int State_dimension, int Measurement_dimension){

		//=====================
		// setup
		//=====================
		
		// create index variables
		int i,j;
	
		// create matrices to store temporary data used in calculations
		
		// z
		float32_t store_z[Measurement_dimension];
		arm_matrix_instance_f32 temp_z;
		arm_mat_init_f32(&temp_z, Measurement_dimension, 1, store_z);
		
		// x
		float32_t store_x1[State_dimension];
		arm_matrix_instance_f32 temp_x1;
		arm_mat_init_f32(&temp_x1, State_dimension, 1, store_x1);
		
		float32_t store_x2[Measurement_dimension];
		arm_matrix_instance_f32 temp_x2;
		arm_mat_init_f32(&temp_x2, Measurement_dimension, 1, store_x2);
		
		float32_t store_x3[State_dimension];
		arm_matrix_instance_f32 temp_x3;
		arm_mat_init_f32(&temp_x3, State_dimension, 1, store_x3);
		
		float32_t store_x4[Measurement_dimension];
		arm_matrix_instance_f32 temp_x4;
		arm_mat_init_f32(&temp_x4, Measurement_dimension, 1, store_x4);
		
		float32_t store_xout[State_dimension];
		arm_matrix_instance_f32 temp_xout;
		arm_mat_init_f32(&temp_xout, State_dimension, 1, store_xout);
		
		// p
		float32_t store_p1[State_dimension*State_dimension];
		arm_matrix_instance_f32 temp_p1;
		arm_mat_init_f32(&temp_p1, State_dimension, State_dimension, store_p1);
		
		float32_t store_p2[State_dimension*State_dimension];
		arm_matrix_instance_f32 temp_p2;
		arm_mat_init_f32(&temp_p2, State_dimension, State_dimension, store_p2);
		
		float32_t store_p3[State_dimension*State_dimension];
		arm_matrix_instance_f32 temp_p3;
		arm_mat_init_f32(&temp_p3, State_dimension, State_dimension, store_p3);
		
		float32_t store_p4[State_dimension*State_dimension];
		arm_matrix_instance_f32 temp_p4;
		arm_mat_init_f32(&temp_p4, State_dimension, State_dimension, store_p4);
		
		// k
		float32_t store_k1[State_dimension*Measurement_dimension];
		arm_matrix_instance_f32 temp_k1;
		arm_mat_init_f32(&temp_k1, State_dimension, Measurement_dimension, store_k1);
		
		float32_t store_k2[Measurement_dimension*Measurement_dimension];
		arm_matrix_instance_f32 temp_k2;
		arm_mat_init_f32(&temp_k2, Measurement_dimension, Measurement_dimension, store_k2);
		
		float32_t store_k3[Measurement_dimension*Measurement_dimension];
		arm_matrix_instance_f32 temp_k3;
		arm_mat_init_f32(&temp_k3, Measurement_dimension, Measurement_dimension, store_k3);
		
		float32_t store_k4[Measurement_dimension*Measurement_dimension];
		arm_matrix_instance_f32 temp_k4;
		arm_mat_init_f32(&temp_k4, Measurement_dimension, Measurement_dimension, store_k4);
		
		// create space for transpose matrices
		float32_t store_ht[State_dimension*Measurement_dimension];
		arm_matrix_instance_f32 h_trans;
		arm_mat_init_f32(&h_trans, State_dimension, Measurement_dimension, store_ht);

		float32_t store_ft[State_dimension*State_dimension];
		arm_matrix_instance_f32 f_trans;
		arm_mat_init_f32(&f_trans, State_dimension, State_dimension, store_ft);
		
		// transpose the matrices
		arm_mat_trans_f32(&kstate->f, &f_trans);
		arm_mat_trans_f32(&kstate->h, &h_trans);
		
		// create space for identity matrix	
		float32_t store_id[State_dimension*State_dimension];
		arm_matrix_instance_f32 id;
		arm_mat_init_f32(&id, State_dimension, State_dimension, store_id);
		
		// fill identity matrix 1s
		for(i=0; i<State_dimension; i++){
			id.pData[i*(State_dimension + 1)] = 1;
		}
		
		
		//=====================
		// calculations
		//=====================
		
		for(i=0; i<Length; i++){
			for(j=0; j<Measurement_dimension; j++){
				store_z[j] = InputArray[Length*j+i];
			}
			arm_status status;
			//---------------------
			// prediction calc
			//---------------------
			
			//x'_k|k-1 = F*x'_k-1|k-1
			status = arm_mat_mult_f32(&kstate->f, &kstate->x, &temp_x1); // F*x'_k-1|k-1
			
			
			//P'_k|k-1 = F*P'_k-1|k-1*F_T+Q
			status = arm_mat_mult_f32(&kstate->f, &kstate->p, &temp_p1); // F*P'_k-1|k-1
			status = arm_mat_mult_f32(&temp_p1, &f_trans, &temp_p2); 		// F*P'_k-1|k-1*F_T
			status = arm_mat_add_f32(&temp_p2, &kstate->q, &temp_p1);		// F*P'_k-1|k-1*F_T+Q
	
			
			//---------------------
			// update calc
			//---------------------
			
			//K = P'_k|k-1*H_T*(H*P'_k|k-1*H_T+R)^-1
			status = arm_mat_mult_f32(&temp_p1, &h_trans, &temp_k1);			// P'_k|k-1*H_T
			status = arm_mat_mult_f32(&kstate->h, &temp_k1, &temp_k2);		// H*P'_k|k-1*H_T
			status = arm_mat_add_f32(&temp_k2, &kstate->r, &temp_k3);		// H*P'_k|k-1*H_T+R       
			status = arm_mat_inverse_f32(&temp_k3, &temp_k4);						// (H*P'_k|k-1*H_T+R)^-1;
			// TODO
			// need to check for singularity
			status = arm_mat_mult_f32(&temp_k1, &temp_k4, &kstate->k);		// P'_k|k-1*H_T*(H*P'_k|k-1*H_T+R)^-1
		
			
			//P'_k|k = (I-K*H)P'_k|k-1
			status = arm_mat_mult_f32(&kstate->k, &kstate->h, &temp_p1);	// K*H
			status = arm_mat_sub_f32(&id, &temp_p2, &temp_p3);						// (I-K*H)
			status = arm_mat_mult_f32(&temp_p3, &temp_p1, &kstate->p);		// (I-K*H)P'_k|k-1

			
			//x'_k|k = x_k|k-1+K*(z_k-H*x'_k|k-1) 
			status = arm_mat_mult_f32(&kstate->h, &temp_x1, &temp_x2);		// H*x'_k|k-1								
			status = arm_mat_sub_f32(&temp_z, &temp_x2, &temp_x4);				// (z_k-H*x'_k|k-1) 			
			status = arm_mat_mult_f32(&kstate->k, &temp_x4, &temp_x3);		// K*(z_k-H*x'_k|k-1) 				////
			status = arm_mat_add_f32(&temp_x1, &temp_x3, &kstate->x);		// x_k|k-1+K*(z_k-H*x'_k|k-1) 
			
			// write to output
			for(j=0; j<State_dimension; j++){
				OutputArray[Length*j+i] = kstate->x.pData[j];
			}
		}
		return 0;
}

// helper function to print matrix
int print_mat(arm_matrix_instance_f32* mat, char* mat_name){
	int i,j;
	printf("%s:\n", mat_name);
	for(i=0; i<mat->numRows; i++){
		printf("|");
		for(j=0; j<mat->numCols; j++){
			printf(" %f", mat->pData[i*mat->numCols+j]);
		}
		printf(" |\n");
	}
	printf(" \n");
	return 0;
}

// helper function to print output array
int print_output(float* OutputArray, int Length, int State_dimension){
	int i,j;
	printf("output:\n");
	for(i=0; i<State_dimension; i++){
		printf("|");
		for(j=0; j<Length; j++){
			printf(" %f", OutputArray[i*Length +j]);
		}
		printf(" |\n");
	}
	
	return 0;
}
