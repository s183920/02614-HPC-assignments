#!/bin/sh

# needed modules
module load python3


# create necesary files and directories
mkdir -p $SIZE_DIR # directory for output files
touch results/$EXPNAME/setup_sizes.txt # file for setup of size exp


# define the mkn values in the MKN variable
SIZES="10 50 100 150 250 350 500 600"
echo "SIZES=$SIZES" >> results/$EXPNAME/setup_sizes.txt

# loop over permutations
for PERM in $PERMS ; do
echo "PERM=$PERM"
# loop over matrix sizes
FILENAME="${SIZE_DIR}/${PERM}.txt"
# touch FILENAME
for S in $SIZES; do
    ./$EXECUTABLE $PERM $S $S $S $BLKSIZE >> $FILENAME
done
done