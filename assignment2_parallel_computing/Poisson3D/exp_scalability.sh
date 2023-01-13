#!/bin/bash

# used to answer most of part 3

EXP_DIR="$RESULT_FOLDER/exp_memory_scalability"
OUT_DIR="$EXP_DIR/output_files"
COMPILE_LOG_DIR="$EXP_DIR/compile_logs"
mkdir -p $EXP_DIR
mkdir -p $OUT_DIR
mkdir -p $COMPILE_LOG_DIR
touch "$EXP_DIR/setup.txt" # file for setup

# set compiler flags
opt_methods=("without_optimization with_optimization")

# exp settings
compiler_flags= #"-Ofast"
# Ns="50 100 500"
Ns="10 50"
thread_step_size=1

for opt_method in $opt_methods; do
    # compile code
    module load gcc
    make realclean
    if [ opt_method = "without_optimization" ]; then
        make XOPTS= 
    else
        make XOPTS=$compiler_flags
    fi

    # set output dir
    OUT_DIR="$EXP_DIR/output_files/$opt_method"
    mkdir -p $OUT_DIR


    # asert more than 1 thread available
    # if [ "$LSB_DJOB_NUMPROC" = "" ] || [ "$LSB_DJOB_NUMPROC" = "1" ]; then
    #     echo "Only one core available, exiting"; 
    #     exit $ERRCODE;
    # fi

    # save settings
    echo "start_T = $start_T" >> $EXP_DIR/setup.txt
    echo "max_iters = $max_iters" >> $EXP_DIR/setup.txt
    echo "tol = $tol" >> $EXP_DIR/setup.txt
    echo "Ns = $Ns" >> $EXP_DIR/setup.txt


    # run tests - WARNING the programs does not ouput the needed data yet
    # echo "Running memory scalability experiment for serial code"
    version=0
    for n_threads in $( eval echo {1..$LSB_DJOB_NUMPROC..$thread_step_size} ); do
        echo "Threads = $n_threads"
        for n in $Ns; do
            echo "N = $n"
            OMP_NUM_THREADS=${n_threads} ./poisson_j $n $max_iters $tol $start_T 0 2 > $OUT_DIR/output_j_N_${n}_threads_${n_threads}_v${version}.txt
            # todo: add other better implmentations
        done
        OMP_NUM_THREADS=${n_threads} ./poisson_gs $n $max_iters $tol $start_T 0 2 > $OUT_DIR/output_gs_N_${n}_threads_${n_threads}_v${version}.txt
    done;echo

   
done

