#!/bin/sh
#BSUB -J proftest
#BSUB -q hpcintrogpu
#BSUB -n 16
#BSUB -R "span[hosts=1]"
#BSUB -gpu "num=1:mode=exclusive_process"
#BSUB -W 10
#BSUB -R "rusage[mem=2048]" 

# if [ $# -ne 2 ]; then
#     echo "Usage: $0 <m> <n> <k>"
#     exit 1
# fi
version=blk_offload
size=2048
FILE=profiles/profile_$(date +%Y%m%d_%H%M%S)_$LSB_JOBID.txt
mkdir -p profiles

if [ $# -eq 1 ]; then
    version=$1
fi

if [ $# -eq 2 ]; then
    version=$1
    size=$2
fi

if [ $# -eq 3 ]; then
    version=$1
    size=$2
    FILE=$3
fi


# mkdir -p $FILEDIR
export TMPDIR=$__LSF_JOB_TMPDIR__
module load cuda/11.8
module load gcc/11.3.0-binutils-2.38
module load nvhpc/22.11-nompi
export MFLOPS_MAX_IT=1

export OMP_NUM_THREADS=16   
# command line output
echo "$(nv-nsight-cu-cli \
        --section MemoryWorkloadAnalysis \
        --section MemoryWorkloadAnalysis_Chart \
        --section ComputeWorkloadAnalysis \
        --section Occupancy \
        --section SpeedOfLight \
        ./matmult_c.nvc++ $version $size $size $size)" > $FILE