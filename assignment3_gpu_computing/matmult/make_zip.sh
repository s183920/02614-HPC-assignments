# compile the code
module load gcc
make realclean
make

# write files
files=(
    "matmult_c.nc++"
    "matmult_c.c"
    "matmult_c.h"
    "Makefile"
    "plot_functions.py"
    "profiler.sh"
    "test_all.sh"
    "load_modules.sh"
    "io_functions.py"
    "io2.py"
    "ex1.sh"
    "ex2.sh"
    "ex4.sh"
    "ex5.sh"
    "experiment_blk.sh"
    "experiment.sh"
)

# make zip
zip -r code.zip "${files[@]}"