#!/bin/bash
#PBS -q gpu
#PBS -l nodes=1:GPU:ppn=12
#PBS -l walltime=10:00:00
#PBS -M ruhansa@buffalo.edu
#PBS -m bea
#PBS -N gpu3MulTile
#PBS -o gpu3HMulTile.txt
#PBS -j oe
#
#
cd $PBS_O_WORKDIR
echo "working directory = "$PBS_O_WORKDIR
ulimit -s unlimited
#
./gpu3 20 4 2 v
for m in 30000 40000
do
for t in 20 30 50 60 100
do
for i in 2 5 10
do
./gpu3 $m $t $i
done
done
done

#
echo "All Done!"
