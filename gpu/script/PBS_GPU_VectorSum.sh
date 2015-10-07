#!/bin/bash
#PBS -q gpu
#PBS -l nodes=1:GPU:ppn=12
#PBS -l walltime=00:30:00
#PBS -M your-ubit-name@buffalo.edu
#PBS -m bea
#PBS -N Vector_Sum
#PBS -o Result_Vector_Sum.out
#PBS -j oe
#
#
cd $PBS_O_WORKDIR
echo "working directory = "$PBS_O_WORKDIR
ulimit -s unlimited
#
./Vector_Sum

#
echo "All Done!"
