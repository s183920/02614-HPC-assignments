#!/bin/bash

# used to answer part 1 and 2

EXP_DIR="$RESULT_FOLDER/exp_iteration_convergence"
OUT_DIR="$EXP_DIR/output_files"
mkdir -p $EXP_DIR
mkdir -p $OUT_DIR
touch "$EXP_DIR/setup.txt" # file for setup

# compile code
module load gcc
make realclean
make 

# set Ns to test
Ns="10 20 30 40 50 100"

# save settings
echo "start_T = $start_T" >> $EXP_DIR/setup.txt
echo "max_iters = $max_iters" >> $EXP_DIR/setup.txt
echo "tol = $tol" >> $EXP_DIR/setup.txt
echo "Ns = $Ns" >> $EXP_DIR/setup.txt


# run tests - WARNING the programs does not ouput the needed data yet
echo "Running iteration convergence experiment"
for n in $Ns; do
    echo "N = $n"
    ./poisson_gs $n $max_iters $tol start_T 0 > $OUT_DIR/output_gs_N_${n}.txt
    ./poisson_j $n $max_iters $tol start_T 0 > $OUT_DIR/output_j_N_${n}.txt
done;echo
