
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
    int i,j,k;
    int **a,**b,**c;
    int msize;
    
    printf("\nSize: \n");
    scanf("%d",&msize);
    
    a = (int**)calloc(msize, sizeof(int*));
    b = (int**)calloc(msize, sizeof(int*));
    c = (int**)calloc(msize, sizeof(int*));
    
#pragma omp parallel private(j,th_id)
    {
        th_id = omp_get_thread_num();
        if (th_id == 0) {
            nthreads = omp_get_num_threads();
            chunksize = msize / nthreads + 1;
            printf("\n%d",omp_get_num_threads());
        }
        
        
        /*Starts the work sharing construct*/
#pragma omp for schedule(dynamic, chunksize)
        for(i = 0; i < msize; i++)
        {
            a[i] = (int*)calloc(msize,sizeof(int));
            for (j = 0; j < msize; j++)
                a[i][j]=rand()%2;
        }
    }
    
    
    
    omp_set_num_threads(nthreads);
#pragma omp parallel private(j,th_id)
    {
        th_id = omp_get_thread_num();
        if (th_id == 0) {
            nthreads = omp_get_num_threads();
            printf("\n%d",omp_get_num_threads());
        }
        
        
        /*Starts the work sharing construct*/
#pragma omp for schedule(dynamic, chunksize)
        for(i = 0; i < msize; i++)
        {
            b[i] = (int*)calloc(msize,sizeof(int));
            for (j = 0; j < msize; j++)
                b[i][j]=rand()%2;
        }
    }
    
#pragma omp barrier
    
    omp_set_num_threads(nthreads);
#pragma omp parallel private(j,k,th_id)
    {
        th_id = omp_get_thread_num();
        if (th_id == 0) {
            nthreads = omp_get_num_threads();
            printf("\n%d",omp_get_num_threads());
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
    
    
    printf("\nthreads # is %d\n", nthreads);
    printf("\nTotal time taken is %f\n", (float)clock()/CLOCKS_PER_SEC);
    free(a); free(b);free(c);
    return 1;
}

