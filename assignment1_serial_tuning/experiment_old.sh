#!/bin/sh

#!/bin/bash
# 02614 - High-Performance Computing, January 2022
# 
# batch script to run matmult on a decidated server in the hpcintro
# queue
#
# Author: Bernd Dammann <bd@cc.dtu.dk>
#
#BSUB -J mm_batch
#BSUB -o mm_batch_%J.out
#BSUB -q hpcintro
#BSUB -n 1
#BSUB -R "rusage[mem=2048]"
#BSUB -W 15
# uncomment the following line, if you want to assure that your job has
# a whole CPU for itself (shared L3 cache)
#BSUB -R "span[hosts=1] affinity[socket(1)]"

# needed modules
module load studio
module load python3

# define the driver name to use
EXECUTABLE=matmult_c.gcc

# experiments name
EXPNAME=matmult_test_$(date +%Y%m%d_%H%M%S)
ANALYZER_DIR=results/${EXPNAME}/analyzer_files
OUTPUT_DIR=results/${EXPNAME}/output_files

# create necesary files and directories
mkdir results/$EXPNAME
touch results/$EXPNAME/setup.txt # file for setup
mkdir $ANALYZER_DIR # directory for analyzer files
mkdir $OUTPUT_DIR # directory for output files

# define the mkn values in the MKN variable
SIZES="100 200"
echo "SIZES=$SIZES" >> results/$EXPNAME/setup.txt

# uncomment and set a reasonable BLKSIZE for the blk version
#
# BLKSIZE=1

# analyzer options
JID=${LSB_JOBID}
# the example below is for L1 hits, L1 misses, L2 hits, L2 misses
HWCOUNT="-h dch,on,dcm,on,l2h,on,l2m,on"


# enable(1)/disable(0) result checking
export MATMULT_COMPARE=0

# start the collect command with the above settings

# loop over permutations
PERMS="mkn mnk kmn knm nmk nkm"
for PERM in $PERMS ; do
echo "PERM=$PERM"
# loop over matrix sizes
# FILENAME=results/$EXPNAME/$PERM
touch $OUTPUT_DIR/$PERM.txt
for S in $SIZES; do
    # EXPOUT="${ANALYZER_DIR}/${PERM}_$S.er"
    ./$EXECUTABLE $PERM $S $S $S $BLKSIZE >> $OUTPUT_DIR/$PERM.txt
    # result=$(collect -o $EXPOUT $HWCOUNT ./$EXECUTABLE $PERM $S $S $S $BLKSIZE -p on -S on)
    # output=$(echo "$result" | grep -Po '"stdout":.*?[^\\]",' | awk -F ':' '{print $2}' | tr -d '",')
    # echo "$output" >> results/$EXPNAME/$PERM.txt
done
done