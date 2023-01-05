#!/bin/sh

#!/bin/bash

BLKSIZES="5 6 7 8 9 10 11 12 13 14 15"

# needed modules
module load python3


# create necesary files and directories
mkdir $SIZE_DIR # directory for output files
touch results/$EXPNAME/setup_sizes.txt # file for setup of size exp


# define the mkn values in the MKN variable
SIZES="20 30"
echo "SIZES=$SIZES; BLKSIZE=$BLKSIZE" >> results/$EXPNAME/setup_sizes.txt


# loop over permutations
for S in $SIZES; do
FILENAME="${SIZE_DIR}/${S}.txt"
for BLK in $BLKSIZES ; do
echo "./$EXECUTABLE blk $S $S $S $BLK >> $FILENAME"
./$EXECUTABLE "blk" $S $S $S $BLK >> $FILENAME
done
done