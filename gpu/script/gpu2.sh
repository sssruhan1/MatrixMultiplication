#!/bin/bash
#PBS -q gpu
#PBS -l nodes=1:GPU:ppn=12
#PBS -l walltime=10:00:00
#PBS -M ruhansa@buffalo.edu
#PBS -m bea
#PBS -N gpu2Huge
#PBS -o gpu2Huge.txt
#PBS -j oe
#
#
cd $PBS_O_WORKDIR
echo "working directory = "$PBS_O_WORKDIR
ulimit -s unlimited
#

./gpu2 10 2 v
for m in 30000
do
for t in 5 10 15 20 30 50
do
./gpu2 $m $t
done
done

#
echo "All Done!"
