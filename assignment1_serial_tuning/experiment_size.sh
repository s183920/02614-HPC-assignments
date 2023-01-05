#!/bin/sh

# needed modules
module load python3
source ../../hpc_env/bin/activate


# create necesary files and directories
mkdir -p $SIZE_DIR # directory for output files
touch results/$EXPNAME/setup_sizes.txt # file for setup of size exp


# save the mkn values in the MKN variable
echo "SIZES=$SIZES" >> results/$EXPNAME/setup_sizes.txt

# write message
echo "Running size experiment..."

# loop over permutations
for PERM in $PERMS ; do
echo "PERM=$PERM"
# loop over matrix sizes
FILENAME="${SIZE_DIR}/${PERM}.txt"
# touch FILENAME
for S in $SIZES; do
    echo "SIZE=$S"
    ./$EXECUTABLE $PERM $S $S $S $BLKSIZE >> $FILENAME
done
done

# make plot
echo "Making plot..."
python3 plot_size.py --exp $EXPNAME --folder results