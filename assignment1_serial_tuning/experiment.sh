#!/bin/sh

# experiments name
export EXPNAME=big_size_$(date +%Y%m%d_%H%M%S)
mkdir -p results/$EXPNAME
mkdir -p hpc_logs

#!/bin/bash
# 02614 - High-Performance Computing, January 2022
# 
# batch script to run matmult on a decidated server in the hpcintro
# queue
#
# Author: Bernd Dammann <bd@cc.dtu.dk>
#
#BSUB -J mm_batch
#BSUB -o hpc_logs/%J.out
#BSUB -e hpc_logs/%J.err
#BSUB -q hpcintro
#BSUB -n 1
#BSUB -R "rusage[mem=2048]"
#BSUB -W 15
# uncomment the following line, if you want to assure that your job has
# a whole CPU for itself (shared L3 cache)
#BSUB -R "span[hosts=1] affinity[socket(1)]"

# set compiler flags
OPT_FLAGS="-g -O3"


# compile the code
module load gcc
make realclean
make OPT="$OPT_FLAGS"

# define the driver name to use
# cp matmult_c.gcc results/$EXPNAME/matmult_c.gcc
export EXECUTABLE=matmult_c.gcc

# create necesary files and directories
export ANALYZER_DIR=results/${EXPNAME}/analyzer_files
export SIZE_DIR=results/${EXPNAME}/output_files

# create necesary files and directories
#mkdir results
touch results/$EXPNAME/setup.txt # file for setup
lscpu >> results/$EXPNAME/setup.txt # write setup to file
echo "Jobid: ${LSB_JOBID}" >> results/$EXPNAME/setup.txt # write setup to file

# safe compile options
cp compile.log results/$EXPNAME/compile.log

# define the mkn values in the MKN variable
export SIZES="5 10 100 150 250 350 500 600 700 800"

# uncomment and set a reasonable BLKSIZE for the blk version
export BLKSIZE=1

# permuations
export PERMS="mkn mnk kmn knm nmk nkm"

# driver options
# export MATMULT_RESULTS=      # {[0]|1}       print result matrices (in Matlab format, def: 0)
export MATMULT_COMPARE=0   # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
# export MFLOPS_MIN_T=         # [3.0]         the minimum run-time (def: 3.0 s)
# export MFLOPS_MAX_IT=1000        # [infinity]    max. no of iterations; set if you want to do profiling.

# perform the experiments
sh experiment_size.sh # uncomment to perform size experiment
# sh experiment_blk.sh # uncomment to perform blk experiment
#sh experiment_analyzer.sh # uncomment to perform tuning experiment

# copy hpc logs to results folder
if [ "$LSB_JOBID" != "" ]; then
    cp hpc_logs/${LSB_JOBID}.out results/$EXPNAME/hpc_logs.out
    cp hpc_logs/${LSB_JOBID}.err results/$EXPNAME/hpc_logs.err
fi
