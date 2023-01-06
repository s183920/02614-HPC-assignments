# compile the code
module load gcc
make realclean
make test

# write files
files=(
    "experiment*.sh"
    "Makefile"
    "libmatmult.so"
    "matrix.c"
    "matmult_c.gcc"
    "plot_blk.py"
    "plot_size.py"
    "help_funcs.py"
    "compiler_optimization.py"
)

# make zip
zip -r code.zip "${files[@]}"