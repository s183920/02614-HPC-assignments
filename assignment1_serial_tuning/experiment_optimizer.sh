#!/bin/sh

# folder for hpc logs
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
#BSUB -W 25
# uncomment the following line, if you want to assure that your job has
# a whole CPU for itself (shared L3 cache)
#BSUB -R "span[hosts=1] affinity[socket(1)]"

# set compiler flags
OPT_FLAGS=(
    "-g"
    "-g -O3" 
    "-g -O3 -funroll-loops"
    "-g -O3 -funroll-loops -flto"
    "-g -O3 -funroll-loops -flto -march=native"
    "-g -O3 -funroll-loops -flto -march=native -ffast-math"
    "-g -O3 -funroll-loops -flto -march=native -ffast-math -funsafe-loop-optimizations"
    "-g -O3 -funroll-loops -flto -march=native -ffast-math -funsafe-loop-optimizations -funsafe-math-optimizations"
    "-g -O3 -funroll-loops -flto -march=native -ffast-math -funsafe-loop-optimizations -funsafe-math-optimizations -mavx2"
    "-g -O3 -funroll-loops -flto -march=native -ffast-math -funsafe-loop-optimizations -funsafe-math-optimizations -mavx2 -mtune=native"
    "-g -Ofast -funroll-loops -flto -march=native -ffast-math -funsafe-loop-optimizations -funsafe-math-optimizations -mavx2 -mtune=native"
)
# experiments name
# export EXPNAME=C${#OPT_FLAGS[@]}_$(date +%Y%m%d_%H%M%S)
export EXPNAME=part2_$(date +%Y%m%d_%H%M%S)
mkdir -p results/$EXPNAME
mkdir -p results/$EXPNAME/compile_logs

# counter
export COUNT=0

for flag in "${OPT_FLAGS[@]}"; do
    echo "Running compiler optimization experimet with: $flag"

    # compile the code
    module load gcc
    make realclean
    make OPT="$flag"

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
    cp compile.log results/$EXPNAME/compile_logs/compile_$COUNT.log

    # define the mkn values in the MKN variable
    # export SIZES="5 10 20 100 150 250 500 800 1000 1500"
    export SIZES="30 75 200 1200"

    # uncomment and set a reasonable BLKSIZE for the blk version
    export BLKSIZE=1

    # permuations
    export PERMS="mkn mnk kmn knm nmk nkm"
    # export PERMS="lib"

    # driver options
    # export MATMULT_RESULTS=      # {[0]|1}       print result matrices (in Matlab format, def: 0)
    export MATMULT_COMPARE=0   # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
    export MFLOPS_MIN_T=5.0         # [3.0]         the minimum run-time (def: 3.0 s)
    # export MFLOPS_MAX_IT=1000        # [infinity]    max. no of iterations; set if you want to do profiling.

    # perform the experiments
    sh experiment_size.sh # uncomment to perform size experiment
    export COUNT=$((COUNT+1))
done

# make plot
echo "Making optimizer plot..."
deactivate
source ../../hpc_env/bin/activate
python3 compiler_optimization.py --exp $EXPNAME --folder results

# copy hpc logs to results folder
if [ "$LSB_JOBID" != "" ]; then
    cp hpc_logs/${LSB_JOBID}.out results/$EXPNAME/hpc_logs.out
    cp hpc_logs/${LSB_JOBID}.err results/$EXPNAME/hpc_logs.err
fi


