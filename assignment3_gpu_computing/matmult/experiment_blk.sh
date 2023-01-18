#!/bin/sh

#!/bin/bash
#!/bin/sh


############ BSUB Tags ##############
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
#########################################################################

#### Compiler Options
OPT_FLAGS="-g -O3 -funroll-loops -flto -march=native -ffast-math -funsafe-loop-optimizations -funsafe-math-optimizations -mavx2"

#### Driver Options
export EXECUTABLE=matmult_c.nvc++ # Driver Name

# export MATMULT_RESULTS=      # {[0]|1}       print result matrices (in Matlab format, def: 0)
export MATMULT_COMPARE=1   # {0|[1]}       control result comparison (def: 1); enable(1)/disable(0) result checking
export MFLOPS_MIN_T=5         # [3.0]         the minimum run-time (def: 3.0 s)
# export MFLOPS_MAX_IT=1000        # [infinity]    max. no of iterations; set if you want to do profiling.

#### Experiment Options
export EXPNAME=blk_size_30_p2_$(date +%Y%m%d_%H%M%S) #Name of Experiment
export EXPPATH=results/${EXPNAME} # Path to experiment folder
BLKSIZES={1..500..10}
SIZES="1000"
REPEATS=1
# Sub-Folders
export ANALYZER_DIR=$EXPPATH/analyzer_files
export OUTPUT_DIR=$EXPPATH/output_files

#### Compile Code
module load gcc
make realclean
make OPT="$OPT_FLAGS"

#### Run Experiment
mkdir -p $EXPPATH
for i in {1..10}; do
echo "iteration: $i"
for S in $SIZES; do
FILENAME="${SIZE_DIR}/${S}.txt"
for BLK in $(eval echo $BLKSIZES) ; do
echo $BLK
./$EXECUTABLE "blk" $S $S $S $BLK >> $FILENAME
done
done
done


#### Setup Logs
mkdir -p $EXPPATH
touch $EXPPATH/setup.txt # file for setup
lscpu >> $SETUP_DIR/setup.txt # write setup to file
echo "Jobid: ${LSB_JOBID}" >> $SETUP_DIR/setup.txt # write setup to file

echo "SIZES=$SIZES\nBLKSIZE=$BLKSIZE" >> $EXPPATH/setup_sizes.txt

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





