TARGET= poisson
OBJS	= main.o \
        jacobi.o \
        init.o \
        print.o \
        alloc3d.o \
        #input.o \

CC	= nvc
CXX	= nvc++

OPT	= -g -fast -Msafeptr -Minfo -acc -mp=gpu -gpu=pinned -gpu=cc80 -gpu=lineinfo -cuda -mp=noautopar
ISA	= 
PARA	= 
INC   = -I$(CUDA_ROOT)/include -I/appl/nvhpc/2022_2211/Linux_x86_64/22.11/examples/OpenMP/SDK/include
LIBS	= -lcuda

CFLAGS = $(OPT) $(INC) $(ISA) $(PARA) $(XOPT)
CXXFLAGS = $(OPT) $(INC) $(ISA) $(PARA) $(XOPT)

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
clean:
	@/bin/rm -f *.o $(TARGET)

test: clean all
	@echo "Testing Jacobi with Map clause"
	OMP_NUM_THREADS=16 ./poisson 200 100 0.01 0 0 0
	@echo
	@echo "Testing Jacobi with optimization from assignment 2"
	OMP_NUM_THREADS=16 ./poisson 200 100 0.01 0 0 1

main.o: main.cpp jacobi.h init.h print.h alloc3d.h
jacobi.o: jacobi.h
init.o: init.h
# input.o: input.h
print.o: print.h
alloc3d.o: alloc3d.h