#include <stdio.h>
#include "arm_math.h"                   // ARM::CMSIS:DSP
#include "math.h"

#define S_DIM 2
#define M_DIM 2													//Measurement dimensions
#define LENGTH 10			

/*struct kstate{
	float f;
	float h;
	float q;
	float r;
	float k;
	float x;
	float p;
};

extern void kal(float *input, float *output, int length, struct kstate *ptr);

int main()
{
	float input[10] = {0.1, 2.2, -0.1, 3.5, 4.0, 4.1, 9.9, 0.0, 0.0, 2.3};
	float output[10];
	struct kstate kvals;
	kvals.f = 0.8;
	kvals.h = 1.2;
	kvals.q = 0.1;
	kvals.r = 0.1;
	kvals.k = 0.0;
	kvals.x = 0.0;
	kvals.p = 0.1;
	
	int i;
		
	kal(input, output, 10, &kvals);
	
	for (i=0; i<10; i++){
		printf("%f \n", output[i]);
	}; 
	return 0;
}*/	

float32_t input[M_DIM*LENGTH] =
{
	1.2, -0.9, 0.7, -0.9, 1.1, -0.8, 0.6, 0.8, -1.3, 0.0, 1.2, -0.9, 0.7, -0.9, 1.1, -0.8, 0.6, 0.8, -1.3, 0.0
};

float32_t output[S_DIM*LENGTH];

float32_t KF[S_DIM*S_DIM] = 
{
	0.8, 0.0, 0.0, 0.8
};
	
float32_t KH[M_DIM*S_DIM] = 
{
	1.2, 0.0, 0.0, 1.2
};

float32_t KQ[S_DIM*S_DIM] = 
{
	0.1, 0.0, 0.0, 0.1
};

float32_t KR[M_DIM*M_DIM] = 
{
	0.1, 0.0, 0.0, 0.1
};

float32_t KK[S_DIM*M_DIM] =
{
	0.0, 0.0, 0.0, 0.0
};

float32_t KX[S_DIM] =
{
	0.0, 0.0
};

float32_t KP[S_DIM*S_DIM] =
{
	0.1, 0.0, 0.0, 0.1
};

struct ks_vector{
	arm_matrix_instance_f32 f;
	arm_matrix_instance_f32 h;
	arm_matrix_instance_f32 q;
	arm_matrix_instance_f32 r;
	arm_matrix_instance_f32 k;
	arm_matrix_instance_f32 x;
	arm_matrix_instance_f32 p;
};

int kalmanfilter_c(float32_t* InArr, float32_t* OutArr, struct ks_vector* ks, int len, int measurement_dim, int state_dim);
int resCalc_CMSIS(float32_t* InArr, float32_t* OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks);
int resCalc(float32_t* InArr, float32_t* OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks);
int dispKstate(struct ks_vector* ks);
int dispMatrix(arm_matrix_instance_f32* matrix, char* name);
int dispOut(float* OutArr, int len, int state_dim);
void checkKS(struct ks_vector* ks);
void checkStatus(arm_status status);
void checkSing(arm_status status);
void checkMatchMult(arm_matrix_instance_f32* matrix1, arm_matrix_instance_f32* matrix2, char* mat1name, char* mat2name);
void checkMatchMultT(arm_matrix_instance_f32* matrix1, arm_matrix_instance_f32* matrix2, char* mat1name, char* mat2name);
void checkMatchAdd(arm_matrix_instance_f32* matrix1, arm_matrix_instance_f32* matrix2, char* mat1name, char* mat2name);
void checkInv(arm_matrix_instance_f32* matrix1);

int main(){
	//Initializing Kalman State vector.
	struct ks_vector kstate;
	arm_mat_init_f32(&kstate.f, S_DIM, S_DIM, KF);
	arm_mat_init_f32(&kstate.h, M_DIM, S_DIM, KH);
	arm_mat_init_f32(&kstate.q, S_DIM, S_DIM, KQ);
	arm_mat_init_f32(&kstate.r, M_DIM, M_DIM, KR);
	arm_mat_init_f32(&kstate.k, S_DIM, M_DIM, KK);
	arm_mat_init_f32(&kstate.x, S_DIM, 1, KX);
	arm_mat_init_f32(&kstate.p, S_DIM, S_DIM, KP);
	
	checkKS(&kstate);
	
	
	//dispKstate(&kstate);
	kalmanfilter_c(input, output, &kstate, LENGTH, M_DIM, S_DIM);
	dispOut(output, LENGTH, S_DIM);
	dispKstate(&kstate);
	//resCalc_CMSIS(input, output, LENGTH, M_DIM, S_DIM, &kstate);
	//resCalc(input, output, LENGTH, M_DIM, S_DIM,  &kstate);
	
	return 0;
}

int kalmanfilter_c(float32_t* InArr, float32_t* OutArr, struct ks_vector* ks, int len, int measurement_dim, int state_dim){
	int i,j;
	arm_status status;
	//Creating templates for calculations.
	float32_t tz[measurement_dim];
	arm_matrix_instance_f32 ztemp;
	arm_mat_init_f32(&ztemp, measurement_dim, 1, tz);
	
	float32_t tx1[state_dim];
	arm_matrix_instance_f32 xtemp1;
	arm_mat_init_f32(&xtemp1, state_dim, 1, tx1);
	
	float32_t tx2[state_dim];
	arm_matrix_instance_f32 xtemp2;
	arm_mat_init_f32(&xtemp2, measurement_dim, 1, tx2);
	
	float32_t tx3[state_dim];
	arm_matrix_instance_f32 xtemp3;
	arm_mat_init_f32(&xtemp3, measurement_dim, 1, tx3);
	
	float32_t tx4[state_dim];
	arm_matrix_instance_f32 xtemp4;
	arm_mat_init_f32(&xtemp4, state_dim, 1, tx4);
	
	float32_t tp1[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp1;
	arm_mat_init_f32(&ptemp1, state_dim, state_dim, tp1);
	
	float32_t tp2[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp2;
	arm_mat_init_f32(&ptemp2, state_dim, state_dim, tp2);
	
	float32_t tp3[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp3;
	arm_mat_init_f32(&ptemp3, state_dim, state_dim, tp3);
	
	float32_t tp4[state_dim*state_dim];
	arm_matrix_instance_f32 ptemp4;
	arm_mat_init_f32(&ptemp4, state_dim, state_dim, tp4);
	
	float32_t tk1[state_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp1;
	arm_mat_init_f32(&ktemp1, state_dim, measurement_dim, tk1);
	
	float32_t tk2[measurement_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp2;
	arm_mat_init_f32(&ktemp2, measurement_dim, measurement_dim, tk2);
	
	float32_t tk3[measurement_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp3;
	arm_mat_init_f32(&ktemp3, measurement_dim, measurement_dim, tk3);
	
	float32_t tk4[measurement_dim*measurement_dim];
	arm_matrix_instance_f32 ktemp4;
	arm_mat_init_f32(&ktemp4, measurement_dim, measurement_dim, tk4);
	
	float32_t Htran[measurement_dim*state_dim];
	arm_matrix_instance_f32 Ht;
	arm_mat_init_f32(&Ht, state_dim, measurement_dim, Htran);
	
	float32_t Ftran[state_dim*state_dim];
	arm_matrix_instance_f32 Ft;
	arm_mat_init_f32(&Ft, state_dim, state_dim, Ftran);
	
	float32_t Ident[state_dim*state_dim];
	arm_matrix_instance_f32 I;
	arm_mat_init_f32(&I, state_dim, state_dim, Ident);
	
	//Creating the identity matrix.
	for(i=0; i<state_dim; i++){
		I.pData[i*(state_dim + 1)] = 1;
	}
	
	//dispMatrix(&I, "I");
	
	//Creating transpose of F and Ht.
	arm_mat_trans_f32(&ks->f, &Ft);
	arm_mat_trans_f32(&ks->h, &Ht);
	
	//dispMatrix(&Ht, "Ht");
	//dispMatrix(&Ft, "Ft");
	
	for(i=0; i<len; i++){
		for(j=0; j<measurement_dim; j++){
			tz[j] = InArr[len*j+i];
		}
		
		//dispMatrix(&ztemp, "Z-Temp");
		//Predictions
		//xk|k-1
		status = arm_mat_mult_f32(&ks->f, &ks->x, &xtemp1);
		checkStatus(status);
		
		
		//pk|k-1
		status = arm_mat_mult_f32(&ks->f, &ks->p, &ptemp2);
		checkStatus(status);
		status = arm_mat_mult_f32(&ptemp2, &Ft, &ptemp1);
		checkStatus(status);
		status = arm_mat_add_f32(&ptemp1, &ks->q, &ptemp2);
		checkStatus(status);

		//Kalman Gain
		status = arm_mat_mult_f32(&ptemp2, &Ht, &ktemp1);
		checkStatus(status);
		status = arm_mat_mult_f32(&ks->h, &ktemp1, &ktemp2);
		checkStatus(status);
		status = arm_mat_add_f32(&ktemp2, &ks->r, &ktemp3);
		checkInv(&ktemp3);
		status = arm_mat_inverse_f32(&ktemp3, &ktemp4);
		checkSing(status);
		status = arm_mat_mult_f32(&ktemp1, &ktemp4, &ks->k);
		checkStatus(status);
		
		//pk|k
		status = arm_mat_mult_f32(&ks->k, &ks->h, &ptemp1);
		checkStatus(status);
		status = arm_mat_sub_f32(&I, &ptemp1, &ptemp3);
		checkStatus(status);
		status = arm_mat_mult_f32(&ptemp3, &ptemp2, &ks->p);
		checkStatus(status);
		
		//xk|k
		status = arm_mat_mult_f32(&ks->h, &xtemp1, &xtemp2);
		checkStatus(status);
		status = arm_mat_sub_f32(&ztemp, &xtemp2, &xtemp3);
		checkStatus(status);
		status = arm_mat_mult_f32(&ks->k, &xtemp3, &xtemp4);
		checkStatus(status);
		status = arm_mat_add_f32(&xtemp1, &xtemp4, &ks->x);
		checkStatus(status);
		
		//Housekeeping.
		for(j=0; j<state_dim; j++){
			OutArr[len*j+i] = ks->x.pData[j];
		}
		
		dispMatrix(&ks->x, "X");
	}
	
	return 0;
}

/*int resCalc_CMSIS(float32_t* InArr, float32_t* OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks){
	int i,j;
	float32_t res1[len*measurement_dim];
	arm_matrix_instance_f32 zr1;
	arm_mat_init_f32(&zr1, measurement_dim, len, res1);
	
	float32_t mean[len*measurement_dim];
	
	float32_t tz2[len*measurement_dim];
	arm_matrix_instance_f32 zk2;
	arm_mat_init_f32(&zk2, measurement_dim, len, tz2);
	
	arm_matrix_instance_f32 xk;
	arm_mat_init_f32(&xk, state_dim, len, output);
	
	float32_t tx2[len*measurement_dim];
	//tx2 = (float32_t*)malloc(len*measurement_dim);
	arm_matrix_instance_f32 xtemp;
	arm_mat_init_f32(&xtemp, state_dim, len, tx2);
	
	float32_t res2[len*measurement_dim];
	arm_matrix_instance_f32 zr2;
	arm_mat_init_f32(&zr2, measurement_dim, len, res2);
	
	float32_t std1[len*measurement_dim];
	arm_matrix_instance_f32 stdev1;
	arm_mat_init_f32(&stdev1, measurement_dim, 1, std1);
	
	float32_t std2[len*measurement_dim];
	arm_matrix_instance_f32 stdev2;
	arm_mat_init_f32(&stdev2, measurement_dim, 1, std2);
	
	float32_t* avg1;
	avg1 = (float32_t*)malloc(len*measurement_dim);
	arm_matrix_instance_f32 mean1;
	arm_mat_init_f32(&mean1, measurement_dim, 1, avg1);
	
	float32_t avg2[len*measurement_dim];
	arm_matrix_instance_f32 mean2;
	arm_mat_init_f32(&mean2, measurement_dim, 1, avg2);
	
	float32_t cor1[(2*len-1)*measurement_dim];
	arm_matrix_instance_f32 cor_coef1;
	arm_mat_init_f32(&cor_coef1, measurement_dim, 2*len-1, cor1);
	
	float32_t cor2[(2*len-1)*measurement_dim];
	arm_matrix_instance_f32 cor_coef2;
	arm_mat_init_f32(&cor_coef2, measurement_dim, 2*len-1, cor2);
	
	float32_t corf1[measurement_dim];
	arm_matrix_instance_f32 cor_cf1;
	arm_mat_init_f32(&cor_cf1, measurement_dim, 1, corf1);
	
	float32_t corf2[measurement_dim];
	arm_matrix_instance_f32 acor_cf2;
	arm_mat_init_f32(&acor_cf2, measurement_dim, 1, corf2);
	
	float32_t corf3[measurement_dim];
	arm_matrix_instance_f32 acor_cf3;
	arm_mat_init_f32(&acor_cf3, measurement_dim, 1, corf3);
	
	
	
	//Calculate residue.
	arm_mat_mult_f32(&ks->h, &xk, &xtemp);
	arm_mat_sub_f32(&zk1, &xtemp, &zr1);
	
	//Calculate resedue mean and standard deviation and mean of input data and standard deviation.
	for(i=0; i<measurement_dim; i++){
		arm_mean_f32(input, len, mean1.pData);
		dispMatrix(&zr1, "Residues");
		arm_std_f32((float32_t *)(&zr1.pData+i*len), len, (float32_t *)(&stdev1.pData+i));
		arm_mean_f32((float32_t *)(&zk1.pData+(i*len*sizeof(float))), len, (float32_t *)(&mean2.pData+i));
		arm_std_f32((float32_t *)(&zk1.pData+(i*len*sizeof(float))), len, (float32_t *)(&stdev2.pData+i));
	}
	
	for(i=0; i<len; i++){
		for(j=0; j<measurement_dim; j++){
			printf("I finished.");
			zr2.pData[len*j+i] = zr1.pData[len*j+i] - mean1.pData[j];
		}			
	}
	
	for(i=0; i<len; i++){
		for(j=0; j<measurement_dim; j++){
			zk2.pData[len*j+i] = zk1.pData[len*j+i] - mean2.pData[j];
		}			
	}
	
	for(i=0; i<measurement_dim; i++){
		arm_correlate_f32((float32_t *)(&zr2.pData+(i*len*sizeof(float))), len, (float32_t *)(&zk2.pData+(i*len*sizeof(float))), len, (float32_t *)(&cor_coef1.pData+(i*(2*len-1)*sizeof(float))));
		arm_correlate_f32((float32_t *)(&zr2.pData+(i*len*sizeof(float))), len, (float32_t *)(&zr2.pData+((i*len+1)*sizeof(float))), len, (float32_t *)(&cor_coef2.pData+(i*(2*len-2)*sizeof(float))));
	}
	
	for(j=0; j<measurement_dim; j++){
		cor_cf1.pData[j] = cor_coef1.pData[j*len+len-1]/(float32_t)sqrt(stdev1.pData[j]*stdev2.pData[j]);
	}
	
	for(j=0; j<measurement_dim; j++){
		acor_cf2.pData[j] = cor_coef2.pData[j*len+len-2]/(float32_t)sqrt(stdev1.pData[j]*stdev1.pData[j]);
	}		
	
	for(j=0; j<measurement_dim; j++){
		acor_cf3.pData[j] = cor_coef2.pData[j*len+len-3]/(float32_t)sqrt(stdev1.pData[j]*stdev1.pData[j]);
	}		
	
	dispMatrix(&mean1, "Mean of residuals");
	dispMatrix(&stdev1, "Standard deviation of residues");
	dispMatrix(&cor_cf1, "Correlation Of zk and residues");
	dispMatrix(&cor_cf1, "Autoorrelation Of residues lag 1");
	dispMatrix(&cor_cf1, "Autocorrelation Of residues lag 2");
	
	return 0;
}*/

/*int resCalc(float32_t* InArr, float32_t* OutArr, int len, int measurement_dim, int state_dim, struct ks_vector* ks){
	float32_t tz1[len*measurement_dim];
	arm_matrix_instance_f32 zk1;
	arm_mat_init_f32(&zk1, measurement_dim, len, tz1);
	
	float32_t tx1[len*state_dim];
	arm_matrix_instance_f32 xk;
	arm_mat_init_f32(&xk, state_dim, len, tx1);
	
	float32_t tx2[len*state_dim];
	arm_matrix_instance_f32 xtemp;
	arm_mat_init_f32(&xtemp, state_dim, len, tx2);
	
	float32_t res[len*measurement_dim];
	arm_matrix_instance_f32 zres;
	arm_mat_init_f32(&zres, measurement_dim, len, res);
	
	float32_t std[len*measurement_dim];
	arm_matrix_instance_f32 stdev;
	arm_mat_init_f32(&stdev, measurement_dim, 1, std);
	
	float32_t avg[len*measurement_dim];
	arm_matrix_instance_f32 mean;
	arm_mat_init_f32(&mean, measurement_dim, 1, avg);
	
	float32_t cor[len*measurement_dim];
	arm_matrix_instance_f32 cor_coef;
	arm_mat_init_f32(&cor_coef, measurement_dim, 1, cor);
	
	float32_t cor1[measurement_dim];
	arm_matrix_instance_f32 cor_coef1;
	arm_mat_init_f32(&cor_coef1, measurement_dim, 1, cor1);
	
	float32_t cor2[3*measurement_dim];
	arm_matrix_instance_f32 cor_coef2;
	arm_mat_init_f32(&cor_coef2, measurement_dim, 3, cor2);
	
	float32_t avg2[len*measurement_dim];
	arm_matrix_instance_f32 mean2;
	arm_mat_init_f32(&mean2, measurement_dim, 1, avg2);
	
	float32_t std2[len*measurement_dim];
	arm_matrix_instance_f32 stdev2;
	arm_mat_init_f32(&stdev2, measurement_dim, 1, std2);
	
	
	int i, j;
	for(i=0; i<measurement_dim; i++){
		arm_mean_f32((float32_t *)(&zk1.pData+(i*len*sizeof(float))), len, (float32_t *)(&mean2.pData+(i*sizeof(float))));
		arm_std_f32((float32_t *)(&zk1.pData+(i*len*sizeof(float))), len, (float32_t *)(&stdev2.pData+(i*sizeof(float))));
	}
	
	float32_t sum;
	for(i=0; i<len*measurement_dim; i++){
		zk1.pData[i] = OutArr[i];
		xk.pData[i] = InArr[i];
	}
	
	
	//Calculate residue.
	arm_mat_mult_f32(&ks->h, &xk, &xtemp);
	arm_mat_sub_f32(&zk1, &xtemp, &zres);
	dispMatrix(&zres, "Autocorrelation of residues w/ time lag 1 w/o cmsis");
	
	//Calculate mean.
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len; j++){
			sum += zres.pData[i*len+j];
		}
		mean.pData[i] = (float32_t)sum/len;
	}
	
	dispMatrix(&mean, "Mean of residues w/o cmsis");
	
	//Calculate standard deviation for residues.
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len; j++){
			sum += (float32_t)(zres.pData[i*len+j] - mean.pData[i])*(zres.pData[i*len+j] - mean.pData[i]);
		}
		arm_sqrt_f32(sum/len, &stdev.pData[i]);
	}
	
	dispMatrix(&stdev, "Standard deviation of residues w/o cmsis");
	
	//Calculate correlation.
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len; j++){
			sum += (float32_t)((zres.pData[i*len+j]-mean.pData[i])*(zk1.pData[i*len+j]- mean2.pData[i]));
		}
		cor_coef1.pData[i] = sum/(float32_t)sqrt(stdev.pData[j]*stdev2.pData[j]);
	}
	
	dispMatrix(&cor_coef1, "Correlation of residues w/o cmsis");
	
	//Autocorrelation with time lag 1
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len-1; j++){
			sum += (float32_t)(zres.pData[i*len+j]*zres.pData[i*len+j+1]);
		}
		cor_coef2.pData[i*len+1] = sum/(float32_t)sqrt(stdev.pData[j]*stdev.pData[j]);
	}
	
	dispMatrix(&cor_coef2, "Autocorrelation of residues w/ time lag 1 w/o cmsis");
	
	//Autocorrelation with time lag 2
	for(i=0; i<measurement_dim; i++){
		sum = 0;
		for(j=0; j<len-2; j++){
			sum += (float32_t)(zres.pData[i*len+j]*zres.pData[i*len+j+2]);
		}
		cor_coef2.pData[i*len+2] = sum/(float32_t)sqrt(stdev.pData[j]*stdev.pData[j]);
	}
	
	dispMatrix(&cor_coef2, "Autocorrelation of residues w/ time lag 2 w/o cmsis");
	return 0;
}*/

int dispKstate(struct ks_vector* ks){
	dispMatrix(&ks->f, "F");
	dispMatrix(&ks->h, "H");
	dispMatrix(&ks->q, "Q");
	dispMatrix(&ks->r, "R");
	dispMatrix(&ks->k, "K");
	dispMatrix(&ks->x, "X");
	dispMatrix(&ks->p, "P");
	return 0;	
}

int dispMatrix(arm_matrix_instance_f32* matrix, char* name){
	int col = matrix->numCols;
	int row = matrix->numRows;
	int i,j;
	printf("--------");
	printf("%s", name);
	printf("--------\n");
	
	for(i=0; i<row; i++){
		printf("|");
		for(j=0; j<col; j++){
			printf(" %f", matrix->pData[i*col+j]);
		}
		printf(" |\n");
	}
	printf(" \n");
	return 0;
}

int dispOut(float* OutArr, int len, int state_dim){
	int i,j;
	printf("--------Output--------\n");
	for(i=0; i<state_dim; i++){
		printf("|");
		for(j=0; j<len; j++){
			printf(" %f", OutArr[i*len +j]);
		}
		printf(" |\n");
	}
	
	return 0;
}

void checkStatus(arm_status status){
	if(status != ARM_MATH_SUCCESS){
		printf("There was a mathematical error....\n");
	}
	return;
}

void checkMatchMult(arm_matrix_instance_f32* matrix1, arm_matrix_instance_f32* matrix2, char* mat1name, char* mat2name){
	if(matrix1->numCols != matrix2->numRows){
		printf("There is a mismatch in size for multiplication with");
		printf(" %s ", mat1name);
		printf("and");
		printf(" %s.\n", mat2name);
		printf("\n");
	}
	return;
}

void checkMatchMultT(arm_matrix_instance_f32* matrix1, arm_matrix_instance_f32* matrix2, char* mat1name, char* mat2name){
	if(matrix1->numCols != matrix2->numCols){
		printf("There is a mismatch in size for multiplication tranpose with");
		printf(" %s ", mat1name);
		printf("and");
		printf(" %s.\n", mat2name);
		printf("\n");
	}
	return;
}

void checkMatchAdd(arm_matrix_instance_f32* matrix1, arm_matrix_instance_f32* matrix2, char* mat1name, char* mat2name){
	if((matrix1->numCols != matrix2->numCols)||(matrix1->numRows != matrix2->numRows)){
		printf("There is a mismatch in size for addition with");
		printf(" %s ", mat1name);
		printf("and");
		printf(" %s.\n", mat2name);
		printf("\n");
	}
	return;
}

void checkInv(arm_matrix_instance_f32* matrix1){
	if(matrix1->numCols != matrix1->numRows){
		printf("There is a mismatch in size for inversion."); 
	}
	return;
}

void checkSing(arm_status status){
	if(status == ARM_MATH_SINGULAR){
		printf("You have a singular matrix.");
	}
	return;
}

void checkKS(struct ks_vector* ks){
	checkMatchMult(&ks->f, &ks->x, "F", "X");
	checkMatchMult(&ks->f, &ks->p, "F", "P");
	checkMatchMult(&ks->p, &ks->f, "P", "Ft");
	checkMatchAdd(&ks->q, &ks->f, "Q", "F");
	checkMatchAdd(&ks->q, &ks->p, "Q", "P");
	checkMatchMultT(&ks->p, &ks->h, "P", "H");
	checkMatchMult(&ks->h, &ks->p, "H", "P");
	checkMatchMult(&ks->k, &ks->h, "K", "H");
	checkMatchMult(&ks->h, &ks->x, "H", "X");
	return;
}
