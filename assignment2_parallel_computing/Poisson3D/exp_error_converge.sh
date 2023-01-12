#!/bin/bash

# used to answer part 1 and 2

EXP_DIR="$RESULT_FOLDER/exp_error_convergence"
OUT_DIR="$EXP_DIR/output_files"
mkdir -p $EXP_DIR
mkdir -p $OUT_DIR
touch "$EXP_DIR/setup.txt" # file for setup

# get needed modules
module load gcc
module load python3
source ../../../venv/bin/activate

# compile code
make realclean
make test test_ext=_test

# set Ns to test
Ns="10 20 30 40 50 60 70 80 90 100"
#Ns="10 30"

# set tolerances to test
# tolerances="0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001"
tolerances="0.1 0.01 0.0001"

# save settings
echo "start_T = $start_T" >> $EXP_DIR/setup.txt
echo "max_iters = $max_iters" >> $EXP_DIR/setup.txt
echo "fixed_tol = $tol" >> $EXP_DIR/setup.txt
echo "Ns = $Ns" >> $EXP_DIR/setup.txt
echo "fixed_N = $N" >> $EXP_DIR/setup.txt
echo "tolerances = $tolerances" >> $EXP_DIR/setup.txt

# run tests - WARNING the programs does not ouput the needed data yet
echo "Running error vs N experiment for tol=$tol"
for n in $Ns; do
    echo "N = $n"
    ./poisson_gs $n $max_iters $tol $start_T 0 > $OUT_DIR/output_gs_N_${n}.txt
    ./poisson_j $n $max_iters $tol $start_T 0 > $OUT_DIR/output_j_N_${n}.txt
done;echo

echo "Running error vs tolerance experiment for N=$N"
for tol in $tolerances; do
    echo "tol = $tol"
    ./poisson_gs $N $max_iters $tol start_T 0 > $OUT_DIR/output_gs_tol_${tol}.txt
    ./poisson_j $N $max_iters $tol start_T 0 > $OUT_DIR/output_j_tol_${tol}.txt
done;echo

# make plot
echo "Making plots of error convergence..."
python3 plot_error_convergence.py --save_folder $EXP_DIR