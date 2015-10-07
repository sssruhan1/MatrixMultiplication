
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
    int msize = atoi(argv[1]);
    
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
                a[i][j]=rand()%2;
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
    free(a);
    free(b);
    free(c);
    
    float oh;
    oh = (float)clock()/CLOCKS_PER_SEC;
    printf("\nTotal time taken is %f\n", oh);
    
    FILE *file1, *file2;
    char f1[30]="",f2[30] = "";
    sprintf(f1,"%d.txt",nthreads);
    sprintf(f2,"%d.txt",msize);
    file1 = fopen(f1,"a+");
    file2 = fopen(f2,"a+");
    
    fprintf(file1,"%d\t%f\n",msize,oh);
    fprintf(file2,"%d\t%f\n",nthreads,oh);
    
    
    
    fclose(file1);
    fclose(file2);
    
    return 1;
}

