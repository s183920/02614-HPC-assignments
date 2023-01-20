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

module load gcc
# module load python3
# source ../../../venv/bin/activate

# set Ns to test
Ns="128 256"
thread_step_size=1

#LSB_DJOB_NUMPROC=4



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
        OMP_NUM_THREADS=$n_threads ./poisson_j $n $max_iters $tol $start_T 0 1 > $OUT_DIR/output_j_N_${n}_threads_${n_threads}_v1.txt
        #OMP_NUM_THREADS=$n_threads ./poisson_gs $n $max_iters $tol $start_T 0 2 > $OUT_DIR/output_gs_N_${n}_threads_${n_threads}_v1.txt
        #OMP_NUM_THREADS=$n_threads ./poisson_j $n $max_iters $tol $start_T 0 2 > $OUT_DIR/output_j_N_${n}_threads_${n_threads}_v2.txt
        #OMP_NUM_THREADS=$n_threads ./poisson_gs $n $max_iters $tol $start_T 0 0 2 > $OUT_DIR/output_gs_N_${n}_threads_${n_threads}_v2.txt
    done
done
export OMP_PLACES=threads
export OMP_PROC_BIND=close
export OMP_SCHEDULE=dynamic,1
for n_threads in $( eval echo {1..$LSB_DJOB_NUMPROC..$thread_step_size} ); do
    echo "Threads = $n_threads"
    for n in $Ns; do
        echo "N = $n"
        #OMP_NUM_THREADS=$n_threads ./poisson_j $n $max_iters $tol $start_T 0 1 > $OUT_DIR/output_j_N_${n}_threads_${n_threads}_v1.txt
        OMP_NUM_THREADS=$n_threads ./poisson_gs $n $max_iters $tol $start_T 0 2 > $OUT_DIR/output_gs_N_${n}_threads_${n_threads}_v1.txt
        OMP_NUM_THREADS=$n_threads ./poisson_j $n $max_iters $tol $start_T 0 2 > $OUT_DIR/output_j_N_${n}_threads_${n_threads}_v2.txt
        #OMP_NUM_THREADS=$n_threads ./poisson_gs $n $max_iters $tol $start_T 0 0 2 > $OUT_DIR/output_gs_N_${n}_threads_${n_threads}_v2.txt
    done
done

echo "Making plots of thread scalability..."
python3 plot_threads.py --save_folder $EXP_DIR --output_files $OUT_DIR
# echo "Making plots of performance..."
# python3 plot_totalperformance.py --save_folder $EXP_DIR --output_files $OUT_DIR
