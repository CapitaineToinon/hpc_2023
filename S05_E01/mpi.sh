#!/bin/bash
#SBATCH --job-name=S05E01
#SBATCH --partition=debug-cpu
#SBATCH --time=00:00:15
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1

module purge
module load GCC/12.2.0 OpenMPI/4.1.4

mpicc mpi.c
srun ./a.out
rm ./a.out
