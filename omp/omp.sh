#!/bin/bash
clear
gcc -fopenmp omp.c -o omp
gcc -fopenmp v.c -o v
./v 10
for i in 32 64
do
    $(echo export OMP_NUM_THREADS=$i)
    for j in 1000 5000
    do
       $(echo ./omp $j)
    done
done
