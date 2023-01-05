#!/bin/sh

#!/bin/bash

BLKSIZES={16..256..16}

# create necesary files and directories
mkdir $SIZE_DIR # directory for output files
touch results/$EXPNAME/setup_sizes.txt # file for setup of size exp


# define the mkn values in the MKN variable
SIZES="300"
REPEATS=1
echo "SIZES=$SIZES\nBLKSIZE=$BLKSIZE" >> results/$EXPNAME/setup_sizes.txt


# loop over permutations
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
