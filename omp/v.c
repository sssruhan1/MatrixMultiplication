
#include <omp.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, const char * argv[])
{
    // paras for parallel computing
    int chunksize;
    int th_id,nthreads;
    int i,j,k,horror;
    int **a,**b,**c;
    int msize = atoi(argv[1]);
    horror = 0;
    
    a = (int**)calloc(msize, sizeof(int*));
    b = (int**)calloc(msize, sizeof(int*));
    c = (int**)calloc(msize, sizeof(int*));
    
#pragma omp parallel private(j,th_id)
    {
        th_id = omp_get_thread_num();
        if (th_id == 0) {
            nthreads = omp_get_num_threads();
            chunksize = msize / nthreads + 1;
            printf("Threads # when input a is %d\n",omp_get_num_threads());
        }
        
        
        /*Starts the work sharing construct*/
#pragma omp for schedule(dynamic, chunksize)
        for(i = 0; i < msize; i++)
        {
            a[i] = (int*)calloc(msize,sizeof(int));
            for (j = 0; j < msize; j++)
                a[i][j]=1;
        }
    }

    
    
    omp_set_num_threads(nthreads);
#pragma omp parallel private(j,th_id)
    {
        th_id = omp_get_thread_num();
        if (th_id == 0) {
            nthreads = omp_get_num_threads();
            printf("Threads # when input b is %d\n",omp_get_num_threads());
        }
        
        
        /*Starts the work sharing construct*/
#pragma omp for schedule(dynamic, chunksize)
        for(i = 0; i < msize; i++)
        {
            b[i] = (int*)calloc(msize,sizeof(int));
            for (j = 0; j < msize; j++)
                b[i][j]=1;
        }
    }
    
#pragma omp barrier
    
    omp_set_num_threads(nthreads);
#pragma omp parallel private(j,k,th_id)
    {
        th_id = omp_get_thread_num();
        if (th_id == 0) {
            nthreads = omp_get_num_threads();
            printf("Threads # when processing a*b is %d\n",omp_get_num_threads());
        }
        
        
        /*Starts the work sharing construct*/
#pragma omp for schedule(dynamic, chunksize)
        for(i = 0; i < msize; i++)
        {
            c[i] = (int*)calloc(msize,sizeof(int));
            for (j = 0; j < msize; j++)
            {
                c[i][j]= 0;
                for (k = 0; k < msize; k++)
                    c[i][j] = c[i][j]+ a[i][k]*b[k][j];
            }
        }
    }
    
    
#pragma omp barrier
    

    omp_set_num_threads(nthreads);
#pragma omp parallel private(j,th_id)
    {
        th_id = omp_get_thread_num();
        if (th_id == 0) {
            nthreads = omp_get_num_threads();
            printf("Threads # when verifying is %d\n",omp_get_num_threads());
        }
        
        
        /*Starts the work sharing construct*/
#pragma omp for schedule(dynamic, chunksize)
        for(i = 0; i < msize; i++)
        {
            for (j = 0; j < msize; j++)
            {
                if (c[i][j] != msize)
                    horror++;
            }
        }
    }
    
    if(horror == 0)
        printf("\n Congrats! The result is correct ^_^!\n");
    else
    {
        printf("\n -----------------!WATCH OUT!------------------\nThe result is WRONG - - !\n");
    }
    
    
    
    
    
    printf("\nTotal time taken is %f\n", (float)clock()/CLOCKS_PER_SEC);
    
    free(a);
    free(b);
    free(c);
    return 1;
}

