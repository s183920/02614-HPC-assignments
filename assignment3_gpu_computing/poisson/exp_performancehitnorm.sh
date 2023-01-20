#!/bin/sh

# experiments name
EXPNAME=part2_$(date +%Y%m%d_%H%M%S)
OUTDIR=results/$EXPNAME/output_files
mkdir -p results/$EXPNAME
mkdir -p $OUTDIR
mkdir -p hpc_logs
mkdir -p results/$EXPNAME/compile_logs

#BSUB -J jacobi_outputs
#BSUB -o hpc_logs/%J.out
#BSUB -e hpc_logs/%J.err
#BSUB -q hpcintrogpu
#BSUB -n 4
#BSUB -R "span[hosts=1]"
#BSUB -gpu "num=1:mode=exclusive_process"
#BSUB -W 10
#BSUB -R "rusage[mem=10GB]"
# uncomment the following line, if you want to assure that your job has
# a whole CPU for itself (shared L3 cache)

export CUDA_VISIBLE_DEVICES=0,1


# compile the code
module load nvhpc/22.11-nompi
module load cuda/11.8
module load gcc/11.3.0-binutils-2.38
#make clean

# define the driver name to use
# cp matmult_c.gcc results/$EXPNAME/matmult_c.gcc
export EXECUTABLE=poisson
# create necesary files and directories
export ANALYZER_DIR=results/${EXPNAME}/analyzer_files
export SIZE_DIR=results/${EXPNAME}/output_files

# create necesary files and directories
#mkdir results
touch results/$EXPNAME/setup.txt # file for setup
lscpu >> results/$EXPNAME/setup.txt # write setup to file
echo "Jobid: ${LSB_JOBID}" >> results/$EXPNAME/setup.txt # write setup to file
# export PERMS="nat lib"

# driver options
#export MATMULT_RESULTS=      # {[0]|1}       print result matrices (in Matlab format, def: 0)
export DEFAULT_N=200           # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
export DEFAULT_MAX_IT=100    # [3.0]         max. no of iterations; set if you want to do profiling.
export DEFAULT_T=0         # [0.32]        tolerance
export DEFAULT_START_T=0      #Start time
export DEFAULT_OUTPUT_ARG=0   #No output 
export VERSIONS="0 4"
export SIZES="100 200 300 400 500 600 700 800 900 1000"
export NUM_THREADS="16"
# run the driver
# total=$VERSIONS* // VERSIONS * SIZES
for n_threads in $NUM_THREADS; do
    make clean
    make THREADS=$n_threads -B
    for VERSION in $VERSIONS; do
        for S in $SIZES; do
            echo "Starting run using $VERSION with grid size $S and $n_threads threads"
            ./$EXECUTABLE $S $DEFAULT_MAX_IT $DEFAULT_T 0 0 $VERSION > $OUTDIR/Jacobi_N${S}_${n_threads}_v${VERSION}.txt
        done
    done
done
make clean
make
export SIZES2="100 200 300 400 500 600"
for S in $SIZES2; do
    for n_threads in $NUM_THREADS; do
        echo "Starting run using 1 with grid size $S and $n_threads threads"
        OMP_NUM_THREADS=$n_threads ./$EXECUTABLE $S $DEFAULT_MAX_IT $DEFAULT_T 0 0 1 > $OUTDIR/Jacobi_N${S}_${n_threads}_v1.txt
    done
done


# copy hpc logs to results folder
mkdir -p results/$EXPNAME/hpc_logs
if [ "$LSB_JOBID" != "" ]; then
    cp hpc_logs/${LSB_JOBID}.out results/$EXPNAME/hpc_logs.out
    cp hpc_logs/${LSB_JOBID}.err results/$EXPNAME/hpc_logs.err
fi
