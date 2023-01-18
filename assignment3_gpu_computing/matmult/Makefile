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
DEFS = -D_T50

CXXFLAGS= $(OPT) $(PIC) $(INC) $(ISA) $(PARA) $(XOPT)
CFLAGS= $(OPT) $(PIC) $(INC) $(ISA) $(PARA) $(XOPT)

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS) $(DEFS)

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

test_standard: clean all
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=1 ./matmult_c.nvc++ mkn_omp 512 513 514
	@echo
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=8 ./matmult_c.nvc++ mkn_omp 512 513 514
	@echo
	# @echo "Testing matmult_blk_omp"
	#OMP_NUM_THREADS=2 ./matmult_c.nvc++ blk_omp 512 513 514
	# @echo


test_speedup: clean all
	@echo "Testing matmult_mkn_omp"
	OMP_NUM_THREADS=2 ./matmult_c.nvc++ mkn_omp 50 60 70
	@echo
	@echo "Testing matmult_mkn_offload"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ mkn_omp 50 60 70
	@echo

test_lib: clean all
	@echo "Testing matmult_lib"
	OMP_NUM_THREADS=2 ./matmult_c.nvc++ lib 512 512 512
	@echo
	@echo "Testing matmult_lib"
	OMP_NUM_THREADS=16 ./matmult_c.nvc++ lib 512 512 512
	@echo

test_offload: clean all
	@echo "Testing matmult_mkn_omp"
	./matmult_c.nvc++ mkn_omp 512 512 512
	@echo
	@echo "Testing matmult_lib"
	./matmult_c.nvc++ lib 512 512 512
	@echo
	@echo "Testing matmult_mkn_offload"
	./matmult_c.nvc++ mkn_offload 512 512 512
	@echo
	@echo "Testing mnk_offload"
	./matmult_c.nvc++ mnk_offload 512 512 512
	@echo
# dependencies
matmult.o: matmult.h matmult.c
