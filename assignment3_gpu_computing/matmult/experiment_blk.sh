#!/bin/sh

#BSUB -J block_size
#BSUB -o hpc_logs/%J.out
#BSUB -e hpc_logs/%J.err
#BSUB -q hpcintrogpu
#BSUB -n 16
#BSUB -R "span[hosts=1]"
#BSUB -gpu "num=1:mode=exclusive_process"
#BSUB -W 10
#BSUB -R "rusage[mem=2048]" 
#########################################################################

#### Compiler Options
OPT_FLAGS="-g -fast -Msafeptr -Minfo -mp=gpu -gpu=pinned -gpu=cc80 -gpu=lineinfo -cuda -mp=noautopar"
THREADS=16
export OMP_NUM_THREADS=$THREADS
TEAMS=108

#### Driver Options
export EXECUTABLE=matmult_c.nvc++ # Driver Name

# export MATMULT_RESULTS=      # {[0]|1}       print result matrices (in Matlab format, def: 0)
export MATMULT_COMPARE=1   # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
export MFLOPS_MIN_T=3.0        # [3.0]         the minimum run-time (def: 3.0 s)
# export MFLOPS_MAX_IT=100       #     remove = ["", " ", "#"][infinity]    max. no of iterations; set if you want to do profiling.

#### Experiment Options
BLKSIZES=1
SIZE="2048"
VERSIONS="mkn_omp"
export EXPNAME=blk_size_${SIZE}_$(date +%Y%m%d_%H%M%S) #Name of Experiment
export EXPPATH=results/${EXPNAME} # Path to experiment folder
# Sub-Folders
export ANALYZER_DIR=$EXPPATH/analyzer_files
export OUTPUT_PATH=$EXPPATH/output_files

#### Modules
module load nvhpc/22.11-nompi
module load cuda/11.8
module load gcc/11.3.0-binutils-2.38



#### Run Experiment
mkdir -p $OUTPUT_PATH
for V in $VERSIONS; do
for BLK in $(eval echo $BLKSIZES) ; do
FILENAME=${V}_${BLK}.txt
make clean
make THREADS=$THREADS TEAMS=$TEAMS BLOCK_SIZE=$BLK

echo "version: $V" >> $OUTPUT_PATH/$FILENAME
echo "block_size: $BLK" >> $OUTPUT_PATH/$FILENAME
echo "size: $SIZE" >> $OUTPUT_PATH/$FILENAME
echo "OMP_NUM_THREADS=$THREADS ./$EXECUTABLE $V $SIZE $SIZE $SIZE $BLK >> $FILENAME"
OMP_NUM_THREADS=16 ./$EXECUTABLE $V $SIZE $SIZE $SIZE $BLK >> $OUTPUT_PATH/$FILENAME
done
done


#### HPC Logs
mkdir -p $EXPPATH/hpc_logs
# copy hpc logs to results folder
if [ "$LSB_JOBID" != "" ]; then
    cp hpc_logs/${LSB_JOBID}.out $EXPPATH/hpc_logs.out
    cp hpc_logs/${LSB_JOBID}.err $EXPPATH/hpc_logs.err
fi

#### Compile Logs
# safe compile options
mkdir -p $EXPPATH/compile_logs/
touch $EXPPATH/compile_logs/compile.log
cp compile.log $EXPPATH/compile_logs/compile.log





