#!/bin/bash
#PBS -q gpu
#PBS -l nodes=1:GPU:ppn=12
#PBS -l walltime=10:00:00
#PBS -M ruhansa@buffalo.edu
#PBS -m bea
#PBS -N gpu4sremain
#PBS -o gpu4sreamin.txt
#PBS -j oe
#
#
cd $PBS_O_WORKDIR
echo "working directory = "$PBS_O_WORKDIR
ulimit -s unlimited
#

for m in 3000 7000 9000 10000 15000
do
for t in 30 50 100
do
for i in 2 10
do
./gpu4 $m $t $i
done
done
done

echo "All Done!"
