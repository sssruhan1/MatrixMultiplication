#!/bin/bash
#PBS -q gpu
#PBS -l nodes=1:GPU:ppn=12
#PBS -l walltime=10:00:00
#PBS -M ruhansa@buffalo.edu
#PBS -m bea
#PBS -N gpu1
#PBS -o gpu1Huge.txt
#PBS -j oe
#
#
cd $PBS_O_WORKDIR
echo "working directory = "$PBS_O_WORKDIR
ulimit -s unlimited
#
./gpu1 10 v
for m in 30000 50000 100000
do
./gpu1 $m
done
#
echo "All Done!"
