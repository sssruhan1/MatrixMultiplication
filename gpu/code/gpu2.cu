#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

__global__  void mul(float *Ad, float *Bd, float *Cd, int msize, int tilewidth);

int main( int argc, char **argv){
	
	clock_t start = clock();
	int i, j;
	int tile;
	int msize;
	
	msize = atoi(argv[1]); //matrix size 
	tile = atoi(argv[2]); //tile width

	float *A, *B, *C;

	A = (float*)malloc(msize * msize * sizeof(float));
	B = (float*)malloc(msize * msize * sizeof(float));
	C = (float*)malloc(msize * msize * sizeof(float));

		
	if (argc == 4) { // for verification
		for( i = 0; i < msize; i++){
			for ( j = 0; j < msize; j++){
				A[ i * msize + j ] = (float)1;
				B[ i * msize + j ] = (float)1;
			}
		}
	}
	
	else{
		for( i = 0; i < msize; i++){
			for( j = 0; j < msize; j++){
				srand( time(NULL));
				A[ i * msize + j] = (float)(rand()%2);
				srand( time(NULL));
				B[ i * msize + j] = (float)(rand()%2);
			}
		}
	}

	float *Ad, *Bd, *Cd;
	
	cudaMalloc( (void**)&Ad, msize * msize * sizeof(float));
	cudaMalloc( (void**)&Bd, msize * msize * sizeof(float));
	cudaMalloc( (void**)&Cd, msize * msize * sizeof(float));

	cudaMemcpy( Ad, A, msize * msize * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy( Bd, B, msize * msize * sizeof(float), cudaMemcpyHostToDevice);

	dim3 dimGrid( (msize/tile), (msize/tile));
	dim3 dimBlock( tile, tile);

	mul<<<dimGrid, dimBlock>>>( Ad, Bd, Cd, msize, tile);

	cudaMemcpy( C, Cd, msize * msize * sizeof(float), cudaMemcpyDeviceToHost);
	
	// verify the result
	int knife = 0;
	if ( argc == 4){
		printf("\n ================== V ======================\n");
		for ( i = 0; i < msize; i++){
			printf("\n");
			for ( j = 0; j < msize; j++){
				printf("%f ", C[ i * msize + j]);
				if ( C[ i * msize + j] != msize)
					knife ++;
			}
		}
		if ( knife != 0) 
			printf("\n ================= OH CRAP! ====================\n");
		else
			printf("\n ================= MO HA HA : ) ====================\n");

	}
	free(A); free(B); free(C);
	cudaFree(Ad); cudaFree(Bd); cudaFree(Cd);

	printf("\n msize: %d\t tilewidth: %d\t time elapsed: %f\n", msize, tile, ((double)(clock() - start))/CLOCKS_PER_SEC); 
	return 1;
}

__global__ void mul( float *Ad, float *Bd, float *Cd, int msize, int tilewidth){

	int r = blockIdx.y * tilewidth + threadIdx.y;
	int c = blockIdx.x * tilewidth + threadIdx.x;

	float Cv = 0;
	int i;
	for( i = 0; i < msize; i++)
		Cv += Ad[ r * msize + i] * Bd[ c + i * msize];

	Cd[ r * msize + c] = Cv;
}


