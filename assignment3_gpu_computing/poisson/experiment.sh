#!/bin/sh

# experiments name
EXPNAME=question6_$(date +%Y%m%d_%H%M%S)
OUTDIR=results/$EXPNAME/output_files
mkdir -p results/$EXPNAME
mkdir -p $OUTDIR
mkdir -p hpc_logs
mkdir -p results/$EXPNAME/compile_logs

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
### BSUB -w "exit(15152889)" # job dependency
#BSUB -R "rusage[mem=2048]"
#BSUB -W 25
# uncomment the following line, if you want to assure that your job has
# a whole CPU for itself (shared L3 cache)
### BSUB -R "span[hosts=1] affinity[socket(1)]"

# compile the code
  module load nvhpc/22.11-nompi
  module load cuda/11.8
  module load gcc/11.3.0-binutils-2.38
make clean
make OPT="$OPT_FLAGS"

# define the driver name to use
# cp matmult_c.gcc results/$EXPNAME/matmult_c.gcc
export EXECUTABLE=possion

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
export DEFAULT_N=10           # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
export DEFAULT_MAX_IT=1000    # [3.0]         max. no of iterations; set if you want to do profiling.
export DEFAULT_T=0.32         # [0.32]        tolerance
export DEFAULT_START_T=0      #Start time
export DEFAULT_OUTPUT_ARG=0   #No output 
export VERSIONS="0 1"
# run the driver
fnum=0
# total=$VERSIONS* // VERSIONS * SIZES
for VERSION in $VERSIONS; do
    for S in $SIZES; do
        echo "Starting run $fnum using $VERSION with grid size $DEFAULT_N"
        echo "version: $VERSION" > $OUTDIR/run_$fnum.txt
        echo "size: $DEFAULT_N" >> $OUTDIR/run_$fnum.txt
        echo "result: $(./$EXECUTABLE $DEFAULT_N $DEFAULT_MAX_IT $DEFAULT_T $DEFAULT_START $DEFAULT_OUTPUT_ARG $VERSION)"  >> $OUTDIR/run_$fnum.txt
        fnum=$((fnum+1))
    done
done

# copy hpc logs to results folder
mkdir -p results/$EXPNAME/hpc_logs
if [ "$LSB_JOBID" != "" ]; then
    cp hpc_logs/${LSB_JOBID}.out results/$EXPNAME/hpc_logs.out
    cp hpc_logs/${LSB_JOBID}.err results/$EXPNAME/hpc_logs.err
fi
