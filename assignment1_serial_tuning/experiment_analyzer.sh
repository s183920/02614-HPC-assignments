#!/bin/sh

# needed modules
module load studio

# create necesary files and directories
mkdir $ANALYZER_DIR # directory for analyzer files
touch results/$EXPNAME/setup_sizes.txt # file for setup of size exp

# define the mkn values in the MKN variable
SIZES="100"
echo "SIZES=$SIZES" >> results/$EXPNAME/setup.txt

# analyzer options
JID=${LSB_JOBID}
# TODO: Add L3
HWCOUNT="-h dch,on,dcm,on,l2h,on,l2m,on" # L1 hits, L1 misses, L2 hits, L2 misses


# loop over permutations

for PERM in $PERMS ; do
echo "PERM=$PERM"
# loop over matrix sizes
# FILENAME=results/$EXPNAME/$PERM
for S in $SIZES; do
    EXPOUT="${ANALYZER_DIR}/${PERM}_$S.er"
    collect -o $EXPOUT $HWCOUNT ./$EXECUTABLE $PERM $S $S $S $BLKSIZE
done
done