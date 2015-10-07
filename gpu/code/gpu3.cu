#include <stdio.h>
#include <cuda.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

__global__ void mul( float *Ad, float *Bd, float *Cd, int msize, int tile, int task);

int main( int argc, char **argv){

	// argv[0]: name, argv[1]: msize, argv[2]: tile_width/ per block, argv[3]: task per thread, argv[4]: isVerification
	clock_t start = clock();
	int i, j;
	int msize = atoi(argv[1]);
	int tile = atoi(argv[2]);
	int task = atoi(argv[3]);

	float *A, *B, *C;
	float *Ad, *Bd, *Cd;

	A = (float*)malloc(msize * msize * sizeof(float));
	B = (float*)malloc(msize * msize * sizeof(float));
	C = (float*)malloc(msize * msize * sizeof(float));

	// for verification
	if( argc == 5){
		for( i = 0; i < msize; i++){
			for( j = 0; j < msize; j++){
				A[i * msize + j] = (float)1;
				B[i * msize + j] = (float)1;
			}
		}
	}
	else{
		for( i = 0; i < msize; i++){
			for ( j = 0; j < msize; j++){
				srand(time(NULL));
				A[ i * msize + j] = (float)(rand()%2);
				srand(time(NULL));
				B[ i * msize + j] = (float)(rand()%2);
			}
		}
	}

	cudaMalloc((void**)&Ad, msize * msize * sizeof(float));
	cudaMalloc((void**)&Bd, msize * msize * sizeof(float));
	cudaMalloc((void**)&Cd, msize * msize * sizeof(float));
	
	cudaMemcpy(Ad, A, msize * msize * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(Bd, B, msize * msize * sizeof(float), cudaMemcpyHostToDevice);

	dim3 dimGrid((msize/tile), (msize/tile));
	dim3 dimBlock((tile/task), (tile/task));

	mul<<<dimGrid, dimBlock>>> (Ad, Bd, Cd, msize, tile, task);

	cudaMemcpy(C, Cd, msize * msize * sizeof(float), cudaMemcpyDeviceToHost);

	//print out the verification result
	int hoosh = 0;
	if( argc == 5){
		printf("\n=================== V =========================\n");
		for ( i = 0; i < msize; i++){
			printf("\n");
			for( j = 0; j < msize; j++){
				printf("%.2f ", C[i * msize + j]);
				if ( C[i * msize + j] != msize)
					hoosh++;
			}
		}

		printf("\n hoosh = %d \n", hoosh);
		if( hoosh != 0 )
			printf("\n YOU JUST DRIVING ME CRAZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZY\n");
		else
			printf("\n GOOD! ONLY ONE MORE TO GO!!! BIG BIG : ) \n");

	}

	free(A); free(B); free(C);
	cudaFree(Ad);cudaFree(Bd);cudaFree(Cd);

	printf( " \n msize: %d\t tilewidth: %d\t task: %d\t timeElapsed: %f\n", msize, tile, task, ((double)(clock()-start)/CLOCKS_PER_SEC));
	return 1;
}

__global__ void mul( float *Ad, float *Bd, float *Cd, int msize, int tile, int task){

	int tx, ty;
	int r, c;
	float Cv;
	int m;

	for ( tx = 0; tx < task; tx++){
		for ( ty = 0; ty < task; ty++){
			r = blockIdx.y * tile + threadIdx.y * task + ty;
			c = blockIdx.x * tile + threadIdx.x * task + tx;
			Cv = (float)0;
			for ( m = 0; m < msize; m++){
				Cv += Ad[ r * msize + m] * Bd[ m * msize + c];
			}
			Cd[ r * msize + c] = Cv;
		}
	}
}	
