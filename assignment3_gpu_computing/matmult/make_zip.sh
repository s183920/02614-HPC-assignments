# compile the code
module load gcc
make realclean
make

# write files
files=(
    "matmult_c.nc++"
    "matmult_c.c"
    "matmult_c.h"
    
)

# make zip
zip -r code.zip "${files[@]}"