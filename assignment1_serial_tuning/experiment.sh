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
# #BSUB -R "span[hosts=1] affinity[socket(1)]"

# define the driver name to use
# valid values: matmult_c.studio, matmult_f.studio, matmult_c.gcc or
# matmult_f.gcc
#
EXECUTABLE=matmult_c.gcc

# experiments name
EXPNAME=matmult_test_$(date +%Y%m%d_%H%M%S)
# EXPNAME="$EXPNAME_$(date +%Y%m%d_%H%M%S)"
mkdir results/$EXPNAME
touch results/$EXPNAME/setup.txt

# define the mkn values in the MKN variable
SIZES="100 200"
echo "SIZES=$SIZES" >> results/$EXPNAME/setup.txt

# uncomment and set a reasonable BLKSIZE for the blk version
#
# BLKSIZE=1

# enable(1)/disable(0) result checking
export MATMULT_COMPARE=0

# start the collect command with the above settings

# loop over permutations
PERMS="mkn mnk kmn knm nmk nkm"
for PERM in $PERMS ; do
echo "PERM=$PERM"
# loop over matrix sizes
FILENAME=results/$EXPNAME/$PERM.txt
touch $FILENAME
for S in $SIZES; do
    ./$EXECUTABLE $PERM $S $S $S $BLKSIZE >> $FILENAME
done
done