TARGET= libmatmult.so
OBJS	= matmult.o

# variables
TEAMS=16384
THREADS=16
SLABS=2
BLOCK_SIZE=16

CC	= nvc
CXX	= nvc++

OPT	= -g -fast -Msafeptr -Minfo -mp=gpu -gpu=pinned -gpu=cc80 -gpu=lineinfo -cuda -mp=noautopar
PIC   = -fpic -shared
ISA	= 
PARA	=  -fopenmp
INC   = -I$(CUDA_ROOT)/include -I/appl/nvhpc/2022_2211/Linux_x86_64/22.11/examples/OpenMP/SDK/include
LIBS	=
DEFS = -D_TEAMS=$(TEAMS) -D_THREADS=$(THREADS) -D_SLABS=$(SLABS) -D_BLOCK_SIZE=$(BLOCK_SIZE)

CXXFLAGS= $(OPT) $(PIC) $(INC) $(ISA) $(PARA) $(XOPT) $(DEFS)
CFLAGS= $(OPT) $(PIC) $(INC) $(ISA) $(PARA) $(XOPT) $(DEFS)

all: clean $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS) 

clean:
	@/bin/rm -f $(TARGET) $(OBJS)


T1_TEST = 50
T2_TEST = 60
T3_TEST = 70
THREADS_TEST = 16
test: THREADS=$(THREADS_TEST) 
test: clean all
	@echo
	@echo "Expected result: "
	@echo "83.594    151.173 0 # matmult_nat"
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ mkn_omp $(T1_TEST) $(T2_TEST) $(T3_TEST)
	@echo "Testing matmult_blk_omp"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ blk_omp $(T1_TEST) $(T2_TEST) $(T3_TEST) 15
	@echo "Testing matmult_lib_omp"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ lib $(T1_TEST) $(T2_TEST) $(T3_TEST)
	@echo "Testing matmult_mkn_offload"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ mkn_offload $(T1_TEST) $(T2_TEST) $(T3_TEST)
	@echo "Testing matmult_mnk_offload"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ mnk_offload $(T1_TEST) $(T2_TEST) $(T3_TEST)
	@echo "Testing matmult_blk_offload"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ blk_offload $(T1_TEST) $(T2_TEST) $(T3_TEST) 15
	@echo "Testing matmult_asy_offload"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ asy_offload $(T1_TEST) $(T2_TEST) $(T3_TEST)
	@echo "Testing matmult_lib_offload"
	OMP_NUM_THREADS=$(THREADS_TEST) ./matmult_c.nvc++ lib_offload $(T1_TEST) $(T2_TEST) $(T3_TEST)

# T1 = 512
# T2 = 513
# T3 = 514
T1 = 2048
T2 = 2049
T3 = 2050

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

ASY_T = 512 # must be divisible by number of slabs
test_asy: DEFS += -D_BLK_SIZE=16
test_asy: clean all
	export MFLOPS_MIN_T=0.0
	@echo "Testing matmult_blk_omp"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ blk_omp $(ASY_T) $(ASY_T) $(ASY_T) 16
	@echo
	@echo "Testing matmult_blk_offload"
	./matmult_c.nvc++ blk_offload $(ASY_T) $(ASY_T) $(ASY_T)
	@echo
	@echo "Testing matmult_asy_offload"
	./matmult_c.nvc++ asy_offload $(ASY_T) $(ASY_T) $(ASY_T)
	@echo

test_lib: clean all
	@echo "Testing matmult_lib"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ lib $(T1) $(T2) $(T3)
	@echo
	@echo "Testing matmult_lib_offload"
	./matmult_c.nvc++ lib_offload $(T1_TEST) $(T2_TEST) $(T3_TEST)
	@echo

test_teams: clean all
	@echo "Testing matmult_teams"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ mkn_offload $(T1) $(T2) $(T3)
	@echo

S_TIME = 512
test_time: DEFS += -D_SLABS=2
test_time: clean all
	export MFLOPS_MIN_T=0.0
	@echo "Testing matmult_blk_offload"
	MFLOPS_MIN_T=0.0 ./matmult_c.nvc++ blk_offload $(S_TIME) $(S_TIME) $(S_TIME)
	@echo
	@echo "Testing matmult_asy_offload"
	MFLOPS_MIN_T=0.0 ./matmult_c.nvc++ asy_offload $(S_TIME) $(S_TIME) $(S_TIME)
	@echo


# dependencies
matmult.o: matmult.h matmult.c
# lib_offload.o: lib_offload.h lib_offload.cpp
