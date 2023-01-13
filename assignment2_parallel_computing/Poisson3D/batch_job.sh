#!/bin/sh

# experiments name
export EXPNAME=test_$(date +%Y%m%d_%H%M%S)
export RESULT_FOLDER="../results/$EXPNAME"
mkdir -p $RESULT_FOLDER
mkdir -p ../hpc_logs
# mkdir -p ../results/$EXPNAME/compile_logs

#!/bin/bash
# 02614 - High-Performance Computing, January 2022
# 
# batch script to run matmult on a decidated server in the hpcintro
# queue
#
# Author: Bernd Dammann <bd@cc.dtu.dk>

### General options
### -- set the job Name --
#BSUB -J OpenMPjob
### -- set the job queue --
#BSUB -q hpcintro
### -- ask for number of cores (default: 1) --
#BSUB -n 20
### -- specify that the cores MUST BE on a single host! It's a SMP job! --
#BSUB -R "span[hosts=1]"
### -- set walltime limit: hh:mm --
#BSUB -W 00:15 
### -- specify that we need 4GB of memory per core/slot -- 
#BSUB -R "rusage[mem=10GB]"
### -- Specify the output and error file. %J is the job-id -- 
### -- -o and -e mean append, -oo and -eo mean overwrite -- 
#BSUB -o ../hpc_logs/%J.out
#BSUB -e ../hpc_logs/%J.err

# set OMP_NUM_THREADS _and_ export! 
# OMP_NUM_THREADS=$LSB_DJOB_NUMPROC 
# export OMP_NUM_THREADS 

# set compiler flags
# export OPT_FLAGS="-g -Ofast -funroll-loops -flto -march=native"


# compile the code
# module load gcc
# make realclean
# make OPT="$OPT_FLAGS"

# create necesary files and directories
export ANALYZER_DIR=${RESULT_FOLDER}/analyzer_files

# create necesary files and directories
#mkdir results
touch $RESULT_FOLDER/setup.txt # file for setup
lscpu >> ../results/$EXPNAME/setup.txt # write setup to file
echo "Jobid: ${LSB_JOBID}" >> $RESULT_FOLDER/setup.txt
echo "Number of available cores: " $LSB_DJOB_NUMPROC >> $RESULT_FOLDER/setup.txt

# safe compile options
# cp compile.log $RESULT_FOLDER/compile_logs/compile.log

# set default values
export start_T=0
export max_iters=200
export tol=0.01
export N=100

# save defualts
echo "def_start_T = $start_T" >> $RESULT_FOLDER/setup.txt
echo "def_max_iters = $max_iters" >> $RESULT_FOLDER/setup.txt
echo "def_tol = $tol" >> $RESULT_FOLDER/setup.txt
echo "def_N = $N" >> $RESULT_FOLDER/setup.txt

# run scripts
# sh exp_error_converge.sh # runs the test case and experiments with different sizes of N and tolerance - used to find error
# sh exp_iteration_convergence.sh # runs the program and experiments with different granularity (N) - used to find convergence of iterations and iterations pr second
# sh exp_memory_scalability.sh # runs the program and experiments with different number of grid sizes wboth with and without both optimization and parallelization - used to find memory scalability
# sh exp_thread_scalability.sh # runs the program and experiments with different number of threads - used to find thread scalability
sh exp_dynamic_allocation.sh
# copy hpc logs to results folder
if [ "$LSB_JOBID" != "" ]; then
    cp ../hpc_logs/${LSB_JOBID}.out $RESULT_FOLDER/hpc_logs.out
    cp ../hpc_logs/${LSB_JOBID}.err $RESULT_FOLDER/hpc_logs.err
fi
