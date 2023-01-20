# compile the code
module load gcc
make realclean
make

# write files
files=(
    "poisson_gs"
    "poisson_j"
    "batch_job.sh"
    "exp_dynamic_allocation.sh"
    "exp_error_convergence.sh"
    "exp_iteration_convergence.sh"
    "exp_memory_scalability.sh"
    "exp_scalability.sh"
    "exp_thread_scalability.sh"
    "gauss_seidel.c"
    "gauss_seidel.h"
    "help_funcs.py"
    "helper.c"
    "helper.h"
    "jacobi.c"
    "jacobi.h"
    "main.c"
    "matrix_init.c"
    "matrix_init.h"
    "matrix_init_test.c"
    "matrix_init_test.h"
    "plot_error_convergence.py"
    "plot_iteration_convergence.py"
    "plot_threads.py"
    "plot_totalperformance.py"
)

# make zip
zip -r code.zip "${files[@]}"