#!/bin/sh

#!/bin/bash

BLKSIZES="2 4 8 16 32"

# create necesary files and directories
mkdir $SIZE_DIR # directory for output files
touch results/$EXPNAME/setup_sizes.txt # file for setup of size exp


# define the mkn values in the MKN variable
SIZES="30"
echo $"SIZES=$SIZES\nBLKSIZE=$BLKSIZE" >> results/$EXPNAME/setup_sizes.txt


# loop over permutations
for S in $SIZES; do
FILENAME="${SIZE_DIR}/${S}.txt"
for BLK in $(eval echo $BLKSIZES) ; do
echo $BLK
./$EXECUTABLE "blk" $S $S $S $BLK >> $FILENAME
done
done


