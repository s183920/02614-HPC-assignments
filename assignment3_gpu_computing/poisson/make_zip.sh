# compile the code
module load gcc
make realclean
make

# write files
files=(
    "alloc3d.cpp"
    "alloc3d.h"
    "exp_performancehitnorm.sh"
    "exp_speedup.sh"
    "experiment.sh"
    "help_funcs.py"
    "init.cpp"
    "init.h"
    "jacobi.cpp"
    "jacobi.h"
    "main.cpp"
    "Makefile"
    "plot_speedup.py"
    "print.cpp"
    "print.h"
)

# make zip
zip -r code.zip "${files[@]}"