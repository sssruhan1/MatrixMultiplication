
#include <mpi.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


#define TAGA 0
#define TAGB 1
#define TAGC 2


int main(int argc, char **argv)
{
    // paras for parallel computing
    int ierr,myid,numprocs;
    MPI_Status stat;
    MPI_Request request;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;
    
    
    clock_t start = clock();
    
    
    //paras
    int msize = atoi(argv[1]);// matrix size
    int chunk = msize/numprocs;//chunk number of rows to each node
    int n,i,j,k;
    int bias, mymin, mymax;
    bias = msize%numprocs; // the bias part of the matrix is computed by master processor
    
    
    
    int **abuffer;// stores chunk
    int **bbuffer;// stores matrix B
    int **cbuffer;
    
    
    //allocating buffer
    abuffer = (int**)calloc(chunk, sizeof(int*));// buffer to store chunk
    if(abuffer == NULL){
        printf("\n==========abuffer not allocated==============\n");
    }
    bbuffer = (int**)calloc(msize, sizeof(int*));// buffer to store Matrix B
    if(bbuffer == NULL){
        printf("\n==========bbuffer not allocated==============\n");
    }
    cbuffer = (int**)calloc(chunk, sizeof(int*));// buffer to store C
    if(cbuffer == NULL){
        printf("\n==========cbuffer not allocated==============\n");
    }
    
    if (myid == 0) { //master process
        
        // get the operand and allocating memory in master process.
        int **a,**b,**c; // matrix A, B and C
        a = (int**)calloc(msize,sizeof(int*));
        b = (int**)calloc(msize,sizeof(int*));
        c = (int**)calloc(msize,sizeof(int*));
        
        for (i = 0; i < msize; i++) {
            a[i] = (int*)calloc(msize,sizeof(int));
            b[i] = (int*)calloc(msize,sizeof(int));
            c[i] = (int*)calloc(msize,sizeof(int));
            for (j = 0; j < msize; j++) {
                srand(time(NULL));
                a[i][j] = rand()%100;
                srand(time(NULL));
                b[i][j] = rand()%100;
            }
        }
        
        // split the task into numprocs parts and Send out
        for (n = 1; n < numprocs; n++) {
            // determine mymin & mymax to all processors
            mymin = bias + chunk * n;// the first row of my chunk
            mymax = bias + chunk * (n+1); // the last row of my chunk
            
            // get my chunk into buffer
            for (i = 0; i< chunk; i++) {
                abuffer[i]=(int*)calloc(msize,sizeof(int));
                memcpy(abuffer[i], a[mymin+i], msize*sizeof(int));
                // send message
                MPI_Send(abuffer[i], msize, MPI_INT, n, TAGA, MPI_COMM_WORLD);            }
            // get matrix B into buffer~ Yeah, I know, it's lame :(
            for (i = 0; i< msize; i++) {
                bbuffer[i]=(int*)calloc(msize,sizeof(int));
                memcpy(bbuffer[i], b[i], msize*sizeof(int));
                // send message
                MPI_Send(bbuffer[i], msize, MPI_INT, n, TAGB, MPI_COMM_WORLD);
            }
        }
        
        //computing the first chunk of data( bias + 1 * chunk)
        for ( i = 0; i< (chunk+bias); i++){
            for ( j = 0; j< msize ; j++) {
                for ( k = 0; k < msize; k++) {
                    c[i][j] += a[i][k]*b[k][j];
                }
            }
        }
        
        
        
        // RECEIVE the rest of the product from all slave processors
        for (n = 1; n < numprocs; n++){
            mymin = bias + chunk * n;
            
            for (i = 0; i < chunk; i++) {
                cbuffer[i]= (int*)calloc(msize, sizeof(int));
                MPI_Recv(cbuffer[i], msize, MPI_INT, n, TAGC, MPI_COMM_WORLD, &stat);
                memcpy(c[mymin+i], cbuffer[i], msize*sizeof(int));
            }
        }
        
        free(a); free(b); free(c);
        printf("\n %d processors %d msize time elapsed: %f\n",numprocs, msize,((double)clock()- start)/ CLOCKS_PER_SEC);

    }
    //========================================================================================================
    //========================================================================================================
    //========================================================================================================
    //========================================================================================================
    // other processors get the data and multiply
    else{
        
        for (i = 0; i < chunk; i++) {
            abuffer[i]=(int*)calloc(msize,sizeof(int));
            MPI_Recv(abuffer[i], msize, MPI_INT, 0, TAGA, MPI_COMM_WORLD,&stat);
        }
        for (i = 0; i < msize; i++) {
            bbuffer[i]=(int*)calloc(msize,sizeof(int));
            MPI_Recv(bbuffer[i], msize, MPI_INT, 0, TAGB, MPI_COMM_WORLD,&stat);
        }
        
        // compute the corresponding part
        for ( i = 0; i< chunk; i++){
            cbuffer[i] = (int*)calloc(msize, sizeof(int));
            for (j = 0; j< msize ; j++) {
                for (k = 0; k < msize; k++) {
                    cbuffer[i][j] += abuffer[i][k]*bbuffer[k][j];
                }
            }
        }
        
        //Send the result
        for (i = 0; i < chunk ; i++) {
            MPI_Send(cbuffer[i], msize, MPI_INT, 0, TAGC, MPI_COMM_WORLD);
        }
        //MPI_Send(cbuffer,chunk*msize, MPI_INT, 0, TAGC, MPI_COMM_WORLD);
    }
    
    free(abuffer);
    free(bbuffer);
    free(cbuffer);

    MPI_FINALIZE();
    return 1;
}

