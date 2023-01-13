EXP_DIR="$RESULT_FOLDER/dynamic_allocation_chunksize"
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
N=500
step_size=1
max_chunk=10
versions=2

# LSB_DJOB_NUMPROC=20




# save settings
echo "start_T = $start_T" >> $EXP_DIR/setup.txt
echo "max_iters = $max_iters" >> $EXP_DIR/setup.txt
echo "tol = $tol" >> $EXP_DIR/setup.txt
echo "Ns = $Ns" >> $EXP_DIR/setup.txt
echo "Num_threads = $LSB_DJOB_NUMPROC" >> $EXP_DIR/setup.txt
echo "thread_step_size = $thread_step_size" >> $EXP_DIR/setup.txt


# run tests - WARNING the programs does not ouput the needed data yet
echo "Running dynamic chunksize experiment for parallel code"
#OMP_NUM_THREADS=$LSB_DJOB_NUMPROC ./poisson_gs $N $max_iters $tol $start_T 0 $version > $OUT_DIR/output_gs_N_${n}_threads_${LSB_DJOB_NUMPROC}_v_static.txt
  
for chunk_size in $( eval echo {1..$max_chunk..$step_size} ); do
    echo '"'"Chunk_size=${chunk_size}"'"'
    
    OMP_SCHEDULE=static,$chunk_size OMP_NUM_THREADS=$LSB_DJOB_NUMPROC ./poisson_j $N $max_iters $tol $start_T 0 2 > $OUT_DIR/output_j_N_${n}_threads_${LSB_DJOB_NUMPROC}_v_static_${chunk_size}.txt
    #OMP_SCHEDULE=“dynamic,$chunk_size” OMP_NUM_THREADS=$LSB_DJOB_NUMPROC ./poisson_gs $N $max_iters $tol $start_T 0 $version > $OUT_DIR/output_gs_N_${n}_threads_${LSB_DJOB_NUMPROC}_v_dynamic_${chunk_size}.txt
    OMP_SCHEDULE=dynamic,$chunk_size OMP_NUM_THREADS=$LSB_DJOB_NUMPROC ./poisson_j $N $max_iters $tol $start_T 0 2 > $OUT_DIR/output_j_N_${n}_threads_${LSB_DJOB_NUMPROC}_v_dynamic_${chunk_size}.txt
done