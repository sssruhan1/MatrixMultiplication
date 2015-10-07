#include <cuda.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

__global__ void mul(float* Ad, float* Bd, float* Cd, int msize);

int main(int argc, char **argv){

	clock_t start = clock();
	int msize;
	msize = atoi(argv[1]);
	int i, j;
	
	//input matrix	
	float *A,*B,*C; // matrix A, B and C
	A = (float*)malloc(msize*msize*sizeof(float));
	B = (float*)malloc(msize*msize*sizeof(float));
	C = (float*)malloc(msize*msize*sizeof(float));
        
	//for verification
        if (argc == 3){
		printf("\n ==============Verifying===================\n"); 
		for ( i = 0; i < msize; i++){
			for ( j = 0; j < msize; j++){
				A[i * msize + j ] = (float)1;
				B[i * msize + j ] = (float)1;
			}
		}
	}

	else{
		for (i = 0; i < msize; i++) {
			for (j = 0; j < msize; j++) {
				srand(time(NULL));
				A[i * msize + j] = (float)(rand()%2);
	    			srand(time(NULL));
	    			B[i * msize + j] = (float)(rand()%2);
			}
		}
	}

	float  *Ad;
	float  *Bd;
	float  *Cd;


	cudaMalloc((void**)&Ad, msize * msize * sizeof(float));
	cudaMalloc((void**)&Bd, msize * msize * sizeof(float));
	cudaMalloc((void**)&Cd, msize * msize * sizeof(float));

	cudaMemcpy( Ad, A, msize * msize * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy( Bd, B, msize * msize * sizeof(float), cudaMemcpyHostToDevice);
	
	dim3 dimGrid(1, 1);
	dim3 dimBlock(msize, msize);

	mul<<<dimGrid, dimBlock>>> ( Ad, Bd, Cd, msize);
				
	cudaMemcpy( C, Cd, msize * msize * sizeof(float), cudaMemcpyDeviceToHost);

	// print the verification result
	int Meow = 0;
	if ( argc == 3){
		for ( i = 0; i < msize; i++){
			printf("\n");
			for ( j = 0; j < msize; j++){
				printf("%d ", (int)C[i * msize + j]);
				if( C[ i * msize + j] != msize){
					Meow++;
				}
			}
		}
		if( Meow > 0 )
			printf("\n\n==========Ding, ding, ding! Please don't kill yourself! Calm down, buddy!==========\n");
		else
			printf("\n\n=========================Finally! You got it! BIG :-) =============================\n");
	}

	free(A);
	free(B);
	free(C);
	cudaFree(Ad);
	cudaFree(Bd);
	cudaFree(Cd);

	printf("\n msize: %d \n TIME ELAPSED : %f \n", msize, ((double)(clock()-start))/CLOCKS_PER_SEC);
	return 1;
}

__global__ void mul(float* Ad, float* Bd, float* Cd, int msize){

	float Avalue, Bvalue;
	float Cvalue = 0;
	
	int i;
	for ( i = 0; i < msize; i++){	
		Avalue = Ad[threadIdx.y * msize + i];
		Bvalue = Bd[i * msize + threadIdx.x];
		Cvalue += Avalue * Bvalue;
	}
	
	Cd[threadIdx.y * msize + threadIdx.x] = Cvalue;
	//Cd[threadIdx.y] = 1;
}
 
