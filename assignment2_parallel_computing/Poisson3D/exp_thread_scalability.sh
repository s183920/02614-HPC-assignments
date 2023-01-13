#!/bin/bash

# used to answer last question in part 3 and part 4

EXP_DIR="$RESULT_FOLDER/exp_thread_scalability"
OUT_DIR="$EXP_DIR/output_files"
mkdir -p $EXP_DIR
mkdir -p $OUT_DIR
touch "$EXP_DIR/setup.txt" # file for setup

# asert more than 1 thread available
# if [ "$LSB_DJOB_NUMPROC" = "" ] || [ "$LSB_DJOB_NUMPROC" = "1" ]; then
#     echo "Only one core available, exiting"; 
#     exit $ERRCODE;
# fi

# compile code
module load gcc
make realclean
make PARA=-fopenmp

# set Ns to test
Ns="10 50 100"
thread_step_size=1

LSB_DJOB_NUMPROC=4



# save settings
echo "start_T = $start_T" >> $EXP_DIR/setup.txt
echo "max_iters = $max_iters" >> $EXP_DIR/setup.txt
echo "tol = $tol" >> $EXP_DIR/setup.txt
echo "Ns = $Ns" >> $EXP_DIR/setup.txt
echo "Num_threads = $LSB_DJOB_NUMPROC" >> $EXP_DIR/setup.txt
echo "thread_step_size = $thread_step_size" >> $EXP_DIR/setup.txt


# run tests - WARNING the programs does not ouput the needed data yet
echo "Running thread scalability experiment for parallel code"
for n_threads in $( eval echo {1..$LSB_DJOB_NUMPROC..$thread_step_size} ); do
    echo "Threads = $n_threads"
    for n in $Ns; do
        echo "N = $n"
        OMP_NUM_THREADS=$n_threads ./poisson_j $n $max_iters $tol $start_T 0 > $OUT_DIR/output_j_N_${n}_threads_${n_threads}_ser.txt
        OMP_NUM_THREADS=$n_threads ./poisson_gs $n $max_iters $tol $start_T 0 > $OUT_DIR/output_gs_N_${n}_threads_${n_threads}_ser.txt
        OMP_NUM_THREADS=$n_threads ./poisson_j $n $max_iters $tol $start_T 0 1 > $OUT_DIR/output_j_N_${n}_threads_${n_threads}_sim.txt
        OMP_NUM_THREADS=$n_threads ./poisson_gs $n $max_iters $tol $start_T 0 1 > $OUT_DIR/output_gs_N_${n}_threads_${n_threads}_sim.txt
    done
done