# Makefile
#
TARGET_J  = poisson_j		# Jacobi
TARGET_GS = poisson_gs		# Gauss-Seidel

SOURCES	= main.c print.c alloc3d.c helper.c
OBJECTS	= print.o alloc3d.o helper.o $(MATRIX_OBJECT)
MAIN_J	= main_j.o
MAIN_GS = main_gs.o
OBJS_J	= $(MAIN_J) jacobi.o
OBJS_GS	= $(MAIN_GS) gauss_seidel.o
MATRIX_SOURCE = matrix_init$(test_ext).c
MATRIX_OBJECT = matrix_init$(test_ext).o
MATRIX_HEADER = matrix_init$(test_ext).h

# options and settings for the GCC compilers
#
CC	= gcc
DEFS	= 
OPT	= -g #-std=c99 #note: -std=c99 does not support M_PI
XOPTS	= 
IPO	= 
ISA	= 
CHIP	= 
ARCH	= 
PARA	= -fopenmp
CFLAGS	= $(DEFS) $(ARCH) $(OPT) $(ISA) $(CHIP) $(IPO) $(PARA) $(XOPTS)
LDFLAGS = -lm 
TEST_FLAG =

all: SOURCES += $(MATRIX_SOURCE)
all: realclean $(TARGET_J) $(TARGET_GS) 


$(TARGET_J): $(OBJECTS) $(OBJS_J)
	echo $(CC) -o $@ $(CFLAGS) $(OBJS_J) $(OBJECTS) $(LDFLAGS) > compile.log
	$(CC) -o $@ $(TEST_FLAG) $(CFLAGS) $(OBJS_J) $(OBJECTS) $(LDFLAGS)
$(TARGET_GS): $(OBJECTS) $(OBJS_GS)
	echo $(CC) -o $@ $(CFLAGS) $(OBJS_GS) $(OBJECTS) $(LDFLAGS) >> compile.log
	$(CC) -o $@ $(TEST_FLAG) $(CFLAGS) $(OBJS_GS) $(OBJECTS) $(LDFLAGS)

$(MAIN_J):
	$(CC) -o $@ -D_JACOBI $(TEST_FLAG) $(CFLAGS) -c main.c 

$(MAIN_GS):
	$(CC) -o $@ -D_GAUSS_SEIDEL $(TEST_FLAG) $(CFLAGS) -c main.c 


# cleaning functions
clean:
	@/bin/rm -f core *.o *~

realclean: clean
	@/bin/rm -f $(TARGET_J)  $(TARGET_GS)

cleanresults:
	@/bin/rm -rf ../results/*

# save results
SAVEKEY="blk"
saveresults: 
	@mkdir -p ../results_saved
	@cp -r ../results/*$(SAVEKEY)* ../results_saved/

test: TEST_FLAG += -D_TEST 
test: all
	@echo
	@echo "Running Jacobi test for N=50"
	@echo
	@echo "Serial"
	./poisson_j 50 20000 0.0001 0 0 0
	@echo
	@echo "Parallel simple"
	./poisson_j 50 20000 0.0001 0 0 1
	@echo
	@echo "Parallel optimized"
	./poisson_j 50 20000 0.0001 0 0 2

	@echo
	@echo "Running Gauss Seidel test for N=50"
	@echo
	@echo "Serial"
	./poisson_gs 50 20000 0.0001 0 0 0
	@echo
	@echo "Parallel simple"
	./poisson_gs 50 20000 0.0001 0 0 1
	@echo
	@echo "Parallel optimized"
	./poisson_gs 50 20000 0.0001 0 0 2

	@echo


test_par: TEST_FLAG += -D_TEST 
test_par: all
	# @echo
	# @echo "Serial"
	# ./poisson_j 50 200 0.0001 0 0 0
	# @echo
	# @echo "1 thread"
	# OMP_NUM_THREADS=1 ./poisson_j 50 200 0.0001 0 0 1
	# @echo
	# @echo "8 threads"
	# OMP_NUM_THREADS=8 ./poisson_j 50 200 0.0001 0 0 1
	# @echo

	@echo
	@echo "Serial"
	./poisson_gs 50 200 0.0001 0 0 0
	@echo
	@echo "1 thread"
	OMP_NUM_THREADS=1 ./poisson_gs 50 200 0.0001 0 0 1
	@echo
	@echo "8 threads"
	OMP_NUM_THREADS=8 ./poisson_gs 50 200 0.0001 0 0 1
	@echo
# DO NOT DELETE

main_j.o: main.c print.h jacobi.h helper.h $(MATRIX_HEADER)
main_gs.o: main.c print.h gauss_seidel.h helper.h $(MATRIX_HEADER)
print.o: print.h
helper.o: helper.h
$(MATRIX_OBJECT): $(MATRIX_SOURCE) $(MATRIX_HEADER)
