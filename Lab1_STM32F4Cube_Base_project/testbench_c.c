#include <stdio.h>
#include "arm_math.h"                   // ARM::CMSIS:DSP

#define S_DIM 9
#define M_DIM 2													//Measurement dimensions
#define LENGTH 10				

/*const float input[M_DIM*LENGTH] =
{
	
};

static float output[S_DIM*LENGTH];*/

const float KF[S_DIM*S_DIM] = 
{
	61, 100, 121, 102, 167, 202, 123, 201, 243
};
	
/*const float KH[M_DIM*S_DIM] = 
{

};

const float KQ[S_DIM*S_DIM] = 
{

};

const float KR[M_DIM*M_DIM] = 
{

};

static float KK[S_DIM*M_DIM] =
{
	
};

static float KX[S_DIM] =
{
	
};

static float KP[S_DIM*S_DIM] =
{
	
};*/

struct ks_vector{
	arm_matrix_instance_f32 f;
	//arm_matrix_instance_f32 h;
	//arm_matrix_instance_f32 q;
	//arm_matrix_instance_f32 r;
	//arm_matrix_instance_f32 k;
	//arm_matrix_instance_f32 x;
	//arm_matrix_instance_f32 p;
};

int kalmanfilter_c(float* InArr, float* OutArr, struct ks_vector* ks, int len, int measurement_dim, int state_dim);
int resCalc_CSMSIS(float* InArr, float*OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks);
int resCalc(float* InArr, float*OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks);
int dispKstate(struct ks_vector* ks, int state_dim, int measurement_dim);
int printOut(float* OutArr);


int main(){
	struct ks_vector kstate;
	arm_mat_init_f32(&kstate.f, S_DIM, S_DIM, (float32_t *)KF);
	//arm_mat_init_f32(&kstate.h, M_DIM, S_DIM, (float32_t *)KH);
	//arm_mat_init_f32(&kstate.q, S_DIM, S_DIM, (float32_t *)KQ);
	//arm_mat_init_f32(&kstate.r, M_DIM, M_DIM, (float32_t *)KR);
	//arm_mat_init_f32(&kstate.k, S_DIM, M_DIM, (float32_t *)KK);
	//arm_mat_init_f32(&kstate.x, S_DIM, 1, (float32_t *)KX);
	//arm_mat_init_f32(&kstate.p, S_DIM, S_DIM, (float32_t *)KP);
	float f_inv[9];
	arm_matrix_instance_f32 fi;
	arm_mat_init_f32(&fi, S_DIM, S_DIM, (float32_t *)f_inv);
	
	arm_mat_inverse_f32(&kstate.f, &fi);
	
	//kalmanfilter_c((float *)input, (float *)output, &kstate, LENGTH, M_DIM, S_DIM);
	
	return 0;
}

/*int kalmanfilter_c(float* InArr, float* OutArr, struct ks_vector* ks, int len, int measurement_dim, int state_dim){
	
	float tz[measurement_dim];
	arm_matrix_instance_f32 ztemp;
	arm_mat_init_f32(&ztemp, measurement_dim, 1, (float32_t *)tz);
	
	float tx1[state_dim];
	arm_matrix_instance_f32 xtemp1;
	arm_mat_init_f32(&xtemp1, state_dim, 1, (float32_t *)tx1);
	
	float tx2[state_dim];
	arm_matrix_instance_f32 xtemp2;
	arm_mat_init_f32(&xtemp2, state_dim, 1, (float32_t *)tx2);
	
	float tx3[state_dim];
	arm_matrix_instance_f32 xtemp3;
	arm_mat_init_f32(&xtemp3, state_dim, 1, (float32_t *)tx3);
	
	float tp1[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp1;
	arm_mat_init_f32(&ptemp1, state_dim, state_dim, (float32_t *)tp1);
	
	float tp2[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp2;
	arm_mat_init_f32(&ptemp2, state_dim, state_dim, (float32_t *)tp2);
	
	float tp3[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp3;
	arm_mat_init_f32(&ptemp3, state_dim, state_dim, (float32_t *)tp3);
	
	float tp4[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp4;
	arm_mat_init_f32(&ptemp4, state_dim, state_dim, (float32_t *)tp4);
	
	float tk1[state_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp1;
	arm_mat_init_f32(&ktemp1, state_dim, measurement_dim, (float32_t *)tk1);
	
	float tk2[measurement_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp2;
	arm_mat_init_f32(&ktemp2, measurement_dim, measurement_dim, (float32_t *)tk2);
	
	float tk3[measurement_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp3;
	arm_mat_init_f32(&ktemp3, measurement_dim, measurement_dim, (float32_t *)tk3);
	
	float tk4[measurement_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp4;
	arm_mat_init_f32(&ktemp4, measurement_dim, measurement_dim, (float32_t *)tk4);
	
	float Htran[measurement_dim*state_dim];
	arm_matrix_instance_f32 Ht;
	arm_mat_init_f32(&Ht, state_dim, measurement_dim, (float32_t *)Htran);
	
	float Ftran[state_dim*state_dim];
	arm_matrix_instance_f32 Ft;
	arm_mat_init_f32(&Ft, state_dim, state_dim, (float32_t *)Ftran);
	
	float Ident[state_dim*state_dim];
	arm_matrix_instance_f32 I;
	arm_mat_init_f32(&I, state_dim, state_dim, (float32_t *)Ident);
	
	int i;
	for(i=0; i<state_dim; i++){
		Ident[i*(state_dim+1)] = 1;
	}
	
	arm_mat_trans_f32(&ks->f, &Ft);
	arm_mat_trans_f32(&ks->h, &Ht);
	
	for(i=0; i<len; i++){
		tz[0] = InArr[measurement_dim*i];
		tz[1] = InArr[measurement_dim*i + 1];
		
		//Predictions
		//xk|k-1
		arm_mat_mult_f32(&ks->f, &ks->x, &xtemp1);
		
		//pk|k-1
		arm_mat_mult_f32(&ks->f, &ks->p, &ptemp2);
		arm_mat_mult_f32(&ptemp2, &Ft, &ptemp1);
		arm_mat_mult_f32(&ptemp1, &ks->q, &ptemp2);
		arm_mat_add_f32(&ptemp2, &ks->q, &ptemp1);

		//Kalman Gain
		arm_mat_mult_f32(&ptemp1, &Ht, &ktemp1);
		arm_mat_mult_f32(&ks->h, &ktemp1, &ktemp2);
		arm_mat_add_f32(&ktemp2, &ks->r, &ktemp3);
		arm_mat_inverse_f32(&ktemp3, &ktemp4);
		arm_mat_mult_f32(&ktemp1, &ktemp4, &ks->k);
		
		//pk|k
		arm_mat_mult_f32(&ks->k, &ks->h, &ptemp2);
		arm_mat_sub_f32(&I, &ptemp2, &ptemp3);
		arm_mat_mult_f32(&ptemp3, &ptemp1, &ks->p);
		
		//xk|k
		arm_mat_mult_f32(&ks->h, &xtemp1, &xtemp2);
		arm_mat_sub_f32(&ztemp, &xtemp2, &xtemp3);
		arm_mat_mult_f32(&ks->k, &xtemp3, &xtemp2);
		arm_mat_add_f32(&xtemp1, &xtemp2, &ks->x);
		
		//Housekeeping.
		OutArr[state_dim*i] = ks->x.pData[0];
		OutArr[state_dim*i + 1] = ks->x.pData[1];
	}
	
	return 0;
}

int resCalc_CSMSIS(float* InArr, float*OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks){
	float tz[len*measurement_dim];
	arm_matrix_instance_f32 zk;
	arm_mat_init_f32(&zk, measurement_dim, len, (float32_t *)tz);
	
	float tx1[len*state_dim];
	arm_matrix_instance_f32 xk;
	arm_mat_init_f32(&xk, state_dim, len, (float32_t *)tx1);
	
	float tx2[len*state_dim];
	arm_matrix_instance_f32 xtemp;
	arm_mat_init_f32(&xtemp, state_dim, len, (float32_t *)tx2);
	
	float res[len*measurement_dim];
	arm_matrix_instance_f32 zres;
	arm_mat_init_f32(&zres, measurement_dim, len, (float32_t *)res);
	
	float std[len*measurement_dim];
	arm_matrix_instance_f32 stdev;
	arm_mat_init_f32(&stdev, measurement_dim, 1, (float32_t *)std);
	
	float avg[len*measurement_dim];
	arm_matrix_instance_f32 mean;
	arm_mat_init_f32(&mean, measurement_dim, 1, (float32_t *)avg);
	
	float cor1[(2*len-1)*measurement_dim];
	arm_matrix_instance_f32 cor_coef1;
	arm_mat_init_f32(&cor_coef1, measurement_dim, 2*len-1, (float32_t *)cor1);
	
	float cor2[(2*len-1)*measurement_dim];
	arm_matrix_instance_f32 cor_coef2;
	arm_mat_init_f32(&cor_coef2, measurement_dim, 2*len-1, (float32_t *)cor2);
	
	int i;
	for(i=0; i<len*measurement_dim; i++){
		zk.pData[i] = OutArr[i];
		xk.pData[i] = InArr[i];
	}
	
	//Calculate residue.
	arm_mat_mult_f32(&ks->h, &xk, &xtemp);
	arm_mat_sub_f32(&zk, &xtemp, &zres);
	
	for(i=0; i<measurement_dim; i++){
		arm_mean_f32((float *)&zres.pData+(i*len*sizeof(float)), len, (float *)&mean.pData+(i*sizeof(float)));
		arm_std_f32((float *)&zres.pData+(i*len*sizeof(float)), len, (float *)&stdev.pData+(i*sizeof(float)));
		arm_correlate_f32((float32_t *)&zk.pData+(i*len*sizeof(float)), len, (float32_t *)&zres.pData+(i*len*sizeof(float)), len, (float32_t *)&cor_coef1.pData+(i*sizeof(float)));
		arm_correlate_f32((float32_t *)&zres.pData+(i*len*sizeof(float)), len, (float32_t *)&zres.pData+(i*len*sizeof(float)), len, (float32_t *)&cor_coef2.pData+(i*sizeof(float)));
	}
	
	return 0;
}

int resCalc(float* InArr, float*OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks){
	float tz[len*measurement_dim];
	arm_matrix_instance_f32 zk;
	arm_mat_init_f32(&zk, measurement_dim, len, (float32_t *)tz);
	
	float tx1[len*state_dim];
	arm_matrix_instance_f32 xk;
	arm_mat_init_f32(&xk, state_dim, len, (float32_t *)tx1);
	
	float tx2[len*state_dim];
	arm_matrix_instance_f32 xtemp;
	arm_mat_init_f32(&xtemp, state_dim, len, (float32_t *)tx2);
	
	float res[len*measurement_dim];
	arm_matrix_instance_f32 zres;
	arm_mat_init_f32(&zres, measurement_dim, len, (float32_t *)res);
	
	float std[len*measurement_dim];
	arm_matrix_instance_f32 stdev;
	arm_mat_init_f32(&stdev, measurement_dim, 1, (float32_t *)std);
	
	float avg[len*measurement_dim];
	arm_matrix_instance_f32 mean;
	arm_mat_init_f32(&mean, measurement_dim, 1, (float32_t *)avg);
	
	float cor[len*measurement_dim];
	arm_matrix_instance_f32 cor_coef;
	arm_mat_init_f32(&cor_coef, measurement_dim, 1, (float32_t *)cor);
	
	float cor1[measurement_dim];
	arm_matrix_instance_f32 cor_coef1;
	arm_mat_init_f32(&cor_coef1, measurement_dim, 1, (float32_t *)cor1);
	
	float cor2[3*measurement_dim];
	arm_matrix_instance_f32 cor_coef2;
	arm_mat_init_f32(&cor_coef2, measurement_dim, 3, (float32_t *)cor2);
	
	int i, j;
	float sum;
	for(i=0; i<len*measurement_dim; i++){
		zk.pData[i] = OutArr[i];
		xk.pData[i] = InArr[i];
	}
	
	//Calculate residue.
	arm_mat_mult_f32(&ks->h, &xk, &xtemp);
	arm_mat_sub_f32(&zk, &xtemp, &zres);
	
	//Calculate mean.
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len; j++){
			sum += zres.pData[i*len+j];
		}
		mean.pData[i] = (float)sum/len;
	}
	
	//Calculate standard deviation.
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len; j++){
			sum += (float)(zres.pData[i*len+j] - mean.pData[i])*(zres.pData[i*len+j] - mean.pData[i]);
		}
		arm_sqrt_f32(sum/len, &stdev.pData[i]);
	}
	
	//Calculate correlation.
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len; j++){
			sum += (float)(zres.pData[i*len+j]*zk.pData[i*len+j]);
		}
		cor_coef1.pData[i] = sum;
	}
	
	//Autocorrelation with time lag 0
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len; j++){
			sum += (float)(zres.pData[i*len+j]*zres.pData[i*len+j]);
		}
		cor_coef2.pData[i*len] = sum;
	}
	
	//Autocorrelation with time lag 1
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len-1; j++){
			sum += (float)(zres.pData[i*len+j]*zres.pData[i*len+j+1]);
		}
		cor_coef2.pData[i*len+1] = sum;
	}
	
	//Autocorrelation with time lag 2
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len-2; j++){
			sum += (float)(zres.pData[i*len+j]*zres.pData[i*len+j+2]);
		}
		cor_coef2.pData[i*len+2] = sum;
	}
}

int dispKstate(struct ks_vector* ks, int state_dim, int measurement_dim){
	int i, j;
	//mxm
	printf("--------F--------\n");
	for(i=0; i<state_dim; i++){
		printf("|");
		for(j=0; j<state_dim; j++){
			printf(" %f", ks->f.pData[j]);
		}
		printf(" |\n");
	}
	printf("\n");
	
	printf("--------Q--------\n");
	for(i=0; i<state_dim; i++){
		printf("|");
		for(j=0; j<state_dim; j++){
			printf(" %f", ks->q.pData[j]);
		}
		printf(" |\n");
	}
	printf("\n");
	
	printf("--------P--------\n");
	for(i=0; i<state_dim; i++){
		printf("|");
		for(j=0; j<state_dim; j++){
			printf(" %f", ks->q.pData[j]);
		}
		printf(" |\n");
	}
	printf("\n");
	
	//mxn
	printf("--------K--------\n");
	for(i=0; i<state_dim; i++){
		printf("|");
		for(j=0; j<measurement_dim; j++){
			printf(" %f", ks->k.pData[j]);
		}
		printf(" |\n");
	}
	printf("\n");
	
	//nxm
	printf("--------H--------\n");
	for(i=0; i<measurement_dim; i++){
		printf("|");
		for(j=0; j<state_dim; j++){
			printf(" %f", ks->h.pData[j]);
		}
		printf(" |\n");
	}
	printf("\n");
	
	//nxn 
	printf("--------R--------\n");
	for(i=0; i<measurement_dim; i++){
		printf("|");
		for(j=0; j<state_dim; j++){
			printf(" %f", ks->r.pData[j]);
		}
		printf(" |\n");
	}
	printf("\n");
	
	//mx1
	printf("--------X--------\n");
	for(i=0; i<state_dim; i++){
		printf("|");
		printf(" %f", ks->x.pData[j]);
		printf(" |\n");
	}
	printf("\n");
	return 0;	
}

int dispOut(float* OutArr, int len, int state_dim){
	int i,j;
	printf("--------Output--------\n");
	for(i=0; i<state_dim; i++){
		printf("|");
		for(j=0; j<len; j++){
			printf(" %f", OutArr[j]);
		}
		printf(" |\n");
	}
	
	return 0;
}*/
