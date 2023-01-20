#!/bin/sh

# experiments name
EXPNAME=q5_$(date +%Y%m%d_%H%M%S)
OUTDIR=results/$EXPNAME/output_files
PROFILE_DIR=results/$EXPNAME/profiles
mkdir -p results/$EXPNAME
mkdir -p $OUTDIR
mkdir -p hpc_logs
mkdir -p results/$EXPNAME/compile_logs
mkdir -p $PROFILE_DIR

# 02614 - High-Performance Computing, January 2022
# 
# batch script to run matmult on a decidated server in the hpcintro
# queue
#
# Author: Bernd Dammann <bd@cc.dtu.dk>
#
#BSUB -J Q5
#BSUB -o hpc_logs/%J.out
#BSUB -e hpc_logs/%J.err
#BSUB -q hpcintrogpu
#BSUB -n 4
#BSUB -R "span[hosts=1]"
#BSUB -gpu "num=1:mode=exclusive_process"
#BSUB -W 10
#BSUB -R "rusage[mem=2048]" 

# SETTINGS
SIZES="128 256 512 1024 2048 3000 4096 8192"
THREADS=16

VERSIONS="lib_offload lib"

# driver options
# export MATMULT_RESULTS=      # {[0]|1}       print result matrices (in Matlab format, def: 0)
export MATMULT_COMPARE=0   # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
export MFLOPS_MIN_T=3         # [3.0]         the minimum run-time (def: 3.0 s)
# export MFLOPS_MAX_IT=1000        # [infinity]    max. no of iterations; set if you want to do profiling.



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
/appl/cuda/11.8.0/extras/demo_suite/deviceQuery >> results/$EXPNAME/setup.txt # write setup to file
echo "Jobid: ${LSB_JOBID}" >> results/$EXPNAME/setup.txt # write setup to file

# safe compile options
# cp compile.log results/$EXPNAME/compile_logs/compile.log

# run the driver
fnum=0


for VERSION in $VERSIONS; do
    for S in $SIZES; do
        echo "Starting run $fnum using $VERSION with size $S, threads $THREADS"
        echo "version: $VERSION" > $OUTDIR/run_$fnum.txt
        echo "size: $S" >> $OUTDIR/run_$fnum.txt
        echo "threads: $THREADS" >> $OUTDIR/run_$fnum.txt
        echo "$(./$EXECUTABLE $VERSION $S $S $S)"  >> $OUTDIR/run_$fnum.txt
        fnum=$((fnum+1))
    done
done

# profile the code
sh profiler.sh lib_offload 2048 $PROFILE_DIR/lib_offload
sh profiler.sh lib 2048 $PROFILE_DIR/lib



# copy hpc logs to results folder
mkdir -p results/$EXPNAME/hpc_logs
if [ "$LSB_JOBID" != "" ]; then
    cp hpc_logs/${LSB_JOBID}.out results/$EXPNAME/hpc_logs/log.out
    cp hpc_logs/${LSB_JOBID}.err results/$EXPNAME/hpc_logs/log.err
fi

# plot
echo "Plotting results for $EXPNAME"
source ../../../hpc_env/bin/activate
python3 plot_functions.py -q 5 --exp $EXPNAME
