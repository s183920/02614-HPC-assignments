#!/bin/sh

# experiments name
EXPNAME=question2_$(date +%Y%m%d_%H%M%S)
OUTDIR=results/$EXPNAME/output_files
mkdir -p results/$EXPNAME
mkdir -p $OUTDIR
mkdir -p hpc_logs
mkdir -p results/$EXPNAME/compile_logs

# 02614 - High-Performance Computing, January 2022
# 
# batch script to run matmult on a decidated server in the hpcintro
# queue
#
# Author: Bernd Dammann <bd@cc.dtu.dk>
#
#BSUB -J Q2
#BSUB -o hpc_logs/%J.out
#BSUB -e hpc_logs/%J.err
#BSUB -q hpcintrogpu
#BSUB -n 4
#BSUB -R "span[hosts=1]"
#BSUB -gpu "num=1:mode=exclusive_process"
#BSUB -W 10
#BSUB -R "rusage[mem=2048]" 


# compile the code
module load cuda/11.8
module load gcc/11.3.0-binutils-2.38
module load nvhpc/22.11-nompi
make clean
make 

# define the driver name to use
# cp matmult_c.gcc results/$EXPNAME/matmult_c.gcc
export EXECUTABLE=matmult_c.nvc++

# create necesary files and directories
#mkdir results
touch results/$EXPNAME/setup.txt # file for setup
lscpu >> results/$EXPNAME/setup.txt # write setup to file
echo "Jobid: ${LSB_JOBID}" >> results/$EXPNAME/setup.txt # write setup to file

# safe compile options
# cp compile.log results/$EXPNAME/compile_logs/compile.log

# define the mkn values in the MKN variable
export SIZES="30"

# uncomment and set a reasonable BLKSIZE for the blk version
export BLKSIZE=30

# permuations
export VERSIONS="mkn_offload mnk_offload"

# driver options
# export MATMULT_RESULTS=      # {[0]|1}       print result matrices (in Matlab format, def: 0)
export MATMULT_COMPARE=1   # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
export MFLOPS_MIN_T=3         # [3.0]         the minimum run-time (def: 3.0 s)
# export MFLOPS_MAX_IT=1000        # [infinity]    max. no of iterations; set if you want to do profiling.

# run the driver
fnum=0
# total=$VERSIONS* // VERSIONS * SIZES
for VERSION in $VERSIONS; do
    for S in $SIZES; do
        echo "Starting run $fnum using $VERSION with size $SIZE"
        echo "version: $VERSION" > $OUTDIR/run_$fnum.txt
        echo "size: $S" >> $OUTDIR/run_$fnum.txt
        echo "result: $(./$EXECUTABLE $VERSION $S $S $S)"  >> $OUTDIR/run_$fnum.txt
        fnum=$((fnum+1))
    done
done



# copy hpc logs to results folder
mkdir -p results/$EXPNAME/hpc_logs
if [ "$LSB_JOBID" != "" ]; then
    cp hpc_logs/${LSB_JOBID}.out results/$EXPNAME/hpc_logs/log.out
    cp hpc_logs/${LSB_JOBID}.err results/$EXPNAME/hpc_logs/log.err
fi
