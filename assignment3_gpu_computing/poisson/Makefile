TARGET= poisson
OBJS	= main.o \
        jacobi.o \
        init.o \
        print.o \
        alloc3d.o \
        #input.o \

TEAMS=16384
THREADS=16

CC	= nvc
CXX	= nvc++

OPT	= -g -fast -Msafeptr -Minfo -acc -mp=gpu -gpu=pinned -gpu=cc80 -gpu=lineinfo -cuda -mp=noautopar
ISA	= 
PARA	= 
INC   = -I$(CUDA_ROOT)/include -I/appl/nvhpc/2022_2211/Linux_x86_64/22.11/examples/OpenMP/SDK/include
LIBS	= -lcuda
DEFS = -D_TEAMS=$(TEAMS) -D_THREADS=$(THREADS)

CFLAGS = $(OPT) $(INC) $(ISA) $(PARA) $(XOPT) $(DEFS)
CXXFLAGS = $(OPT) $(INC) $(ISA) $(PARA) $(XOPT) $(DEFS)

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
clean:
	@/bin/rm -f *.o $(TARGET)

test: clean all
	@echo "Testing Jacobi with Map clause"
	./poisson 200 100 0.01 0 0 0
	@echo
	@echo "Testing Jacobi with optimization from assignment 2"
	./poisson 200 100 0.01 0 0 1
	@echo
	@echo "Testing Jacobi with optimization on the GPU"
	./poisson 200 100 0.01 0 0 2
	@echo
	@echo "Testing Jacobi with optimization on dual GPUs"
	export CUDA_VISIBLE_DEVICES=0,1
	./poisson 200 100 0.01 0 0 3
	@echo
	@echo "Testing Jacobi with map clause and normalization calculation"
	./poisson 200 100 0.01 0 0 4


main.o: main.cpp jacobi.h init.h print.h alloc3d.h
jacobi.o: jacobi.h
init.o: init.h
# input.o: input.h
print.o: print.h
alloc3d.o: alloc3d.h