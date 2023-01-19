TARGET= libmatmult.so
OBJS	= matmult.o

CC	= nvc
CXX	= nvc++

OPT	= -g -fast -Msafeptr -Minfo -mp=gpu -gpu=pinned -gpu=cc80 -gpu=lineinfo -cuda -mp=noautopar
PIC   = -fpic -shared
ISA	= 
PARA	=  -fopenmp
INC   = -I$(CUDA_ROOT)/include -I/appl/nvhpc/2022_2211/Linux_x86_64/22.11/examples/OpenMP/SDK/include
LIBS	=
DEFS = -D_TEAMS=16384 -D_THREADS=16

CXXFLAGS= $(OPT) $(PIC) $(INC) $(ISA) $(PARA) $(XOPT) $(DEFS)
CFLAGS= $(OPT) $(PIC) $(INC) $(ISA) $(PARA) $(XOPT) $(DEFS)

all: clean $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS) 

# $(TARGET): $(OBJS) 
# 	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	@/bin/rm -f $(TARGET) $(OBJS)


# test: clean all
# 	@echo "Expected result: "
# 	@echo "83.594    151.173 0 # matmult_nat"
# 	@echo
# 	@echo "Testing matmult_mkn_omp"
# 	OMP_NUM_THREADS=2 ./matmult_c.nvc++ mkn_omp 50 60 70
# 	@echo
# 	# @echo "Testing matmult_blk_omp"
# 	#OMP_NUM_THREADS=2 ./matmult_c.nvc++ blk_omp 50 60 70
# 	# @echo
# 	@echo "Testing matmult_lib"
# 	OMP_NUM_THREADS=2 ./matmult_c.nvc++ lib 50 60 70
# 	@echo
# 	@echo "Testing matmult_mkn_offload"
# 	OMP_NUM_THREADS=2 ./matmult_c.nvc++ mkn_offload 50 60 70
# 	@echo
# 	# @echo "Testing matmult_mnk_offload"
# 	#OMP_NUM_THREADS=2 ./matmult_c.nvc++ mnk_offload 50 60 70
# 	# @echo

# T1 = 512
# T2 = 513
# T3 = 514
T1 = 2048
T2 = 2049
T3 = 2050

test: clean all
	@echo "Expected:"
	@echo "98304.000 6528.841 300 # matmult_mnk_offload"
	@echo "Testing matmult_mkn_offload"
	./matmult_c.nvc++ mnk_offload 2048 2048 2048
	@echo

STANDARD_T1 = 512
STANDARD_T2 = 512
STANDARD_T3 = 512
test_standard: clean all
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=1 ./matmult_c.nvc++ mkn_omp $(STANDARD_T1) $(STANDARD_T2) $(STANDARD_T3)
	@echo
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ mkn_omp $(STANDARD_T1) $(STANDARD_T2) $(STANDARD_T3)
	@echo
	@echo "Testing matmult_blk_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ blk_omp $(STANDARD_T1) $(STANDARD_T2) $(STANDARD_T3) 15
	@echo
	@echo "Testing matmult_lib_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ lib $(STANDARD_T1) $(STANDARD_T2) $(STANDARD_T3)
	@echo


test_speedup: clean all
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ mkn_omp $(T1) $(T2) $(T3)
	@echo
	@echo "Testing matmult_mkn_offload"
	./matmult_c.nvc++ mkn_offload $(T1) $(T2) $(T3)
	@echo

test_offload: clean all
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ mkn_omp $(T1) $(T2) $(T3)
	@echo
	@echo "Testing matmult_lib"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ lib $(T1) $(T2) $(T3)
	@echo
	@echo "Testing matmult_mkn_offload"
	./matmult_c.nvc++ mkn_offload $(T1) $(T2) $(T3)
	@echo
	@echo "Testing mnk_offload"
	./matmult_c.nvc++ mnk_offload $(T1) $(T2) $(T3)
	@echo

test_blk: clean all
	@echo "Testing matmult_blk_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ blk_omp $(T1) $(T2) $(T3) 15
	@echo
	@echo "Testing matmult_blk_offload"
	./matmult_c.nvc++ blk_offload $(T1) $(T2) $(T3)
	@echo

ASY_T = 2048 # must be divisible by number of slabs
test_asy: clean all
	export MFLOPS_MIN_T=0.0
	@echo "Testing matmult_blk_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ blk_omp $(ASY_T) $(ASY_T) $(ASY_T)
	@echo
	@echo "Testing matmult_blk_offload"
	./matmult_c.nvc++ blk_offload $(ASY_T) $(ASY_T) $(ASY_T)
	@echo
	@echo "Testing matmult_asy_offload"
	./matmult_c.nvc++ asy_offload $(ASY_T) $(ASY_T) $(ASY_T)
	@echo

test_lib: clean all
	@echo "Testing matmult_lib"
	OMP_NUM_THREADS=2 ./matmult_c.nvc++ lib $(T1) $(T2) $(T3)
	@echo
	@echo "Testing matmult_lib"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ lib $(T1) $(T2) $(T3)
	@echo

test_teams: clean all
	@echo "Testing matmult_teams"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ mkn_offload $(T1) $(T2) $(T3)
	@echo

# dependencies
matmult.o: matmult.h matmult.c
